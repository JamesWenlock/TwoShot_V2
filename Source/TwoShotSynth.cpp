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

TwoShotSynth::TwoShotSynth()
{
    int numVoices = 3;

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
    const size_t sampleProgress
)
{
    BigInteger range;
    range.setRange(0, 128, true);
    m_synth.addSound(new TwoShotSound("sample", source, range, 64, 0.01, 0.01, 20));
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
}

/**
* changes the TwoShotSynth attack of the envelope
*/
void TwoShotSynth::setAttack(const double attackSeconds)
{
}

/**
* changes the TwoShotSynth decay of the envelope
*/
void TwoShotSynth::setDecay(const double decaySeconds)
{
}

/**
* This is called when the user changes the pitch, from within the UI
*/
void TwoShotSynth::setDetune(const double detuneAmount)
{
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
    m_synth.renderNextBlock(outputAudio, midiData, 0, outputAudio.getNumSamples());
}