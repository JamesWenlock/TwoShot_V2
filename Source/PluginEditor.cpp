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
    addAndMakeVisible(slider);
    addAndMakeVisible(m_reverseButton);
    addAndMakeVisible(m_modeButton);
    slider.addListener(this);
    m_reverseButton.addListener(this);    
    m_modeButton.addListener(this);
    slider.setRange(-1200.0, 1200);
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    m_reverseButton.setButtonText("reverse");
    m_modeButton.setButtonText("mode");
}

TwoShot_V2AudioProcessorEditor::~TwoShot_V2AudioProcessorEditor()
{
}

void TwoShot_V2AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    audioProcessor.m_sampler.setDetune(slider->getValue());
}

void TwoShot_V2AudioProcessorEditor::buttonClicked(Button* button)
{
    if (std::addressof(m_reverseButton) == button)
    {
        audioProcessor.m_sampler.setReverse(button->getToggleState());
    }

    if (std::addressof(m_modeButton) == button)
    {
        File* sample = new File(
            File::getSpecialLocation(File::userDesktopDirectory)
            .getChildFile("Samples")
            .getChildFile("112loop.wav")
        );
        std::unique_ptr<AudioFormatReader> fileReader(audioProcessor.m_formatManager.createReaderFor(*sample));
        if (m_modeButton.getToggleState())
        {
            audioProcessor.m_sampler.setAudio(*fileReader, fileReader->sampleRate, 112, 0);

        }
        else
        {
            audioProcessor.m_sampler.setAudio(*fileReader, fileReader->sampleRate, std::nullopt, 0);

        }
    }
}

void TwoShot_V2AudioProcessorEditor::buttonStateChanged(Button* button)
{

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
    Rectangle<int> modeBounds = bounds.removeFromBottom(100);
    Rectangle<int> reverseBounds = bounds.removeFromBottom(100);

    //label.setBounds(getLocalBounds());
    slider.setBounds(bounds);
    m_modeButton.setBounds(modeBounds);
    m_reverseButton.setBounds(reverseBounds);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
