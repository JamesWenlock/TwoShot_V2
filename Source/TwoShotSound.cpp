/*
==============================================================================

TwoShotSound.cpp
Created: 17 Jun 2022 3:26:17pm
Author:  Deuel Lab

==============================================================================
*/

#pragma

#include "TwoShotSound.h"

TwoShotSound::TwoShotSound(
    AudioBuffer<float> & buffer,
    double bufferSampleRate,
    const BigInteger& notes,
    int midiNoteForNormalPitch,
    double attackTimeSecs,
    double releaseTimeSecs,
    double maxSampleLengthSeconds)
    :
    sourceSampleRate(bufferSampleRate),
    midiNotes(notes),
    midiRootNote(midiNoteForNormalPitch)
{
    if (sourceSampleRate > 0 && buffer.getNumSamples() > 0)
    {
        length = jmin((int)buffer.getNumSamples(),
            (int)(maxSampleLengthSeconds * sourceSampleRate));

        data.reset(new AudioBuffer<float>(jmin(2, (int)buffer.getNumChannels()), length + 4));
        data.get()->clear();
        for (int i = 0; i < buffer.getNumChannels(); ++i)
        {
            data.get()->copyFrom(i, 0, buffer.getReadPointer(i), length);
        }

        params.attack = static_cast<float> (attackTimeSecs);
        params.release = static_cast<float> (releaseTimeSecs);
    }
}

TwoShotSound::TwoShotSound(
    AudioBuffer<float> & buffer,
    double bufferSampleRate,
    const BigInteger& notes,
    int midiNoteForNormalPitch,
    int startSample,
    int numSamples,
    int fadeLength,
    double attackTimeSecs,
    double releaseTimeSecs,
    double maxSampleLengthSeconds)
    :
    sourceSampleRate(bufferSampleRate),
    midiNotes(notes),
    midiRootNote(midiNoteForNormalPitch)
{
    if (sourceSampleRate > 0 && buffer.getNumSamples() > 0)
    {
        length = jmin((int)numSamples,
            (int)(maxSampleLengthSeconds * sourceSampleRate));
        data.reset(new AudioBuffer<float>(jmin(2, (int)buffer.getNumChannels()), length + 4));
        data.get()->clear();
        for (int i = 0; i < buffer.getNumChannels(); ++i)
        {
            data.get()->copyFrom(i, 0,  buffer, i, startSample, length);
        }
        data.get()->applyGainRamp(
            data.get()->getNumSamples() - (fadeLength + 4), 
            fadeLength, 
            1.0, 
            0
        );
        params.attack = static_cast<float> (attackTimeSecs);
        params.release = static_cast<float> (releaseTimeSecs);
    }
}

TwoShotSound::~TwoShotSound()
{
}

bool TwoShotSound::appliesToNote(int midiNoteNumber)
{
    return midiNotes[midiNoteNumber];
}

void TwoShotSound::setMidiNotes(BigInteger midiNotes, int midiRootNote)
{
    this->midiNotes = midiNotes;
    this->midiRootNote = midiRootNote;
}

bool TwoShotSound::appliesToChannel(int /*midiChannel*/)
{
    return true;
}
