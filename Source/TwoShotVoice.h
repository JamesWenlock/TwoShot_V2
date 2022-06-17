/*
  ==============================================================================

    TwoShotVoice.h
    Created: 17 Jun 2022 3:26:01pm
    Author:  Deuel Lab

  ==============================================================================
*/

#pragma once
//==============================================================================
/**
    A subclass of SynthesiserVoice that can play a SamplerSound.
    To use it, create a Synthesiser, add some SamplerVoice objects to it, then
    give it some SampledSound objects to play.
    @see SamplerSound, Synthesiser, SynthesiserVoice
    @tags{Audio}
*/
class JUCE_API  SamplerVoice    : public SynthesiserVoice
{
public:
    //==============================================================================
    /** Creates a SamplerVoice. */
    SamplerVoice();

    /** Destructor. */
    ~SamplerVoice() override;

    //==============================================================================
    bool canPlaySound (SynthesiserSound*) override;

    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int pitchWheel) override;
    void stopNote (float velocity, bool allowTailOff) override;

    void pitchWheelMoved (int newValue) override;
    void controllerMoved (int controllerNumber, int newValue) override;

    void renderNextBlock (AudioBuffer<float>&, int startSample, int numSamples) override;
    using SynthesiserVoice::renderNextBlock;

private:
    //==============================================================================
    double pitchRatio = 0;
    double sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;

    ADSR adsr;

    JUCE_LEAK_DETECTOR (SamplerVoice)
};