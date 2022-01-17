/*
  ==============================================================================

    SingleNoteSettingsComponent.h
    Created: 30 Apr 2021 8:31:23pm
    Author:  jonny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VisualisationComponent.h"

//==============================================================================
/*
*/
class SingleNoteSettingsComponent  : public juce::Component, 
                           juce::Button::Listener, 
                           juce::Slider::Listener,
                           juce::MidiKeyboardState::Listener
{
public:
    SingleNoteSettingsComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor, VisualisationComponent* visualisation_component=nullptr);
    ~SingleNoteSettingsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;

    void handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override {};
    void setMidiNoteRange(int midi_note_min, int midi_note_max);

private:

    PadSynthJustIntonationPlugInAudioProcessor& audio_processor_;

    juce::Slider wavetable_duration_slider_;
    juce::Slider number_of_harmonics_slider_;
    juce::Slider midi_note_number_slider_;
    juce::Slider frequency_slider_;
    juce::Slider base_frequency_slider_;
    juce::Slider selected_channel_slider_;
    juce::ToggleButton mute_channel_button_;
    juce::ToggleButton validate_button_;

    std::vector<juce::Component*> members_;

    VisualisationComponent* visualisation_component_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleNoteSettingsComponent)
};
