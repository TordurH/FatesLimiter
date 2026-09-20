/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 30 Aug 2026 1:26:27am
    Author:  Bboy_

  ==============================================================================
*/

#include "LookAndFeel.h"

myLookAndFeel::myLookAndFeel()
{
    knobSequence = juce::ImageCache::getFromMemory(BinaryData::knobStrip_png, BinaryData::knobStrip_pngSize);
    areaToDraw = juce::Rectangle<float>(0, 0, 520, 520);

	imgButtonOff = juce::ImageCache::getFromMemory(BinaryData::ButtonOff_png, BinaryData::ButtonOff_pngSize);
	imgButtonOn = juce::ImageCache::getFromMemory(BinaryData::ButtonOn_png, BinaryData::ButtonOn_pngSize);
}

void myLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	juce::Rectangle<float> bounds = slider.getLocalBounds().toFloat();
    //auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

	int areaSize = std::min(bounds.getWidth(), bounds.getHeight());

    int imgSize = knobSequence.getWidth();

	int numFrames = knobSequence.getHeight() / imgSize; // Assuming each frame is 128 pixels high

	float frameIndex = std::floor(sliderPosProportional * (numFrames-1)); // Assuming each frame is 128 pixels high


	int offset = frameIndex * imgSize;


    if (knobSequence.isValid())
    {
		//int numFrames = 100; // Assuming there are 100 frames in the knob sequence
        //int frameIndex = juce::jlimit(0, numFrames - 1, (int)(sliderPosProportional * (float)(numFrames - 1)));
		
        g.drawImage(knobSequence, 0,0,areaSize, areaSize, 0, 0 + offset, imgSize, imgSize);
    }
}  


void myLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
	

	//Draw the button here, you can use the shouldDrawButtonAsHighlighted and shouldDrawButtonAsDown parameters to determine the button's state and draw accordingly.

	areaToDraw = button.getLocalBounds().toFloat();

    if(button.getToggleState())
    {
		g.drawImage(imgButtonOn, 0, 0, areaToDraw.getWidth(), areaToDraw.getHeight(), 0, 0, imgButtonOn.getWidth(), imgButtonOn.getHeight());
    }
    else
    {
        // Draw the button in the "off" state
        g.drawImage(imgButtonOff, 0, 0, areaToDraw.getWidth(), areaToDraw.getHeight(), 0, 0, imgButtonOff.getWidth(), imgButtonOff.getHeight());
    }

}
