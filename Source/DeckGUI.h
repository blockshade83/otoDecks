/*
  ==============================================================================

    DeckGUI.h
    Created: 27 Jul 2021 10:57:00am

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "RotarySliderLookandFeel.h"
#include "Track.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer

{
public:
    DeckGUI(DJAudioPlayer* _djAudioPlayer,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** virtual function from Button::Listener class */
    void buttonClicked(juce::Button* button) override;
    
    /** virtual function from Slider::Listener class */
    void sliderValueChanged (juce::Slider *slider) override;
    
    /** virtual functions from FileDragAndDropTarget class */
    virtual bool isInterestedInFileDrag (const juce::StringArray& files) override;
    virtual void filesDropped (const juce::StringArray& files, int x, int y) override;
    
    /** virtual function from Timer class */
    void timerCallback() override;
    
    /** get current volume of the volume slider */
    double getGain();
    
    // variable for controlling volume
    juce::Slider volumeSlider;
    
    DJAudioPlayer* djAudioPlayer;
    WaveformDisplay waveformDisplay;
    
    // variable to display which track is currently playing
    juce::TextEditor nowPlaying;

private:
    
    // images and buttons for play, pause, stop, loop
    juce::Image playButtonImage;
    juce::ImageButton playButton;
    juce::Image pauseButtonImage;
    juce::ImageButton pauseButton;
    juce::Image stopButtonImage;
    juce::ImageButton stopButton;
    juce::Image loopButtonOffImage;
    juce::Image loopButtonOnImage;
    juce::ImageButton loopButton;
    
    // variable to store whether loop is active or not
    bool looping = false;
    
    // slider variables to control position and speed
    juce::Slider positionSlider;
    juce::Slider speedSlider;
    
    // textbox to display the position in seconds instead of as a %
    juce::TextEditor positionSliderText;
    
    /** function to convert slider value from 0-1 range to seconds, considering a given track length */
    juce::String convertToDuration(double sliderValue, double trackLength);
 
    // look and feel to change volume slider to rotary slider
    RotarySliderLookandFeel sliderLookandFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
