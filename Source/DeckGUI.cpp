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
    playButtonImage = juce::ImageCache::getFromMemory (BinaryData::PlayButton_png, BinaryData::PlayButton_pngSize);
    pauseButtonImage = juce::ImageCache::getFromMemory (BinaryData::PauseButton_png, BinaryData::PauseButton_pngSize);
    loopButtonOffImage = juce::ImageCache::getFromMemory (BinaryData::LoopButton_png, BinaryData::LoopButton_pngSize);
    loopButtonOnImage = juce::ImageCache::getFromMemory (BinaryData::LoopButtonOn_png, BinaryData::LoopButtonOn_pngSize);
    
    playButton.setImages(false, true, true,
                         playButtonImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                         playButtonImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                         playButtonImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
    
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    
    pauseButton.setImages(false, true, true,
                         pauseButtonImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                         pauseButtonImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                         pauseButtonImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
    
    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);
    
    loopButton.setImages(false, true, true,
                         loopButtonOffImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                         loopButtonOffImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                         loopButtonOffImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
    
    addAndMakeVisible(loopButton);
    loopButton.addListener(this);
    
    addAndMakeVisible(volumeSlider);
    volumeSlider.addListener(this);
    volumeSlider.setRange(0, 1);
    volumeSlider.setValue(0.5);
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volumeSlider.setLookAndFeel(&sliderLookandFeel);
    volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0,0);
    
    addAndMakeVisible(loopButton);
    loopButton.addListener(this);
    
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
    g.fillAll(juce::Colours::black);   // clear the background
}

void DeckGUI::resized()
{
//    float rowH = getHeight() / 8;
    
    auto area = getLocalBounds();
    waveformDisplay.setBounds(area.removeFromTop(getHeight() * 0.35));
    positionSlider.setBounds(area.removeFromTop(getHeight() * 0.15));
    loadButton.setBounds(area.removeFromBottom(getHeight() * 0.15));
    speedSlider.setBounds(area.removeFromBottom(getHeight() * 0.15));
    volumeSlider.setBounds(area.removeFromRight(getWidth() / 4));
    playButton.setBounds(area.removeFromLeft(getWidth() / 4));
    pauseButton.setBounds(area.removeFromLeft(getWidth() / 4));
    loopButton.setBounds(area.removeFromRight(getWidth() / 4));
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        djAudioPlayer->setPosition(0);
        djAudioPlayer->play();
        //DBG("MainComponent::buttonClicked: playButton");
    }
    
    if (button == &pauseButton)
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
    
    if (button == &loopButton)
    {
        if (looping == false)
        {
            looping = true;
            loopButton.setImages(false, true, true,
                                 loopButtonOnImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                                 loopButtonOnImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                                 loopButtonOnImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
            
        }
        else
        {
            looping = false;
            loopButton.setImages(false, true, true,
                                 loopButtonOffImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                                 loopButtonOffImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                                 loopButtonOffImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
        }
        std::cout << "MainComponent::buttonClicked: loopButton: " << looping << std::endl;
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
    positionSlider.setValue(djAudioPlayer->getPositionRelative());
    if (looping == true && positionSlider.getValue() == 1.0)
    {
        positionSlider.setValue(0.0);
        djAudioPlayer->setPosition(0.0);
        djAudioPlayer->play();
    }
}

double DeckGUI::getGain()
{
    return volumeSlider.getValue();
}
