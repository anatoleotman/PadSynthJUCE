/*
  ==============================================================================

    PadSynthVoice.h
    Created: 4 Apr 2021 5:15:26pm
    Author:  jonny

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PadSynthSound.h"

class PadSynthVoice   : public juce::SynthesiserVoice
{
public:    
    PadSynthVoice();

    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<PadSynthSound*> (sound) != nullptr;
    }

    void startNote (int midiNoteNumber,
                    float velocity,
                    juce::SynthesiserSound* synthesiser_sound,
                    int /*currentPitchWheelPosition*/) override;

    void stopNote (float /*velocity*/, bool allowTailOff) override;


    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

    void setADSRSampleRate(double sampleRate);

    void getEnvelopeParams(std::atomic<float>* attack, std::atomic<float>* decay, std::atomic<float>* sustain, std::atomic<float>* release);
private:
    PadSynthSound* current_synthesiser_sound_;
    juce::ADSR adsr_;
    juce::ADSR::Parameters adsr_parameters_;
    int current_index_;
    juce::AudioBuffer<float> overlap_buffer_;
    int overlap_index_;
    juce::ADSR fade_out_;
    juce::ADSR::Parameters fade_out_parameters_;
};