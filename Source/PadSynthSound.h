/*
  ==============================================================================

    PadSynthSound.h
    Created: 4 Apr 2021 5:15:05pm
    Author:  jonny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyPadSynth.h"

class PadSynthSound : public juce::SynthesiserSound, public MyPadSynth
{
public:
    PadSynthSound ( int N,
                    int samplerate,
                    int number_harmonics,
                    float fundamental_frequency,
                    float base_frequency,
                    int midi_note_number_active);

    bool appliesToNote(int midi_note_number) override { if (midi_note_number == this->midi_note_number_active_) return true; else return false; }
    bool appliesToChannel (int) override        { return true; }

    const juce::AudioBuffer<float>& getAudioBuffer() { return this->audio_buffer_; }

    void set_parameters(int N, int samplerate, int number_harmonics, float fundamental_frequency, float base_frequency);
    void set_parameters(PadSynthParameters params, int channel);
    PadSynthParameters get_parameters(int channel);
    std::vector<float> getHarmonics(int channel);
    bool get_channel_mix(int channel) {
        return this->channel_mix_.at(channel);
    };

    const juce::AudioBuffer<float> getPrototypeSignal();

    float getSample(int sampleIndex);

    void set_channel_mix(int channel, bool select_channel);

private:
    void copy_wavetable_to_audio_buffer(int channel);
    int midi_note_number_active_;
    juce::AudioBuffer<float> audio_buffer_;
    std::vector<std::vector<float>> harmonics_;
    std::vector<bool> channel_mix_;
    std::vector<PadSynthParameters> sounds_parameters_;
};