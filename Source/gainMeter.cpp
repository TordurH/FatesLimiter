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

gainMeter::gainMeter(std::function<float()>&& callback) : getGainValue(std::move(callback))
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

    const float level = getGainValue();

    int offsetX = 40;
	int offsetY = 50;

	juce::Rectangle<float> bounds = getLocalBounds().reduced(50).toFloat();

	g.setColour(juce::Colours::black);
    g.fillRect(bounds);

	g.setGradientFill(gradient);

	const float scaledY = juce::jmap(level, -60.0f, 6.0f, 0.0f,static_cast<float>(getHeight()));

	g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.0f);


   
}

void gainMeter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	juce::Rectangle<float> bounds = getLocalBounds().toFloat();

	gradient = juce::ColourGradient(juce::Colours::green, bounds.getBottomLeft(), juce::Colours::red, bounds.getTopLeft(), false);
	gradient.addColour(0.5, juce::Colours::yellow);
}
