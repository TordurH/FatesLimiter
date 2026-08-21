/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FatesLimiterAudioProcessorEditor::FatesLimiterAudioProcessorEditor (FatesLimiterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    auto sliderHelper = [&](juce::Slider &slider){
        
        slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
		addAndMakeVisible(slider);

		};

	buttonDirt.setButtonText("Dirt");
	addAndMakeVisible(buttonDirt);

	dirtAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "dirt", buttonDirt);
	gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gain", sliderGain);
	releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "release", sliderRelease);

	sliderHelper(sliderGain);
	sliderHelper(sliderRelease);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

FatesLimiterAudioProcessorEditor::~FatesLimiterAudioProcessorEditor()
{
}

//==============================================================================
void FatesLimiterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void FatesLimiterAudioProcessorEditor::resized()
{

	sliderGain.setBounds(50, 50, 100, 100);
	sliderRelease.setBounds(250, 50, 100, 100);
	buttonDirt.setBounds(150, 200, 100, 30);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
