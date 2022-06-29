/*
  ==============================================================================

    TwoShotSynth.h
    Created: 29 Jun 2022 11:51:36am
    Author:  Deuel Lab

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class TwoShotSynth : public juce::Synthesizer
{
    public
    {
        startVoice(
            SynthesiserVoice* const voice,
            SynthesiserSound* const sound,
            const int midiChannel,
            const int midiNoteNumber,
            const float velocity
        ) override;
    };
};