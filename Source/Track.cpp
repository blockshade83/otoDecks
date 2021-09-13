/*
  ==============================================================================

    Track.cpp
    Created: 9 Aug 2021 11:22:52am
    Author:  Ionut Costache

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Track.h"

//==============================================================================
Track::Track(juce::URL _trackURL)
: trackURL{_trackURL}
{
    trackName = trackURL.getFileName();
    if (trackURL.isLocalFile())
    {
        trackDuration = getTrackDuration(trackURL);
    }
}

Track::~Track()
{
}

juce::String Track::getTrackDuration(juce::URL trackFileURL)
{
    // initialize format manager and reader to get track duration
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    juce::AudioFormatReader* reader = formatManager.createReaderFor(trackFileURL.createInputStream(false));
    
    if (reader == nullptr)
    {
        return "0:00";
    }
    else
    {
        int duration;
        // calculate duration depending on number of samples and sampleRate
        if (reader->sampleRate > 0) {
            duration = reader->lengthInSamples / reader->sampleRate;
        }
        else
        {
            duration = 0;
        }
        // extract minutes and seconds from duration in seconds
        int durationMinutes = duration / 60;
        int durationSeconds = duration % 60;
        
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
}
