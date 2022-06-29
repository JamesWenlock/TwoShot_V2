/*
  ==============================================================================

    TwoShotSampler.h
    Created: 17 Jun 2022 3:29:38pm
    Author:  Deuel Lab

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TwoShotSound.h"
#include "TwoShotVoice.h"

/**
 * Has 2 modes:
 * monophonic SAMPLE MODE
 *  - in this mode, the user can detune the sample from the UI
 *  - when a user plays notes, the sample is repitched to match the midi key (like a standard Synth)
 * polyphonic LOOP MODE (with BPM)
 *  - In this mode, the loop is time-stretched to synchronise with the host's BPM
 *  - when a user plays notes, the sample is played from different time locations (skip back and forward through the loop, rather than repitch it)
 *  - the amount skipped forward / backwards should be 1 BAR at a time
 *  - Here's an example: https://www.youtube.com/watch?v=B-xQP7_vI64
 */
class TwoShotSampler
{
    public:
        TwoShotSampler();
        ~TwoShotSampler();

        /**
         * Updates the audio for the Synth
         * @param audioBpm if this value is present, then this is a polyphonic Loop, and the Synth goes into LOOP MODE
         */
        void setAudio(
            juce::AudioBuffer<float>&& buffer,
            const double audioSampleRate,
            std::optional<const double> audioBpm,
            const size_t sampleProgress = 0
        );

        ///**
        // * This is called when the host updates it's sampleRate
        // */
        void setHostSampleRate(const double currentSampleRate);

        //void setHostBlockSize(const uint blockSize);

        /**
         * This is called when the user clicks the reverse toggle in the UI
         */
        void setReverse(const bool isReversed);

        /**
         * changes the Synth attack of the envelope
         */
        void setAttack(const double attackSeconds);

        /**
         * changes the Synth decay of the envelope
         */
        void setDecay(const double decaySeconds);

        /**
         * This is called when the user changes the pitch, from within the UI
         */
        void setDetune(const double detuneAmount);

        //void setVoiceSampleRate(const uint sampleRate);


        /**
         * This is called from the JUCE process block method
         */
        void processNextBlock(
            juce::AudioBuffer<float>& outputAudio,
            const juce::MidiBuffer& midiData,
            std::optional<const double> currentHostBpm
        );

    private:
        AudioBuffer<float>* getSamplerAudio(int soundIndex);
        void setIsLoop(const bool isLoop);
        void updateADSR();
        void reverse();
        juce::Synthesiser m_synth;
        juce::ADSR::Parameters m_adsrParams;
        std::atomic<double> m_audioSampleRate;
        std::atomic<double> m_audioBPM;
        std::atomic<bool> m_isReversed;
        std::atomic<bool> m_isLoop;
        std::atomic<int> m_midiNaturalNote;
        //soundtouch::SoundTouch m_soundTouch;
        //std::vector<float> m_buf;
};
