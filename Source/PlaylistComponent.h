/*
  ==============================================================================

    PlaylistComponent.h
    Created: 2 Aug 2021 10:22:47am
    Author:  Ionut Costache

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Track.h"
#include "DeckControl.h"
#include <fstream>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener,
                           public juce::Timer
{
public:
    PlaylistComponent(DeckControl* _deckControl);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** virtual function required by TableListBoxModel class *
     * returns the number of rows in the table */
    int getNumRows () override;
    
    /** virtual function required by TableListBoxModel class *
     * provides graphical code which draws the background of a row in the table */
    void paintRowBackground (juce::Graphics&,
                             int rowNumber,
                             int width,
                             int height,
                             bool rowIsSelected) override;
    
    /** virtual function required by TableListBoxModel class *
     * contains graphical code which draws the contents of individual cells */
    void paintCell (juce::Graphics&,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;
    
    /** virtual function from Timer class */
    void timerCallback() override;
    
    /** virtual function brom Button class */
    void buttonClicked(juce::Button* button) override;
    
    juce::Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        juce::Component *existingComponentToUpdate) override;
private:
    // variables for the deck control and table components
    DeckControl* deckControl;
    juce::TableListBox tableComponent;
    
    std::vector<Track> allTracks; // all tracks available in the library
    std::vector<Track> filteredTracks; // stores the tracks that we filter dynamically
    std::vector<Track> tracks; // used to switch from allTracks to filteredTracks depending on the active filtering
    
    // playlist buttons and search box
    juce::TextButton loadTrackButton;
    juce::TextButton loadPlaylistButton;
    juce::TextButton savePlaylistButton;
    juce::TextEditor searchBoxLabel;
    juce::TextEditor searchBox;
    
    /** saves most recent selection as default playlist so that *
     * the user can have them when re-opening the application */
    void saveDefaultPlaylist();
    
    /** load the default playlist, which was the most recent selection used */
//    void loadDefaultPlaylist();
    
    /** load default playlist or open selection window to load from file */
    void loadPlaylist(bool defaultMode = true);
    
    /** save current selection to a CSV file*/
    void saveToFile();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
