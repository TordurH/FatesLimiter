/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class FatesLimiterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FatesLimiterAudioProcessorEditor (FatesLimiterAudioProcessor&);
    ~FatesLimiterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> dirtAttachment;

	juce::Slider sliderGain, sliderRelease;
	juce::ToggleButton buttonDirt;

	myLookAndFeel MyLookAndFeel;


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FatesLimiterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FatesLimiterAudioProcessorEditor)
};
