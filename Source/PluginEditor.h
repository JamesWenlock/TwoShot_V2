/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TwoShot_V2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TwoShot_V2AudioProcessorEditor (TwoShot_V2AudioProcessor&);
    ~TwoShot_V2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TwoShot_V2AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoShot_V2AudioProcessorEditor)
};
