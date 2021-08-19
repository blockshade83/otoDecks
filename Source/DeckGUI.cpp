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
    // images for play, pause, stop, loop buttons
    playButtonImage = juce::ImageCache::getFromMemory (BinaryData::PlayButton_png, BinaryData::PlayButton_pngSize);
    pauseButtonImage = juce::ImageCache::getFromMemory (BinaryData::PauseButton_png, BinaryData::PauseButton_pngSize);
    stopButtonImage = juce::ImageCache::getFromMemory (BinaryData::StopButton_png, BinaryData::StopButton_pngSize);
    loopButtonOffImage = juce::ImageCache::getFromMemory (BinaryData::LoopButton_png, BinaryData::LoopButton_pngSize);
    loopButtonOnImage = juce::ImageCache::getFromMemory (BinaryData::LoopButtonOn_png, BinaryData::LoopButtonOn_pngSize);
    
    // set up play button
    playButton.setImages(false, true, true,
                         playButtonImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                         playButtonImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                         playButtonImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    
    // set up pause button
    pauseButton.setImages(false, true, true,
                         pauseButtonImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                         pauseButtonImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                         pauseButtonImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);
    
    // set up stop button
    stopButton.setImages(false, true, true,
                         stopButtonImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                         stopButtonImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                         stopButtonImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    
    // set up loop button (off position default)
    loopButton.setImages(false, true, true,
                         loopButtonOffImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                         loopButtonOffImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                         loopButtonOffImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
    addAndMakeVisible(loopButton);
    loopButton.addListener(this);
    
    // set up volume slider
    addAndMakeVisible(volumeSlider);
    volumeSlider.addListener(this);
    volumeSlider.setRange(0, 1);
    volumeSlider.setValue(0.5);
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volumeSlider.setLookAndFeel(&sliderLookandFeel);
    volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0,0);
    volumeSlider.setMouseDragSensitivity(75);
    
    // set up position slider
    addAndMakeVisible(positionSlider);
    positionSlider.addListener(this);
    positionSlider.setRange(0.0, 1.0);
    positionSlider.setValue(0.0);
    positionSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0,0);
    
    // set up text box to display position in minutes::seconds
    addAndMakeVisible(positionSliderText);
    
    // set up speed slider
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.0, 10.0);
    speedSlider.setValue(1.0);

    // set up waveformDisplay
    addAndMakeVisible(waveformDisplay);
    
    // set up Now Playing text box
    addAndMakeVisible(nowPlaying);
    nowPlaying.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    nowPlaying.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    nowPlaying.setReadOnly(true);
    
    // start time to use for filtering playlist
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
    auto area = getLocalBounds();
    
    // reserve space for now Playing and waveform display at the top
    nowPlaying.setBounds(area.removeFromTop(getHeight() * 0.12));
    waveformDisplay.setBounds(area.removeFromTop(getHeight() * 0.40));
    
    // reserve space for the slider and text box
    auto sliderArea = area.removeFromTop(getHeight() * 0.08);
    positionSliderText.setBounds(sliderArea.removeFromLeft(getWidth() * 0.15));
    positionSlider.setBounds(sliderArea.removeFromLeft(getWidth() * 0.85));
    positionSliderText.setReadOnly(true);
    
    // reserve space for speed slider at the bottom
    speedSlider.setBounds(area.removeFromBottom(getHeight() * 0.15));
    
    // buttons and volume control go in the remaining space, split into 5 horizontal boxes
    playButton.setBounds(area.removeFromLeft(getWidth() / 5));
    pauseButton.setBounds(area.removeFromLeft(getWidth() / 5));
    stopButton.setBounds(area.removeFromLeft(getWidth() / 5));
    loopButton.setBounds(area.removeFromLeft(getWidth() / 5));
    volumeSlider.setBounds(area.removeFromLeft(getWidth() / 5));
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        djAudioPlayer->play();
    }
    
    if (button == &pauseButton)
    {
        // we stop the player but without changing position
        djAudioPlayer->stop();
    }
    
    if (button == &stopButton)
    {
        // we stop the player and we set position to 0
        djAudioPlayer->setPosition(0);
        djAudioPlayer->stop();
    }
    
    if (button == &loopButton)
    {
        if (looping == false) // check if looping is activated
        {
            looping = true; // if not, we "activate" looping
            // change button image to give visual feedback that selection in On
            loopButton.setImages(false, true, true,
                                 loopButtonOnImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                                 loopButtonOnImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                                 loopButtonOnImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
        }
        else
        {
            looping = false; // looping was active and we clicked to deactivate
            // change button image to default give visual feedback that selection in Off
            loopButton.setImages(false, true, true,
                                 loopButtonOffImage, 1, juce::Colour::fromRGBA(0,0,0,0),
                                 loopButtonOffImage, 0.5, juce::Colour::fromRGBA(0,0,0,0),
                                 loopButtonOffImage, 0.75, juce::Colour::fromRGBA(0,0,0,0));
        }
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
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::URL fileURL = juce::URL{juce::File{files[0]}};
    djAudioPlayer->loadURL(fileURL);
    nowPlaying.setText(fileURL.getFileName());
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
    positionSlider.setValue(djAudioPlayer->getPositionRelative());
    positionSliderText.setText(convertToDuration(positionSlider.getValue(), djAudioPlayer->getDuration() ));
    
    // check if looping is active and we have reached the end of the track
    // if yes, we start the track from the beginning
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

juce::String DeckGUI::convertToDuration(double sliderValue, double trackLength)
{
    if (trackLength == 0.0)
    {
        return "0:00";
    }
    
    // extract minutes and seconds from the track length position in seconds
    int durationMinutes = int(sliderValue * trackLength) / 60;
    int durationSeconds = int(sliderValue * trackLength) % 60;
    
    if (durationSeconds < 10)
    {
        // add a 0 if less than 10 seconds so that we can format as "5:09" instead of "5:9"
        return std::to_string(durationMinutes) + ":0" + std::to_string(durationSeconds);
    }
    else
    {
        return std::to_string(durationMinutes) + ":" + std::to_string(durationSeconds);
    }
}
