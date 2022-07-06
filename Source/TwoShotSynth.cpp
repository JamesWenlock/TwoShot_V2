/*
  ==============================================================================

    TwoShotTwoShotSynth.cpp
    Created: 17 Jun 2022 3:29:38pm
    Author:  Deuel Lab

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TwoShotSynth.h"

/**
* Has 2 modes:
* monophonic SAMPLE MODE
*  - in this mode, the user can detune the sample from the UI
*  - when a user plays notes, the sample is repitched to match the midi key (like a standard TwoShotSynth)
* polyphonic LOOP MODE (with BPM)
*  - In this mode, the loop is time-stretched to synchronise with the host's BPM
*  - when a user plays notes, the sample is played from different time locations (skip back and forward through the loop, rather than repitch it)
*  - the amount skipped forward / backwards should be 1 BAR at a time
*  - Here's an example: https://www.youtube.com/watch?v=B-xQP7_vI64
*/

TwoShotSynth::TwoShotSynth() : 
    m_audioSampleRate(44100),
    m_audioBPM(120),
    m_isReversed(false),
    m_isLoop(true),
    m_midiNaturalNote(64)
{
    //m_soundTouch.setChannels(2);
    //m_soundTouch.setSampleRate(m_audioSampleRate);
    //m_soundTouch.setRate(1.0);
    //m_soundTouch.setPitch(0.5);
    //m_soundTouch.flush();

    int numVoices = 16;

    for (auto i = 0; i < numVoices; ++i)
    {
        m_synth.addVoice(new TwoShotVoice());
    }
}

TwoShotSynth::~TwoShotSynth()
{

}

/**
* Updates the audio for the TwoShotSynth
* @param audioBpm if this value is present, then this is a polyphonic Loop, and the TwoShotSynth goes into LOOP MODE
*/
void TwoShotSynth::setAudio(
    juce::AudioFormatReader &source,
    const double audioSampleRate,
    std::optional<const double> audioBpm,
    const double sliceLength,
    const double maxSampleLength,
    const double initAtk,
    const double initDec,
    const int midiNaturalNote
)
{
    m_synth.clearSounds();
    m_midiNaturalNote = midiNaturalNote;
    if (audioBpm.has_value())
    {
        m_audioSampleRate = audioSampleRate;
        m_audioBPM = audioBpm.value();
        m_isLoop = true;
        setIsLoop(true);
        double beatsPerSecond = (audioBpm.value() / 60.0);
        double beatsPerSample = beatsPerSecond / audioSampleRate;
        double samplesPerBeat = 1.0 / beatsPerSample;
        int samplesPerBar = (int) samplesPerBeat * sliceLength;
        int fadeLength = 70;
        int startSample = 0;
        int numSamples = jmin((int) samplesPerBar, (int) source.lengthInSamples);
        int i = 0;
        while (numSamples >= fadeLength && startSample < source.lengthInSamples)
        {
            BigInteger range;
            range.setBit(m_midiNaturalNote + i);
            m_synth.addSound(new TwoShotSound(
                source, 
                range, 
                m_midiNaturalNote + i, 
                startSample, 
                numSamples, 
                fadeLength, 
                initAtk, 
                initDec, 
                maxSampleLength
            ));
            startSample += samplesPerBar;
            numSamples = jmin((int)samplesPerBar, (int)source.lengthInSamples - startSample);
            i++;
        }
    }
    else
    {
        m_isLoop = false;
        setIsLoop(false);
        BigInteger range;
        range.setRange(0, 127, true);
        m_audioSampleRate = audioSampleRate;
        m_synth.addSound(new TwoShotSound(
            source, 
            range, 
            m_midiNaturalNote, 
            0.01, 
            0.01, 
            120
        ));
    }
    if (m_isReversed)
    {
        m_isReversed = false;
        setReverse(true);
    }
}

/**
* This is called when the host updates it's sampleRate
*/
void TwoShotSynth::setHostSampleRate(const double currentSampleRate)
{
    m_synth.setCurrentPlaybackSampleRate(currentSampleRate);
}

/**
* This is called when the user clicks the reverse toggle in the UI
*/
void TwoShotSynth::setReverse(const bool isReversed)
{
    if (m_isReversed != isReversed)
    {
        reverse();
        m_isReversed = isReversed;
    }
}

void TwoShotSynth::reverse()
{
    if (m_isLoop)
    {
        for (int i = 0; i < m_synth.getNumSounds(); ++i)
        {
            const int numSamples = getSamplerAudio(i)->getNumSamples();

            getSamplerAudio(i)->reverse(0, numSamples);
            auto sound = dynamic_cast<TwoShotSound*>(m_synth.getSound(i).get());
            if (sound)
            {
                BigInteger midiNotes;
                int midiIndex = (m_synth.getNumSounds() + m_midiNaturalNote - 1) - i;
                DBG(midiIndex);
                if (m_isReversed)
                {
                    midiIndex = m_midiNaturalNote + i;
                }
                midiNotes.setBit(midiIndex);
                sound->setMidiNotes(midiNotes, midiIndex);
            }
        }
    }
    else
    {
        const int soundIndex = m_synth.getNumSounds() - 1;
        const int numSamples = getSamplerAudio(soundIndex)->getNumSamples();
        getSamplerAudio(soundIndex)->reverse(soundIndex, numSamples);
    }
}

/**
* changes the TwoShotSynth attack of the envelope
*/
void TwoShotSynth::setAttack(const double attackSeconds)
{
    m_adsrParams.attack = static_cast<float>(attackSeconds);
    updateADSR();
}

/**
* changes the TwoShotSynth decay of the envelope
*/
void TwoShotSynth::setDecay(const double decaySeconds)
{
    m_adsrParams.release = static_cast<float>(decaySeconds);
    updateADSR();
}

/**
* This is called when the user changes the pitch, from within the UI
*/
void TwoShotSynth::setDetune(const double detuneAmount)
{
    for (int i = 0; i < m_synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<TwoShotVoice*>(m_synth.getVoice(i)))
        {
            voice->setDetune(detuneAmount);
        }
    }
}

/**
* This is called when user switches to mode 2
*/
void TwoShotSynth::setIsLoop(const bool isLoop)
{
    for (int i = 0; i < m_synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<TwoShotVoice*>(m_synth.getVoice(i)))
        {
            voice->setIsLoop(isLoop);
        }
    }
}

/**
* This is called from the JUCE process block method
*/
void TwoShotSynth::processNextBlock(
    juce::AudioBuffer<float>& outputAudio,
    const juce::MidiBuffer& midiData,
    std::optional<const double> currentHostBpm
)
{
    if (currentHostBpm.has_value())
    {
        //m_soundTouch.setPitch(m_audioBPM / currentHostBpm.value());
        for (int i = 0; i < m_synth.getNumVoices(); ++i)
        {
            if (auto voice = dynamic_cast<TwoShotVoice*>(m_synth.getVoice(i)))
            {
                voice->setBPMComp(m_audioBPM, currentHostBpm.value());
            }
        }
    }
    m_synth.renderNextBlock(outputAudio, midiData, 0, outputAudio.getNumSamples());
    //int nch = 2;
    //// copy input samples in interleaved format to helper buffer
    //for (int i = 0; i < nch; ++i)
    //    for (int j = 0; j < outputAudio.getNumSamples(); ++j)
    //        m_buf[j * nch + i] = outputAudio.getSample(i, j);
    //m_soundTouch.putSamples(m_buf.data(), outputAudio.getNumSamples());
    //if (m_soundTouch.numSamples() >= outputAudio.getNumSamples()) // does SoundTouch have enough samples ready?
    //{
    //    m_soundTouch.receiveSamples(m_buf.data(), outputAudio.getNumSamples());
    //    // copy SoundTouch output samples to split format Juce buffer
    //    for (int i = 0; i < nch; ++i)
    //        for (int j = 0; j < outputAudio.getNumSamples(); ++j)
    //            outputAudio.setSample(i, j, m_buf[j * nch + i]);
    //}
    //else
    //{
    //    // SoundTouch didn't have enough output samples, just output silence
    //    outputAudio.clear();
    //}
}


AudioBuffer<float>* TwoShotSynth::getSamplerAudio(int soundIndex)
{
    // get the last added synth sound as a TwoShotSound*
    auto sound = dynamic_cast<TwoShotSound*>(m_synth.getSound(soundIndex).get());

    if (sound)
    {
        return sound->getAudioData();
    }

    // just in case it somehow happens that the sound doesn't exist or isn't a SamplerSound,
    // return a static instance of an empty AudioBuffer here...
    static AudioBuffer<float> dummybuffer;
    return &dummybuffer;
}

void TwoShotSynth::updateADSR()
{
    for (int i = 0; i < m_synth.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<TwoShotSound*>(m_synth.getSound(i).get()))
        {
            sound->setEnvelopeParameters(m_adsrParams);
        }
    }
}