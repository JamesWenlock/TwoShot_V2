/*
  ==============================================================================

    TwoShotSynth.cpp
    Created: 17 Jun 2022 3:29:38pm
    Author:  Deuel Lab

  ==============================================================================
*/

#include "TwoShotSynth.h"
#include <JuceHeader.h>
#include "Synth.h"
#include "TwoShotSamplerSound.h"

#pragma once
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

Synth::Synth()
{
}

Synth::~Synth()
{

}

/**
* Updates the audio for the Synth
* @param audioBpm if this value is present, then this is a polyphonic Loop, and the Synth goes into LOOP MODE
*/
void Synth::setAudio(
    juce::AudioBuffer<float> && audioBuffer,
    const double audioSampleRate,
    std::optional<const double> audioBpm,
    const size_t sampleProgress
)
{
}

/**
* This is called when the host updates it's sampleRate
*/
void Synth::setHostSampleRate(const double currentSampleRate)
{
}

/**
* This is called when the user clicks the reverse toggle in the UI
*/
void Synth::setReverse(const bool isReversed)
{
}

/**
* changes the Synth attack of the envelope
*/
void Synth::setAttack(const double attackSeconds)
{
}

/**
* changes the Synth decay of the envelope
*/
void Synth::setDecay(const double decaySeconds)
{
}

/**
* This is called when the user changes the pitch, from within the UI
*/
void Synth::setDetune(const double detuneAmount)
{
}

/**
* This is called from the JUCE process block method
*/
void Synth::processNextBlock(
    juce::AudioBuffer<float>& outputAudio,
    const juce::MidiBuffer& midiData,
    std::optional<const double> currentHostBpm
)
{
}