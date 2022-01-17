/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PadSynthJustIntonationUIComponent.h"
//#include "ADSRComponent.h"
#include "SingleNoteSettingsComponent.h"
#include "GlobalSettingsComponent.h"
#include "VisualisationComponent.h"

//==============================================================================
/**
*/
class PadSynthJustIntonationPlugInAudioProcessorEditor  : public juce::AudioProcessorEditor
    //, juce::KeyListener
{
public:
    PadSynthJustIntonationPlugInAudioProcessorEditor (PadSynthJustIntonationPlugInAudioProcessor& p);
    ~PadSynthJustIntonationPlugInAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //bool keyPressed(const juce::KeyPress& key, Component* originatingComponent) override;
    bool keyPressed(const juce::KeyPress& key) override;
private:
    void setMidiInput(int index);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PadSynthJustIntonationPlugInAudioProcessor& audioProcessor;

    PadSynthJustIntonationUIComponent pad_synth_UI_component_;

    juce::AudioDeviceManager deviceManager;

    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;

    juce::MidiKeyboardComponent midiKeyboardComponent;
    int midiKeyboardComponentBaseOctave_;
    GlobalSettingsComponent global_settings_component_;
    //ADSRComponent ADSRComponent_;

    //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> slider_attachement_;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> slider1_attachement_;

    SingleNoteSettingsComponent single_note_settings_component_;
    VisualisationComponent visualisation_component_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadSynthJustIntonationPlugInAudioProcessorEditor)
};
