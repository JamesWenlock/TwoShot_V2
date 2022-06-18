/*
  ==============================================================================

    TwoShotVoice.h
    Created: 17 Jun 2022 3:26:01pm
    Author:  Deuel Lab

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//==============================================================================
/**
    A subclass of SynthesiserVoice that can play a SamplerSound.
    To use it, create a Synthesiser, add some TwoShotVoice objects to it, then
    give it some SampledSound objects to play.
    @see SamplerSound, Synthesiser, SynthesiserVoice
    @tags{Audio}
*/

namespace juce
{
    class TwoShotVoice : public SynthesiserVoice
    {
    public:
        //==============================================================================
        /** Creates a TwoShotVoice. */
        TwoShotVoice();

        /** Destructor. */
        ~TwoShotVoice() override;

        //==============================================================================
        bool canPlaySound(SynthesiserSound*) override;

        void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int pitchWheel) override;
        void stopNote(float velocity, bool allowTailOff) override;

        void pitchWheelMoved(int newValue) override;
        void controllerMoved(int controllerNumber, int newValue) override;

        void setDetune(double newValue, double audioSampleRate);

        void renderNextBlock(AudioBuffer<float>&, int startSample, int numSamples) override;
        using SynthesiserVoice::renderNextBlock;

    private:
        //==============================================================================
        double pitchRatio = 0;
        double detuneRatio = 0;
        double sourceSamplePosition = 0;
        float lgain = 0, rgain = 0;

        ADSR adsr;

        JUCE_LEAK_DETECTOR(TwoShotVoice);
    };
}