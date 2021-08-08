/*
  ==============================================================================

    RotarySliderLookandFeel.h
    Created: 4 Aug 2021 10:42:04am

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotarySliderLookandFeel  : public juce::Component,
                                 public juce::LookAndFeel_V4
{
public:
    RotarySliderLookandFeel();
    ~RotarySliderLookandFeel() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** virtual function required by LookAndFeel_V4*/
    void drawRotarySlider(juce::Graphics& g,
                          int x,
                          int y,
                          int width,
                          int height,
                          float sliderPos,
                          float rotaryStartAngle,
                          float rotaryEndAngle,
                          juce::Slider& slider) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotarySliderLookandFeel)
};
