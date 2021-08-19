/*
  ==============================================================================

    RotarySliderLookandFeel.cpp
    Created: 4 Aug 2021 10:42:04am
    Documentation used: https://docs.juce.com/master/tutorial_look_and_feel_customisation.html

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotarySliderLookandFeel.h"

//==============================================================================
RotarySliderLookandFeel::RotarySliderLookandFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

RotarySliderLookandFeel::~RotarySliderLookandFeel()
{
}

void RotarySliderLookandFeel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("VOL", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void RotarySliderLookandFeel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void RotarySliderLookandFeel::drawRotarySlider(juce::Graphics& g,
                      int x,
                      int y,
                      int width,
                      int height,
                      float sliderPosition,
                      float startingAngle,
                      float endingAngle,
                      juce::Slider& slider)
{
    // get the maximum radius of a circle that could fit in the rectangle area
    float radius = juce::jmin(height, width) / 2 - 4.0f;
    
    // get the coordinates of the centre of the area
    float centreX = y + width / 2;
    float centreY = y + height / 2;
    
    // get the left X coordinate of a circle centered around the X,Y point determined earlier
    float rx = centreX - radius;
    // get the top Y coordinate of a circle centered around the X,Y point determined earlier
    float ry = centreY - radius;
    
    // calculate angle dependening of the starting angle and how much the slider has been moved
    float angle = startingAngle + sliderPosition * (endingAngle - startingAngle);
    
    // define the smallest rectangle which can include the circle
    juce::Rectangle<float> buttonArea (rx, ry, radius * 2, radius * 2);
    
    g.setColour(juce::Colours::white);
    
    // set coordinates for the volume indicator and draw it
    juce::Path volumeIndicator;
    volumeIndicator.addRectangle(0, -radius, 5.0f, radius * 0.3);
    g.fillPath(volumeIndicator, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    
    g.setColour(juce::Colours::white);
    g.drawEllipse(rx, ry, radius * 2, radius * 2, 3.0f);
}
