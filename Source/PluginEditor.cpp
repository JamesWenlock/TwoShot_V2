/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TwoShot_V2AudioProcessorEditor::TwoShot_V2AudioProcessorEditor (TwoShot_V2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(label);
}

TwoShot_V2AudioProcessorEditor::~TwoShot_V2AudioProcessorEditor()
{
}

//==============================================================================
void TwoShot_V2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    label.setText(juce::String(audioProcessor.getBPM()), juce::dontSendNotification);
    //g.fillAll (juce::Colours::black);
    //g.setColour(juce::Colours::white);
    //g.drawFittedText("Testing detune", getLocalBounds(), juce::Justification::centred, 1);
}

void TwoShot_V2AudioProcessorEditor::resized()
{
    label.setBounds(getLocalBounds());
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
