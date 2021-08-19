/*
  ==============================================================================

    DeckControl.h
    Created: 6 Aug 2021 9:04:21am
    Author:  Ionut Costache

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "RotarySliderLookandFeel.h"

//==============================================================================
/*
*/
class DeckControl  : public juce::Component,
                     public juce::Slider::Listener,
                     public juce::Button::Listener
{
public:
    DeckControl();
    
    ~DeckControl() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // functions to manage combined sound from the 2 music players
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    
    /** virtual function from Slider::Listener class */
    void sliderValueChanged (juce::Slider *slider) override;
    
    /** virtual function from Button::Listener class */
    void buttonClicked(juce::Button* button) override;
    
    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};
    
    DeckGUI deck1{&player1, formatManager, thumbnailCache};
    DeckGUI deck2{&player2, formatManager, thumbnailCache};
    
private:
    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{100}; // note we need to tell it how large the cache is.
    
    // slider which changes the volume/focus on each player simultaneously
    juce::Slider volLeftRightSlider;
    
    // mixer source to combine sound from both audio players
    juce::MixerAudioSource mixerSource;
    
    // buttons to set up cue points to which we can jump
    juce::TextButton setCueButton1;
    juce::TextButton cuePlayButton1;
    juce::TextButton clearCueButton1;
    juce::TextButton setCueButton2;
    juce::TextButton cuePlayButton2;
    juce::TextButton clearCueButton2;
    
    // store position of each cue, initialize to -1
    double cueStartPosition1 = -1.0;
    double cueStartPosition2 = -1.0;
    
    // button to play both songs at once starting from the specific positions recorded
    juce::TextButton playBothCuesButton;
    
    // look and feel to change volume sliders to rotary sliders
    RotarySliderLookandFeel sliderLookandFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckControl)
};
