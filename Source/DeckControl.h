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
                     public juce::Slider::Listener
{
public:
    DeckControl();
    
    ~DeckControl() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    
    /** virtual function from Slider::Listener class */
    void sliderValueChanged (juce::Slider *slider) override;
    
private:
    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{20}; // note we need to tell it how large the cache is. 20 files are ok for now.
    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};
    
    DeckGUI deck1{&player1, formatManager, thumbnailCache};
    DeckGUI deck2{&player2, formatManager, thumbnailCache};
    
    juce::TextButton header;
    juce::TextButton sidebar;
    juce::TextButton footer;
    juce::Slider volLeftRightSlider;
    juce::MixerAudioSource mixerSource;
    
    RotarySliderLookandFeel sliderLookandFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckControl)
};
