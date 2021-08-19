/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 2 Aug 2021 10:22:47am
    Author:  Ionut Costache

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <filesystem>
#include <unistd.h>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckControl* _deckControl)
: deckControl{_deckControl}

{

    // initialize table component
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("Track Duration", 2, 100);
    tableComponent.getHeader().addColumn("", 3, 100);
    tableComponent.getHeader().addColumn("", 4, 100);
    tableComponent.getHeader().addColumn("", 5, 100);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    
    // initialize buttons
    loadTrackButton.addListener(this);
    loadTrackButton.setButtonText("Load Track");
    loadTrackButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    addAndMakeVisible(loadTrackButton);
    
    loadPlaylistButton.addListener(this);
    loadPlaylistButton.setButtonText("Load Playlist");
    loadPlaylistButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    addAndMakeVisible(loadPlaylistButton);
    
    savePlaylistButton.addListener(this);
    savePlaylistButton.setButtonText("Save Playlist");
    savePlaylistButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    addAndMakeVisible(savePlaylistButton);
    
    // initialize search box and label
    searchBox.addListener(this);
    searchBox.setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkgrey);
    searchBox.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    addAndMakeVisible(searchBox);

    searchBoxLabel.setText("Search:");
    searchBoxLabel.setReadOnly(true);
    searchBoxLabel.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    searchBoxLabel.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    addAndMakeVisible(searchBoxLabel);
    
    // load default playlist - i.e., the last one used
    loadPlaylist();
    
    startTimer(200);
}

PlaylistComponent::~PlaylistComponent()
{
    stopTimer();
}

void PlaylistComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (12.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    auto area = getLocalBounds();
    
    // reserve space for the search box section, at the top of the playlist area
    auto searchBoxArea = area.removeFromTop(juce::jmin(getHeight() * 0.20, 30.0));
    searchBoxLabel.setBounds(searchBoxArea.removeFromLeft(juce::jmin(getWidth() * 0.50, 100.0)));
    searchBox.setBounds(searchBoxArea.removeFromLeft(getWidth() * 0.2));
    
    // reserve space for the buttons, at the bottom of the playlist area
    auto buttonsArea = area.removeFromBottom(juce::jmin(getHeight() * 0.20, 50.0));
    loadTrackButton.setBounds(buttonsArea.removeFromLeft(getWidth() / 3));
    loadPlaylistButton.setBounds(buttonsArea.removeFromLeft(getWidth() / 3));
    savePlaylistButton.setBounds(buttonsArea.removeFromLeft(getWidth() / 3));
    
    // remaining space is allocated for the actual list of songs
    tableComponent.setBounds(area.removeFromTop(getHeight()));
}

int PlaylistComponent::getNumRows ()
{
    return int(tracks.size());
}

void PlaylistComponent::paintRowBackground (juce::Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    // just highlight selected rows
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::darkgrey);
    }
    else
    {
        g.fillAll(juce::Colours::black);
    }
}

void PlaylistComponent::paintCell (juce::Graphics & g,
                                   int rowNumber,
                                   int columnId,
                                   int width,
                                   int height,
                                   bool rowIsSelected)
{
    g.setColour (juce::Colours::white);
    g.setFont (12.0f);
    
    if (columnId == 1) // Track name
    {
        g.drawText (tracks[rowNumber].trackName,
                    2, 0,
                    width - 4, height,
                    juce::Justification::centredLeft,
                    true);
    }
    if (columnId == 2) // Track duration
    {
        g.drawText (tracks[rowNumber].trackDuration,
                    2, 0,
                    width - 4, height,
                    juce::Justification::centred,
                    true);
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell (int rowNumber,
                                                       int columnId,
                                                       bool isRowSelected,
                                                       juce::Component *existingComponentToUpdate)
{
    if (columnId == 3) // Play on 1 - left player
    {
        if (existingComponentToUpdate == nullptr)
        {
            // codify button id by adding 1 at the beginning of the button code
            juce::String id{"1." + std::to_string(rowNumber)};
            juce::TextButton* btn = new juce::TextButton("Play on 1");
            existingComponentToUpdate = btn;
            btn->setComponentID(id);
            btn->addListener(this);
        }
    }
    if (columnId == 4) // Play on 2 - right player
    {
        if (existingComponentToUpdate == nullptr)
        {
            // codify button id by adding 2 at the beginning of the button code
            juce::String id{"2." + std::to_string(rowNumber)};
            juce::TextButton* btn = new juce::TextButton("Play on 2");
            existingComponentToUpdate = btn;
            btn->setComponentID(id);
            btn->addListener(this);
        }
    }
    if (columnId == 5) // Remove track from playlist
    {
        if (existingComponentToUpdate == nullptr)
        {
            // codify button id by adding 3 at the beginning of the button code
            juce::String id{"3." + std::to_string(rowNumber)};
            juce::TextButton* btn = new juce::TextButton("Remove");
            existingComponentToUpdate = btn;
            btn->setComponentID(id);
            btn->addListener(this);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &loadTrackButton)
    {
        // select file to load
        juce::FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            // add track to playlist
            juce::URL audioURL = juce::URL{chooser.getResult()};
            allTracks.push_back(Track(audioURL));
            // refresh list
            tableComponent.updateContent();
            // save list to default
            saveDefaultPlaylist();
        }
    }
    
    else if (button == &loadPlaylistButton)
    {
        // load playlist from file, default mode = false
        loadPlaylist(false);
    }
    
    else if (button == &savePlaylistButton)
    {
        // save current playlist to file
        saveToFile();
    }
    
    else // we clicked on the buttons specific to the tracks
    {
        // get the ID of the button we pressed
        std::string buttonID = button->getComponentID().toStdString();
        // extract the action type corresponding to the button id
        // 1 - play on left, 2 - play on right, 3 - delete track
        std::string buttonAction = buttonID.substr(0,1);
        // extract the track ID by ignoring the first 2 characters, which were reserved for the action type
        int trackID = std::stoi(buttonID.substr(2));
        if (buttonAction == "1") // play track on 1
        {
            deckControl->deck1.waveformDisplay.loadURL(tracks[trackID].trackURL);
            deckControl->player1.loadURL(tracks[trackID].trackURL);
            deckControl->player1.play();
            deckControl->deck1.nowPlaying.setText("Now playing: " + tracks[trackID].trackURL.getFileName());
        }
        if (buttonAction == "2") // play track on 2
        {
            deckControl->deck2.waveformDisplay.loadURL(tracks[trackID].trackURL);
            deckControl->player2.loadURL(tracks[trackID].trackURL);
            deckControl->player2.play();
            deckControl->deck2.nowPlaying.setText("Now playing: " + tracks[trackID].trackURL.getFileName());
        }
        if (buttonAction == "3") // delete from playlist
        {
            if (filteredTracks.size() > 0)
            {
                int positionToDelete;
                // search the complete list of tracks to know which one to delete
                // this is important when using the button while filtering the list
                for (int i = 0; i < allTracks.size(); ++i)
                {
                    if (allTracks[i].trackURL == filteredTracks[trackID].trackURL)
                    {
                        positionToDelete = i;
                    }
                }
                // remove track from the playlists
                tracks.erase(tracks.begin() + trackID);
                allTracks.erase(allTracks.begin() + positionToDelete);
            }

            else
            {
                allTracks.erase(allTracks.begin() + trackID);
            }
            tableComponent.updateContent();
            // save to default playlist
            saveDefaultPlaylist();
        }
    }
}

void PlaylistComponent::saveDefaultPlaylist()
{
    // fixed location for default playlist
    juce::File playlistFile = juce::File{"~/Playlist.csv"};
    if ( ! playlistFile.existsAsFile() )
    {
        playlistFile.create();
    }
    // create output stream, set position to 0 and remove any pre-existing values
    juce::FileOutputStream output(playlistFile);
    output.setPosition(0);
    output.truncate();
    
    // send track URL values to output stream
    for (Track& currentTrack : allTracks)
    {
        output << currentTrack.trackURL.toString(false) << "\n";
    }
}

void PlaylistComponent::loadPlaylist(bool defaultMode)
{
    juce::String line;
    juce::File playlistFile;
    
    if (defaultMode == false) // load playlist from file
    {
        juce::FileChooser chooser{"Select a file..."};
        chooser.browseForFileToOpen();
        playlistFile = chooser.getResult();
    }
    
    if (defaultMode == true) // load default playlist from fixed location, i.e., most recent one used
    {
        playlistFile = juce::File{"~/Playlist.csv"};
        playlistFile.create();
    }
    
    juce::FileInputStream input(playlistFile);
        
    if (input.openedOk())
    {
        // remove any values existing in the allTracks vector
        allTracks.erase(allTracks.begin(), allTracks.end());
        
        // read until we reach the end of the file
        // using try / catch approach to avoid crashing the application if the wrong type of file is selected
        while(input.isExhausted() == false)
        {
            try
            {
                // CSV should have one track URL per each line
                line = input.readNextLine();
                Track newTrack = Track{juce::URL(line)};
                // add track to playlist vector
                allTracks.push_back(newTrack);
            }
            catch(const std::exception& e)
            {
                std::cout << "PlaylistComponent::loadPlaylist error - could not read line" << std::endl;
            }
        }// end of while
        tableComponent.updateContent();
        // save to default after we load but only if input opened ok
        saveDefaultPlaylist();
    }
    else
    {
        std::cout << "Playlist not loaded" << std::endl;
    }
}

void PlaylistComponent::saveToFile()
{
    // open file chooser
    juce::FileChooser chooser{"Save Playlist", juce::File()};

    if (chooser.browseForFileToSave(true))
    {
        juce::FileOutputStream output(chooser.getResult());
        // clear current content just in case an existing file was selected
        output.setPosition(0);
        output.truncate();
        
        for (Track& currentTrack : tracks)
        {
            output << currentTrack.trackURL.toString(false) << "\n";
        }
    }
}

void PlaylistComponent::timerCallback()
{
    // extract text typed in the search box
    juce::String searchValue = searchBox.getText();
    
    // only trigger search if we typed at least 3 characters
    if (searchValue.length() >= 3)
    {
        // clear content of filtered tracks
        filteredTracks.clear();
        
        // iterate over the list of tracks
        for (Track& currentTrack : allTracks)
        {
            // check if the name of the file contains the text we typed
            // not case-sensitive
            if (currentTrack.trackName.containsIgnoreCase(searchValue))
            {
                // extract all tracks which meet the condition to filteredTracks vector
                filteredTracks.push_back(currentTrack);
            }
        }
        // change reference of tracks to filteredTracks
        tracks = filteredTracks;
        tableComponent.updateContent();
    }
    
    else
    {
        // search condition no longer applies, changing reference for tracks to allTracks
        tracks = allTracks;
        // update table and clear filtered tracks
        tableComponent.updateContent();
        filteredTracks.clear();
    }
}
