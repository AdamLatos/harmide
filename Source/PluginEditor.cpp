/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HarmideAudioProcessorEditor::HarmideAudioProcessorEditor (HarmideAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);

    // define parameters of gain object
    pluginVolumeSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    pluginVolumeSlider.setRange(0.0, 1.0, 0.0);
    pluginVolumeSlider.setValue(0.2);
    pluginVolumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    pluginVolumeSlider.setPopupDisplayEnabled(true, false, this);
    pluginVolumeSlider.setTextValueSuffix(" Gain");

    addAndMakeVisible(&pluginVolumeSlider);

    pluginVolumeSlider.addListener(this);
}

HarmideAudioProcessorEditor::~HarmideAudioProcessorEditor()
{
}

//==============================================================================
void HarmideAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::darkslateblue);
    g.setColour (juce::Colours::palegreen);
    g.setFont (juce::Font(15.0, juce::Font::bold));
    g.drawFittedText("Midi Volume", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void HarmideAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    pluginVolumeSlider.setBounds(40, 30, 20, getHeight() - 60);
}

void HarmideAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &pluginVolumeSlider)
        audioProcessor.pluginVolume = pluginVolumeSlider.getValue();
}
