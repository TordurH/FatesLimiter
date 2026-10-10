/*
  ==============================================================================

    gainMeter.h
    Created: 3 Sep 2026 7:50:37pm
    Author:  Bboy_

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>

//==============================================================================
/*
*/
class gainMeter  : public juce::Component, public juce::Timer
{
public:
    gainMeter();
    gainMeter(std::function<std::array<juce::LinearSmoothedValue<float>, 2>()>&& callback);
    ~gainMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;
	void timerCallback() override;

private:

	
	std::function<std::array<juce::LinearSmoothedValue<float>, 2>()> getGainValue; // Callback function for gain changes

    juce::ColourGradient gradient;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (gainMeter)
};
