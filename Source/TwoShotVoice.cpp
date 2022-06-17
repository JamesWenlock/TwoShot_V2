/*
  ==============================================================================

    TwoShotVoice.cpp
    Created: 17 Jun 2022 3:26:01pm
    Author:  Deuel Lab

  ==============================================================================
*/

#include "TwoShotVoice.h"
#include "TwoShotSound.h"

namespace juce
{
    TwoShotVoice::TwoShotVoice() {}
    TwoShotVoice::~TwoShotVoice() {}

    bool TwoShotVoice::canPlaySound(SynthesiserSound* sound)
    {
        return dynamic_cast<const TwoShotSound*> (sound) != nullptr;
    }

    void TwoShotVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* s, int /*currentPitchWheelPosition*/)
    {
        if (auto* sound = dynamic_cast<const TwoShotSound*> (s))
        {
            pitchRatio = std::pow(2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
                * sound->sourceSampleRate / getSampleRate();

            sourceSamplePosition = 0.0;
            lgain = velocity;
            lgain = velocity;
            rgain = velocity;

            adsr.setSampleRate(sound->sourceSampleRate);
            adsr.setParameters(sound->params);

            adsr.noteOn();
        }
        else
        {
            jassertfalse; // this object can only play TwoShotSounds!
        }
    }

    void TwoShotVoice::stopNote(float /*velocity*/, bool allowTailOff)
    {
        if (allowTailOff)
        {
            adsr.noteOff();
        }
        else
        {
            clearCurrentNote();
            adsr.reset();
        }
    }

    void TwoShotVoice::pitchWheelMoved(int /*newValue*/) {}
    void TwoShotVoice::controllerMoved(int /*controllerNumber*/, int /*newValue*/) {}

    //==============================================================================
    void TwoShotVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
    {
        if (auto* playingSound = static_cast<TwoShotSound*> (getCurrentlyPlayingSound().get()))
        {
            auto& data = *playingSound->data;
            const float* const inL = data.getReadPointer(0);
            const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : nullptr;

            float* outL = outputBuffer.getWritePointer(0, startSample);
            float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

            while (--numSamples >= 0)
            {
                auto pos = (int)sourceSamplePosition;
                auto alpha = (float)(sourceSamplePosition - pos);
                auto invAlpha = 1.0f - alpha;

                // just using a very simple linear interpolation here..
                float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
                float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
                    : l;

                auto envelopeValue = adsr.getNextSample();

                l *= lgain * envelopeValue;
                r *= rgain * envelopeValue;

                if (outR != nullptr)
                {
                    *outL++ += l;
                    *outR++ += r;
                }
                else
                {
                    *outL++ += (l + r) * 0.5f;
                }

                sourceSamplePosition += pitchRatio;

                if (sourceSamplePosition > playingSound->length)
                {
                    stopNote(0.0f, false);
                    break;
                }
            }
        }
    }
}