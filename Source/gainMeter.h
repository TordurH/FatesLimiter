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
    gainMeter(std::function<float()>&& callback);
    ~gainMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;
	void timerCallback() override;

private:

    //float gainValue{}; // Variable to store the gain value

	std::function<float()> getGainValue; // Callback function for gain changes

    juce::ColourGradient gradient;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (gainMeter)
};
