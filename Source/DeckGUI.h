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

private:
    
    DJAudioPlayer* djAudioPlayer;
    
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton loadButton;
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Slider speedSlider;
    WaveformDisplay waveformDisplay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
