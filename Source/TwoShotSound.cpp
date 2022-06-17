/*
  ==============================================================================

    TwoShotSound.cpp
    Created: 17 Jun 2022 3:26:17pm
    Author:  Deuel Lab

  ==============================================================================
*/

#pragma

#include "TwoShotSound.h"

namespace juce
{
    TwoShotSound::TwoShotSound(const String& soundName,
        AudioFormatReader& source,
        const BigInteger& notes,
        int midiNoteForNormalPitch,
        double attackTimeSecs,
        double releaseTimeSecs,
        double maxSampleLengthSeconds)
        : name(soundName),
        sourceSampleRate(source.sampleRate),
        midiNotes(notes),
        midiRootNote(midiNoteForNormalPitch)
    {
        if (sourceSampleRate > 0 && source.lengthInSamples > 0)
        {
            length = jmin((int)source.lengthInSamples,
                (int)(maxSampleLengthSeconds * sourceSampleRate));

            data.reset(new AudioBuffer<float>(jmin(2, (int)source.numChannels), length + 4));

            source.read(data.get(), 0, length + 4, 0, true, true);

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

    bool TwoShotSound::appliesToChannel(int /*midiChannel*/)
    {
        return true;
    }
}