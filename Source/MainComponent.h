#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;


    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    juce::AudioFormatManager formatManager;
    // note we need to tell it how large the cache is. 20 files are ok for now.
    juce::AudioThumbnailCache thumbnailCache{20};
    
    DJAudioPlayer player1{formatManager};
    DeckGUI deck1{&player1, formatManager, thumbnailCache};
    
    DJAudioPlayer player2{formatManager};
    DeckGUI deck2{&player2, formatManager, thumbnailCache};
    
    juce::MixerAudioSource mixerSource;
    PlaylistComponent playlist;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
