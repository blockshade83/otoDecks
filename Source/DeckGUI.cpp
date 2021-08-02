/*
  ==============================================================================

    DeckGUI.cpp
    Created: 27 Jul 2021 10:57:00am

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _djAudioPlayer,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse)

: djAudioPlayer{_djAudioPlayer}, // asign djAudioPlayer to data member
  waveformDisplay(formatManagerToUse, cacheToUse) // call constructor on waveformDisplay

{
    
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    playButton.setButtonText("Play");
    
    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    stopButton.setButtonText("Stop");
    
    addAndMakeVisible(volumeSlider);
    volumeSlider.addListener(this);
    volumeSlider.setRange(0, 1);
    volumeSlider.setValue(0.5);
    
    addAndMakeVisible(positionSlider);
    positionSlider.addListener(this);
    positionSlider.setRange(0.0, 1.0);
    positionSlider.setValue(0.0);
    
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.0, 10.0);
    speedSlider.setValue(1.0);
    
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    loadButton.setButtonText("Load");
    
    addAndMakeVisible(waveformDisplay);
    
    startTimer(200);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void DeckGUI::resized()
{
    float rowH = getHeight() / 8;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    volumeSlider.setBounds(0, rowH*2, getWidth(), rowH);
    positionSlider.setBounds(0, rowH*3, getWidth(), rowH);
    speedSlider.setBounds(0, rowH*4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH*5, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH*7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        djAudioPlayer->setPosition(0);
        djAudioPlayer->play();
        //DBG("MainComponent::buttonClicked: playButton");
    }
    
    if (button == &stopButton)
    {
        djAudioPlayer->stop();
        //DBG("MainComponent::buttonClicked: stopButton");
    }
    
    if (button == &loadButton)
    {
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            juce::URL audioURL = juce::URL{chooser.getResult()};
            djAudioPlayer->loadURL(audioURL);
            waveformDisplay.loadURL(audioURL);
        }
        //DBG("MainComponent::buttonClicked: loadButton");
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        djAudioPlayer->setGain(volumeSlider.getValue());
    }
    
    if (slider == &positionSlider)
    {
        djAudioPlayer->setPositionRelative(positionSlider.getValue());
    }
    
    if (slider == &speedSlider)
    {
       djAudioPlayer->setSpeed(speedSlider.getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    //DBG("DeckGUI::isInterestedInFileDrag" );
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::URL fileURL = juce::URL{juce::File{files[0]}};
    djAudioPlayer->loadURL(fileURL);
    //DBG("DeckGUI::filesDropped" );
}

void DeckGUI::timerCallback()
{
    //DBG("DeckGUI::timerCallback" );
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
}
