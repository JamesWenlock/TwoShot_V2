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
class TwoShot_V2AudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                        public juce::Slider::Listener,
                                        public juce::Button::Listener
{
public:
    TwoShot_V2AudioProcessorEditor (TwoShot_V2AudioProcessor&);
    ~TwoShot_V2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button* button) override;
    void buttonStateChanged(Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TwoShot_V2AudioProcessor& audioProcessor;
    juce::Label label;
    juce::Slider slider;
    juce::ToggleButton m_reverseButton;
    juce::ToggleButton m_modeButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TwoShot_V2AudioProcessorEditor)
};
