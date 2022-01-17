/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fftw3.h>
#include <armadillo>
#include "MyPadSynth.h"
#include "PadSynthSound.h"
#include "PadSynthVoice.h"
#include "LayerSynth.h"

#include <vector>

//TODOLIST
// void SingleNoteSettingsComponent::buttonClicked(juce::Button* button)
// void SingleNoteSettingsComponent::sliderValueChanged(juce::Slider* slider)
// update values and graph/harmonic histogram on change
//==============================================================================
/**
*/
class PadSynthJustIntonationPlugInAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    PadSynthJustIntonationPlugInAudioProcessor();
    ~PadSynthJustIntonationPlugInAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    LayerSynth& getSynthesiser() { return this->synthesiser_; }
    //enum class populateSynthType { minimal, complete};
    //void populate_synth(PadSynthJustIntonationPlugInAudioProcessor::populateSynthType populate_synth_type);

    juce::AudioProcessorValueTreeState apvts_;

    //float volume_track1_;
    //float volume_track2_;

    juce::MidiKeyboardState keyboardState;
    juce::MidiMessageCollector midiCollector;
private:
    double lastSampleRate_;

    //MyPadSynth pad_synth_;
    std::vector<MyPadSynth> synths_;
    juce::AudioParameterFloat* gain;
    
    //juce::Synthesiser synthesiser_;
    //std::vector<PadSynthSound> synth_sounds_;
    LayerSynth synthesiser_;
    PadSynthVoice* myVoice;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadSynthJustIntonationPlugInAudioProcessor)

};
