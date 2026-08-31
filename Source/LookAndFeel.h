/*
  ==============================================================================

    LookAndFeel.h
    Created: 30 Aug 2026 1:26:27am
    Author:  Bboy_

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

class myLookAndFeel : public juce::LookAndFeel_V4
{

public:

    myLookAndFeel();

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;
	void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    juce::Image knobSequence;
    juce::Rectangle<float> areaToDraw;
};