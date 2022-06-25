/*
==============================================================================

TwoShotVoice.h
Created: 17 Jun 2022 3:26:01pm
Author:  Deuel Lab

==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <ea_soundtouch/ea_soundtouch.h>

//==============================================================================
/**
A subclass of SynthesiserVoice that can play a SamplerSound.
To use it, create a Synthesiser, add some TwoShotVoice objects to it, then
give it some SampledSound objects to play.
@see SamplerSound, Synthesiser, SynthesiserVoice
@tags{Audio}
*/


class TwoShotVoice : public SynthesiserVoice
{
public:
    //==============================================================================
    /** Creates a TwoShotVoice. */
    TwoShotVoice();

    /** Destructor. */
    ~TwoShotVoice() override;

    //==============================================================================

    void setSampleRate(uint sampleRate);
    void setBlockSize(uint blockSize);

    bool canPlaySound(SynthesiserSound*) override;

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int pitchWheel) override;
    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;

    void setDetune(double newValue);
    void setBPMComp(double audioBPM, double hostBPM);
    void setIsLoop(bool newValue);

    void renderNextBlock(AudioBuffer<float>&, int startSample, int numSamples) override;
    using SynthesiserVoice::renderNextBlock;
    std::vector<float> m_buf;

private:
    //==============================================================================
    double pitchRatio = 0;
    double detuneRatio = 1;
    double  bpmCompRatio = 1;
    double sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;
    bool isLoop = false;
    soundtouch::SoundTouch soundTouch;

    ADSR adsr;

    JUCE_LEAK_DETECTOR(TwoShotVoice);
};
