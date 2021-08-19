/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 15 Jul 2021 11:09:40am

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer: public juce::AudioSource
{

public:
    DJAudioPlayer(juce::AudioFormatManager& formatManager);
    ~DJAudioPlayer();
    
    /** virtual function from juce::AudioSource, tells the source to prepare for playing */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    /** virtual function from juce::AudioSource, called repeatedly to fetch subsequent blocks of audio data */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    
    /** virtual function from juce::AudioSource, allows the source to release anything it *
     * no longer needs after playback has stopped */
    void releaseResources() override;
    
    /** load file from a local folder or from an online source */
    void loadURL(juce::URL file);
    
    /** start playing the audio file */
    void play();

    /** stop playing the audio file */
    void stop();

    /** go to a position in the audio file, specified in seconds */
    void setPosition(double posInSecs);
    
    /** set relative position in the audio file, specified as a fraction between 0 and 1 */
    void setPositionRelative(double pos);

    /** set volume for the audio file */
    void setGain(double gain);
    
    /** set speed for the audio file */
    void setSpeed(double ratio);
    
    /** get the relative position of the play head */
    double getPositionRelative();
    
    /** get the duration of the track currently playing */
    double getDuration();

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
