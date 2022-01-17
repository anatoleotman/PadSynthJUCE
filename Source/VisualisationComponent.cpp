/*
  ==============================================================================

    VisualisationComponent.cpp
    Created: 16 May 2021 1:39:12pm
    Author:  jonny

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VisualisationComponent.h"
#include <algorithm>

//==============================================================================
VisualisationComponent::VisualisationComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor) :
    thumbnailCache(5),
    thumbnail(16, formatManager, thumbnailCache),
    //harmonics_(nullptr),
    last_selected_channel_(0),
    audio_processor_(audio_processor)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    //int nb_sounds = this->audio_processor_.getSynthesiser().getNumSounds();
    //PadSynthSound* sound = dynamic_cast<PadSynthSound*>(this->audio_processor_.getSynthesiser().getSound(0).get());
    //if (sound != nullptr) {
    //    this->harmonics_ = sound->getHarmonics();
    //    //this->signal_prototype_audio_buffer_ = sound->getPrototypeSignal();
    //    this->init_thumbnail();
    //}
    this->audio_processor_.keyboardState.addListener(this);
}

VisualisationComponent::~VisualisationComponent()
{
}

void VisualisationComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    //g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("VisualisationComponent", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text

    juce::Rectangle<int> local_bounds = this->getLocalBounds();

    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //g.setColour(juce::Colours::darkblue);
    g.fillRect(local_bounds);
    //juce::Rectangle<int> area1 = local_bounds.removeFromTop(200);

    if (this->harmonics_.size()) {
        g.setColour(juce::Colours::grey);
        float harmonic_max_amplitude = *std::max_element(this->harmonics_.begin(), this->harmonics_.end());
        float harmonic_width = (float)local_bounds.getWidth() / (float)this->harmonics_.size();
        //harmonic_width /= 2;
        for (int i = 0; i != this->harmonics_.size(); ++i) {
            float harmonic_amplitude_normalized = this->harmonics_.at(i) / harmonic_max_amplitude * local_bounds.getHeight();
            juce::Rectangle<float> harmonic_rect(harmonic_width * i, local_bounds.getHeight() - harmonic_amplitude_normalized, harmonic_width, harmonic_amplitude_normalized);

            g.fillRect(harmonic_rect);
        }
    }
    g.setColour(juce::PixelARGB::PixelARGB(100, 255, 255, 255));
    //juce::Rectangle<int> thumbnail_bounds = local_bounds.removeFromLeft(harmonic_width);
    this->thumbnail.drawChannels(g,
                                local_bounds,
                                0.0,                                    // start time
                                thumbnail.getTotalLength(),             // end time
                                1.0f);                                  // vertical zoom
}

void VisualisationComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //float nb_members = 4.f;
    //juce::FlexBox fb;
    //fb.flexDirection = juce::FlexBox::Direction::row;
    //for (juce::Component* member : this->members_) {
    //    fb.items.add(juce::FlexItem(*member).withFlex(0, 1, (float)getWidth() / this->members_.size()));
    //}
    //fb.performLayout(this->getLocalBounds().toFloat());
}

void VisualisationComponent::handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    //DBG("VisualisationComponent::handleNoteOn");
    this->update(midiNoteNumber);
}

void VisualisationComponent::update(int midiNoteNumber)
{
    this->update(midiNoteNumber, this->last_selected_channel_);
}

void VisualisationComponent::update(int midiNoteNumber, int selected_channel) {
    this->last_selected_channel_ = selected_channel;
    int lowest_midi_note_number = this->audio_processor_.getSynthesiser().getLowestMidiNote();
    int sound_index = midiNoteNumber - lowest_midi_note_number;
    PadSynthSound* sound = dynamic_cast<PadSynthSound*>(this->audio_processor_.getSynthesiser().getSound(sound_index).get());
    if (sound != nullptr) {
        this->harmonics_ = sound->getHarmonics(selected_channel);
        //DBG("VisualisationComponent::update" << sound_index << " / nb_harmonics: " << this->harmonics_.size());
        this->repaint();
        ////this->signal_prototype_audio_buffer_ = sound->getPrototypeSignal();
        //this->init_thumbnail();
        MyPadSynth::PadSynthParameters sound_params(sound->get_parameters(selected_channel));
        int sound_samplerate = sound_params.samplerate;
        this->thumbnail.reset(1, sound_samplerate);
        juce::AudioSampleBuffer sampleHold(1, sound->getAudioBuffer().getNumSamples());
        sampleHold.copyFrom(0, 0, sound->getAudioBuffer().getReadPointer(selected_channel), sound->getAudioBuffer().getNumSamples());
        //this->thumbnail.addBlock(0, sound->getAudioBuffer(), 0, sound->getAudioBuffer().getNumSamples() / 6);
        this->thumbnail.addBlock(0, sampleHold, 0, sampleHold.getNumSamples() / 6);
    }
}

void VisualisationComponent::init_thumbnail()
{
    //thumbnail.addChangeListener(this);
    PadSynthSound* sound = dynamic_cast<PadSynthSound*>(this->audio_processor_.getSynthesiser().getSound(0).get());
    MyPadSynth::PadSynthParameters sound_params(sound->get_parameters(0));
    int sound_samplerate = sound_params.samplerate;
    this->thumbnail.reset(1, sound_samplerate);
    //this->thumbnail.addBlock(0, sound->getAudioBuffer(), 0, sound->getAudioBuffer().getNumSamples() / 6);
    this->thumbnail.addBlock(0, sound->getPrototypeSignal(), 0, sound->getPrototypeSignal().getNumSamples());
    this->thumbnail.addBlock(sound->getPrototypeSignal().getNumSamples(), sound->getPrototypeSignal(), 0, sound->getPrototypeSignal().getNumSamples());

}
