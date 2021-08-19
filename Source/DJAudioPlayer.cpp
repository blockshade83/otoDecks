/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 15 Jul 2021 11:09:40am

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)

: formatManager(_formatManager)

{
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resampleSource.getNextAudioBlock (bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::play()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    if (posInSecs < 0 || posInSecs > transportSource.getLengthInSeconds())
    {
        DBG("DJAudioPlayer::setPosition: warning set position " << posInSecs << " greater than length " << transportSource.getLengthInSeconds() );
        return;
    }
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    double posInSecs = pos * transportSource.getLengthInSeconds();
    setPosition(posInSecs);
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.5)
    {
        DBG("DJAudioPlayer::setGain: input should be between 0 and 1.5");
    }
    else
    {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 10)
    {
        DBG("DJAudioPlayer::setSpeed: input should be between 0 and 10");
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

double DJAudioPlayer::getPositionRelative()
{
    if (transportSource.getLengthInSeconds() > 0)
    {
        return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
    else
    {
        return 0;
    }
}


double DJAudioPlayer::getDuration()
{
    return transportSource.getLengthInSeconds();
}

