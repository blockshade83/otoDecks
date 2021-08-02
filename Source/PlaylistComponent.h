/*
  ==============================================================================

    PlaylistComponent.h
    Created: 2 Aug 2021 10:22:47am
    Author:  Ionut Costache

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener
{
public:
    PlaylistComponent();
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
    
    /** virtual function from TableListBoxModel class, optional */
//    void cellClicked (int rowNumber,
//                              int columnId,
//                              const juce::MouseEvent&) override;
    
    void buttonClicked(juce::Button* button) override;
    
    juce::Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        juce::Component *existingComponentToUpdate) override;

private:
    juce::TableListBox tableComponent;
    std::vector<std::string> trackTitles;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
