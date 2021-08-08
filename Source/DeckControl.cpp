/*
  ==============================================================================

    DeckControl.cpp
    Created: 6 Aug 2021 9:04:21am
    Author:  Ionut Costache

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckControl.h"

//==============================================================================
DeckControl::DeckControl()

{
    header.setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
            header.setButtonText ("Header");
            addAndMakeVisible (header);
     
    footer.setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
    footer.setButtonText ("Footer");
    addAndMakeVisible (footer);

    sidebar.setColour (juce::TextButton::buttonColourId, juce::Colours::black);
    sidebar.setButtonText ("Sidebar");
    addAndMakeVisible (sidebar);
    
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    
    volLeftRightSlider.setRange(-1.0f, 1.0f);
    volLeftRightSlider.setValue(0.0f);
    volLeftRightSlider.addListener(this);
    volLeftRightSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volLeftRightSlider.setLookAndFeel(&sliderLookandFeel);
    volLeftRightSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0,0);
    addAndMakeVisible(volLeftRightSlider);
    
//    playButtonImage = juce::ImageCache::getFromMemory (BinaryData::PlayButton_jpeg, BinaryData::PlayButton_jpegSize);
    
//    button1.setImages(true, true, true,
//             playButtonImage, 0.5, juce::Colour::fromHSV (0.5f,    // hue
//                                                        0.5f,    // saturation
//                                                        0.5f,    // brightness
//                                                        0.5f),   // alpha,
//             playButtonImage, 0,juce::Colour(255,0,0),
//             playButtonImage, 0, juce::Colour(100,0,0));
//
//    addAndMakeVisible(button1);
//    addAndMakeVisible(button2);
    
    formatManager.registerBasicFormats();
}

DeckControl::~DeckControl()
{
}

void DeckControl::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);   // clear the background

    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckControl", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DeckControl::resized()
{
    auto area = getLocalBounds();
     
    auto headerFooterHeight = 36;
    header.setBounds(area.removeFromTop(headerFooterHeight));
    footer.setBounds(area.removeFromBottom(headerFooterHeight));

    auto sidebarWidth = 80;
    sidebar.setBounds (area.removeFromRight(sidebarWidth));

    deck1.setBounds(area.removeFromLeft(300));
    deck2.setBounds(area.removeFromRight(300));
    
    volLeftRightSlider.setBounds(area.removeFromTop(100));
//    button1.setBounds(area.removeFromTop(100));
//    button2.setBounds(area.removeFromTop(100));
}

//==============================================================================
void DeckControl::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DeckControl::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void DeckControl::releaseResources()
{
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

void DeckControl::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volLeftRightSlider)
    {
        std::cout << "Slider value: " << slider->getValue() << std::endl;
        double sliderValue = volLeftRightSlider.getValue();
        if (sliderValue > 0)
        {
            player2.setGain(juce::jmin(deck2.getGain() * (1 + sliderValue / 2), 1.5));
            std::cout << "Player 2 gain: " << juce::jmin(deck2.getGain() * (1 + sliderValue / 2), 1.5) << std::endl;
            player1.setGain(juce::jmax(deck1.getGain() * (1 - sliderValue / 2), 0.0));
            std::cout << "Player 1 gain: " << juce::jmax(deck1.getGain() * (1 - sliderValue / 2), 0.0) << std::endl;
        }
        if (sliderValue < 0)
        {
            player1.setGain(juce::jmin(deck1.getGain() * (1 - sliderValue / 2), 1.5));
            std::cout << "Player 1 gain: " << juce::jmin(deck1.getGain() * (1 - sliderValue / 2), 1.5) << std::endl;
            player2.setGain(juce::jmax(deck2.getGain() * (1 + sliderValue / 2), 0.0));
            std::cout << "Player 2 gain: " << juce::jmax(deck2.getGain() * (1 + sliderValue / 2), 0.0) << std::endl;
        }
    }
}
