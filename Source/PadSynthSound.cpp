/*
  ==============================================================================

    PadSynthSound.cpp
    Created: 4 Apr 2021 5:15:05pm
    Author:  jonny

  ==============================================================================
*/

#include "PadSynthSound.h"
#include <math.h>


PadSynthSound::PadSynthSound(int N,
                            int samplerate,
                            int number_harmonics,
                            float fundamental_frequency,
                            float base_frequency,
                            int midi_note_number_active) : 
    midi_note_number_active_(midi_note_number_active),
    audio_buffer_(2, N),
    //harmonics_(std::vector<float>(2, std::vector<float>(number_harmonics))),
    harmonics_({ std::vector<float>(), std::vector<float>()}),
    channel_mix_({true, false}),
    MyPadSynth( N,
                samplerate,
                number_harmonics,
                fundamental_frequency,
                base_frequency),
    sounds_parameters_(std::vector<PadSynthParameters>(2))
{
    //this->audio_buffer_.setSize(2, N);
    this->copy_wavetable_to_audio_buffer(0);
    PadSynthParameters params0(N, samplerate, number_harmonics, fundamental_frequency, base_frequency);
    this->sounds_parameters_[0] = params0;

    MyPadSynth::set_parameters(N, samplerate, number_harmonics, fundamental_frequency * 3. / 2., base_frequency);
    this->copy_wavetable_to_audio_buffer(1);
    PadSynthParameters params1(N, samplerate, number_harmonics, fundamental_frequency * 3. / 2., base_frequency);
    this->sounds_parameters_[1] = params1;
}

void PadSynthSound::copy_wavetable_to_audio_buffer(int channel)
{
    //copy wavetable to audiobuffer
    //const int numChannels = this->audio_buffer_.getNumChannels();
    //const int numSamples = this->audio_buffer_.getNumSamples();
    //for (int channel = 0; channel < numChannels; ++channel) {
    //    float* writePointer = this->audio_buffer_.getWritePointer(channel);
    //    for (int sample = 0; sample < numSamples; ++sample) {
    //        writePointer[sample] = this->getWavetable().at(sample);
    //    }
    //}
    const int numSamples = this->audio_buffer_.getNumSamples();
    float* writePointer = this->audio_buffer_.getWritePointer(channel);
    for (int sample = 0; sample < numSamples; ++sample) {
        writePointer[sample] = this->getWavetable().at(sample);
    }
    //jassert(channel < this->harmonics_.size());
    this->harmonics_[channel] = MyPadSynth::getHarmonics();
    //this->harmonics_.push_back(MyPadSynth::getHarmonics());
}

void PadSynthSound::set_parameters(int N, int samplerate, int number_harmonics, float fundamental_frequency, float base_frequency)
{
    //TODO PadSynthSound::set_parameters -> selected_chan
    MyPadSynth::set_parameters(N, samplerate, number_harmonics, fundamental_frequency, base_frequency);
    this->audio_buffer_.clear();
    this->audio_buffer_.setSize(2, N);
    this->copy_wavetable_to_audio_buffer(0);
    PadSynthParameters params0(N, samplerate, number_harmonics, fundamental_frequency, base_frequency);
    this->sounds_parameters_.push_back(params0);

    MyPadSynth::set_parameters(N, samplerate, number_harmonics, fundamental_frequency * 3./2., base_frequency);
    PadSynthParameters params1(N, samplerate, number_harmonics, fundamental_frequency * 3. / 2., base_frequency);
    this->sounds_parameters_.push_back(params1);
    this->copy_wavetable_to_audio_buffer(1);
}

void PadSynthSound::set_parameters(PadSynthParameters params, int channel)
{
    MyPadSynth::set_parameters(params.N, params.samplerate, params.number_harmonics, params.fundamental_frequency, params.base_frequency);
    this->audio_buffer_.clear(channel, 0, this->audio_buffer_.getNumSamples());
    this->audio_buffer_.setSize(2, params.N, true, true);
    this->copy_wavetable_to_audio_buffer(channel);
    this->sounds_parameters_[channel] = params;
}

MyPadSynth::PadSynthParameters PadSynthSound::get_parameters(int channel)
{
    jassert(channel < this->sounds_parameters_.size());
    return this->sounds_parameters_.at(channel);
}

std::vector<float> PadSynthSound::getHarmonics(int channel)
{
    return this->harmonics_.at(channel);
    //return std::vector<float>();
}

const juce::AudioBuffer<float> PadSynthSound::getPrototypeSignal()
{
    //copy wavetable to audiobuffer
    int T = this->getSampleRate();
    //float f = 1.;
    juce::AudioBuffer<float> signal_prototype_audio_buffer(1, T);
    //signal_prototype_audio_buffer.setSize(1, T);
    const int numChannels = signal_prototype_audio_buffer.getNumChannels();
    const int numSamples = signal_prototype_audio_buffer.getNumSamples();
    for (int channel = 0; channel < numChannels; ++channel) {
        float* writePointer = signal_prototype_audio_buffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample) {
            //writePointer[sample] = 6*tanh(sin(2 * juce::MathConstants<float>::pi * f * sample / T));
            //writePointer[sample] = sin(2 * juce::MathConstants<float>::pi * f * sample / T);
            writePointer[sample] = this->prototype_signal_.at(sample);
        }
    }
    return signal_prototype_audio_buffer;
}

float PadSynthSound::getSample(int sampleIndex)
{
    float sample_mix = 0.;
    if (this->channel_mix_.at(0)) sample_mix += this->audio_buffer_.getSample(0, sampleIndex);
    if (this->channel_mix_.at(1)) sample_mix += this->audio_buffer_.getSample(1, sampleIndex); //crash if no channel 1
    return sample_mix;
}


void PadSynthSound::set_channel_mix(int channel, bool select_channel)
{
    jassert(channel < this->channel_mix_.size());
    this->channel_mix_[channel] = select_channel;
}
