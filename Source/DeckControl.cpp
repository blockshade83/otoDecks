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
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    
    // set properties for volLeftRightSlider
    volLeftRightSlider.setRange(-1.0f, 1.0f);
    volLeftRightSlider.setValue(0.0f);
    volLeftRightSlider.addListener(this);
    volLeftRightSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volLeftRightSlider.setLookAndFeel(&sliderLookandFeel);
    volLeftRightSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0,0);
    volLeftRightSlider.setMouseDragSensitivity(75);
    addAndMakeVisible(volLeftRightSlider);
    
    // set up cue buttons
    setCueButton1.addListener(this);
    setCueButton1.setButtonText("Set Cue 1");
    setCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    setCueButton1.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(setCueButton1);
    
    cuePlayButton1.addListener(this);
    cuePlayButton1.setButtonText("Play Cue 1");
    cuePlayButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    addAndMakeVisible(cuePlayButton1);
    
    clearCueButton1.addListener(this);
    clearCueButton1.setButtonText("Clear Cue 1");
    clearCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    addAndMakeVisible(clearCueButton1);
    
    setCueButton2.addListener(this);
    setCueButton2.setButtonText("Set Cue 2");
    setCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    setCueButton2.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(setCueButton2);
    
    cuePlayButton2.addListener(this);
    cuePlayButton2.setButtonText("Play Cue 2");
    cuePlayButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    addAndMakeVisible(cuePlayButton2);
    
    clearCueButton2.addListener(this);
    clearCueButton2.setButtonText("Clear Cue 2");
    clearCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    addAndMakeVisible(clearCueButton2);
    
    playBothCuesButton.addListener(this);
    playBothCuesButton.setButtonText("Play Both Cues");
    playBothCuesButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    addAndMakeVisible(playBothCuesButton);
    
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
    g.setFont (12.0f);
}

void DeckControl::resized()
{
    auto area = getLocalBounds();
    
    // set bounds for the audio player decks
    deck1.setBounds(area.removeFromLeft(getWidth() * 0.40));
    deck2.setBounds(area.removeFromRight(getWidth() * 0.40));
    
    // set bounds for mixer area, in the middle of the deck
    volLeftRightSlider.setBounds(area.removeFromTop(100));
    
    // reserve space for 2 sets of cue buttons: set Cue, play Cue, clear Cue
    // reserving 100 px and arranging button from bottom to leave some black space at the top
    auto subArea1 = area.removeFromTop(100);
    
    // split space into 2 columns for each of the players
    auto leftArea = subArea1.removeFromLeft(getWidth() * 0.10);
    auto rightArea = subArea1.removeFromRight(getWidth() * 0.10);
    
    // cue buttons for each of the players
    clearCueButton1.setBounds(leftArea.removeFromBottom(30));
    cuePlayButton1.setBounds(leftArea.removeFromBottom(30));
    setCueButton1.setBounds(leftArea.removeFromBottom(30));
    
    clearCueButton2.setBounds(rightArea.removeFromBottom(30));
    cuePlayButton2.setBounds(rightArea.removeFromBottom(30));
    setCueButton2.setBounds(rightArea.removeFromBottom(30));
 
    // applies to cues from both players
    playBothCuesButton.setBounds(area.removeFromTop(30));
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
        // slider has values between -1 and 1
        // -1 corresponds to 150% of Player 1 initial volume and 50% of Player 2 initial volume
        // +1 corresponds to 50% of Player 1 initial volume and 150% of Player 2 initial volume
        // 0 leaves the initial volume unchanged
        // volume can be increased to a maximum of 150% and decreased by at most 50% of player initial volume
        double sliderValue = volLeftRightSlider.getValue();
        
        // if slider is moved to the right, volume is increased on the right and decreased on the left side
        // if slider is moved to the left, volume is incresaed on the left side and decreaed on the right side
        if (sliderValue > 0) // moved slider to the right
        {
            player2.setGain(juce::jmin(deck2.getGain() * (1 + sliderValue / 2), 1.5));
            player1.setGain(juce::jmax(deck1.getGain() * (1 - sliderValue / 2), 0.0));
        }
        if (sliderValue < 0) // moved slider to the left
        {
            player1.setGain(juce::jmin(deck1.getGain() * (1 - sliderValue / 2), 1.5));
            player2.setGain(juce::jmax(deck2.getGain() * (1 + sliderValue / 2), 0.0));
        }
    }
}

void DeckControl::buttonClicked(juce::Button* button)
{
    if (button == &setCueButton1)
    {
        // record position on player 1 when button was pressed
        cueStartPosition1 = player1.getPositionRelative();
        // change button colours to give visual feedback that cue is stored
        setCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
        setCueButton1.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    }
    
    if (button == &setCueButton2)
    {
        // record position on player 2 when button was pressed
        cueStartPosition2 = player2.getPositionRelative();
        // change button colours to give visual feedback that cue is stored
        setCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);
        setCueButton2.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    }
    
    if (button == &clearCueButton1)
    {
        // reset cue position
        cueStartPosition1 = -1;
        // change button colours to original colours to give visual feedback that cue is no longer stored
        setCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        setCueButton1.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    }
    
    if (button == &clearCueButton2)
    {
        // reset cue position
        cueStartPosition2 = -1;
        // change button colours to original colours to give visual feedback that cue is no longer stored
        setCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        setCueButton2.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    }
    
    if (button == &cuePlayButton1)
    {
        // only play if the cue position has a valid value
        if (cueStartPosition1 >= 0.0 && cueStartPosition1 <= 1.0)
        {
            player1.setPositionRelative(cueStartPosition1);
            player1.play();
        }
    }
    
    if (button == &cuePlayButton2)
    {
        // only play if the cue position has a valid value
        if (cueStartPosition2 >= 0.0 && cueStartPosition2 <= 1.0)
        {
            player2.setPositionRelative(cueStartPosition2);
            player2.play();
        }
    }
    
    if (button == &playBothCuesButton)
    {
        // only play if both cue positions have a valid value
        if (cueStartPosition1 >= 0 && cueStartPosition2 >= 0 &&
            cueStartPosition1 <= 1.0 && cueStartPosition2 <= 1.0)
        {
            player1.setPositionRelative(cueStartPosition1);
            player2.setPositionRelative(cueStartPosition2);
            player1.play();
            player2.play();
        }
    }
}

