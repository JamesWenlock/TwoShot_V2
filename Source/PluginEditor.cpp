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
    //addAndMakeVisible(label);
    addAndMakeVisible(slider);
    slider.addListener(this);
    slider.setRange(0.01, 2.0);
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
}

TwoShot_V2AudioProcessorEditor::~TwoShot_V2AudioProcessorEditor()
{
}

void TwoShot_V2AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    label.setText(juce::String(slider->getValue()), juce::dontSendNotification);
    audioProcessor.m_sampler.setDecay(slider->getValue());
}

//==============================================================================
void TwoShot_V2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //label.setText(juce::String(audioProcessor.getBPM()), juce::dontSendNotification);
    //g.fillAll (juce::Colours::black);
    //g.setColour(juce::Colours::white);
    //g.drawFittedText("Testing detune", getLocalBounds(), juce::Justification::centred, 1);
}

void TwoShot_V2AudioProcessorEditor::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    Rectangle<int> labelBounds = bounds.removeFromBottom(100);

    //label.setBounds(getLocalBounds());
    slider.setBounds(bounds);
    label.setBounds(labelBounds);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
