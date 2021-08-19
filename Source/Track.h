/*
  ==============================================================================

    Track.h
    Created: 9 Aug 2021 11:22:52am
    Author:  Ionut Costache

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>

//==============================================================================
/*
*/
class Track
{
public:
    Track(juce::URL _trackURL);
    ~Track();
    
    juce::URL trackURL;
    juce::String trackName;
    juce::String trackDuration;
    juce::String getTrackDuration(juce::URL trackFileURL);

private:
    
    
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Track)
};
