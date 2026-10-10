/*
  ==============================================================================

    gainMeter.cpp
    Created: 3 Sep 2026 7:50:37pm
    Author:  Bboy_

  ==============================================================================
*/

#include <JuceHeader.h>
#include "gainMeter.h"

//==============================================================================
gainMeter::gainMeter()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

gainMeter::gainMeter(std::function<std::array<juce::LinearSmoothedValue<float>, 2>()>&& callback) : getGainValue(std::move(callback))
{
   
	startTimerHz(30); // Start the timer with a frequency of 30 Hz
}

gainMeter::~gainMeter()
{
}

void gainMeter::timerCallback()
{
    
    repaint();
}

void gainMeter::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    std::array<juce::LinearSmoothedValue<float>, 2> gainValues = getGainValue();

    const float levelL = juce::Decibels::gainToDecibels(gainValues[0].getCurrentValue());
	const float levelR = juce::Decibels::gainToDecibels(gainValues[1].getCurrentValue());

    int offsetX = 40;
	int offsetY = 50;

	juce::Rectangle<float> bounds = getLocalBounds().reduced(50).toFloat();

	g.setColour(juce::Colours::black);
    g.fillRect(bounds);

	juce::Rectangle<float>  leftMeter = bounds.removeFromLeft(bounds.getWidth() / 2.0f);

	g.setGradientFill(gradient);

	const float scaledYLeft = juce::jmap(levelL, -60.0f, 6.0f, 0.0f,static_cast<float>(getHeight()));
	const float scaledYRight = juce::jmap(levelR, -60.0f, 6.0f, 0.0f,static_cast<float>(getHeight()));

	DBG("Level L: " << levelL << " dB, Level R: " << levelR << " dB");

	g.fillRoundedRectangle(leftMeter.removeFromBottom(scaledYLeft), 5.0f);
	g.fillRoundedRectangle(bounds.removeFromBottom(scaledYRight), 5.0f);


   
}

void gainMeter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	juce::Rectangle<float> bounds = getLocalBounds().toFloat();

	gradient = juce::ColourGradient(juce::Colours::green, bounds.getBottomLeft(), juce::Colours::red, bounds.getTopLeft(), false);
	gradient.addColour(0.5, juce::Colours::yellow);
}
