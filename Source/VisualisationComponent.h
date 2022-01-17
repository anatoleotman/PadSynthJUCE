/*
  ==============================================================================

    VisualisationComponent.h
    Created: 16 May 2021 1:39:12pm
    Author:  jonny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class VisualisationComponent  : public juce::Component, juce::MidiKeyboardState::Listener
{
public:
    VisualisationComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor);
    ~VisualisationComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override {};

    void update(int midiNoteNumber);
    void update(int midiNoteNumber, int selected_channel);
private:
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;

    std::vector<float> harmonics_;
    int last_selected_channel_;
    //juce::AudioBuffer<float> signal_prototype_audio_buffer_;

    PadSynthJustIntonationPlugInAudioProcessor& audio_processor_;

    void init_thumbnail();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualisationComponent)
};
