#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    addAndMakeVisible(deckControl);
    addAndMakeVisible(&playlist);
    
    setSize (1000, 650);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    deckControl.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    deckControl.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    deckControl.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
    g.setFont(12.0f);
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    deckControl.setBounds(area.removeFromTop(getHeight() * 0.4));
    playlist.setBounds(area.removeFromBottom(getHeight()));
}
