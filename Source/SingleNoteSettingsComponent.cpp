/*
  ==============================================================================

    SingleNoteSettingsComponent.cpp
    Created: 30 Apr 2021 8:31:23pm
    Author:  jonny

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SingleNoteSettingsComponent.h"
#include "PadSynthSound.h"


SingleNoteSettingsComponent::SingleNoteSettingsComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor, VisualisationComponent* visualisation_component):
    audio_processor_(audio_processor),
    visualisation_component_(visualisation_component)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    //wavetable_duration_slider_;
    this->wavetable_duration_slider_.setSliderStyle(juce::Slider::LinearBar);
    this->wavetable_duration_slider_.setRange(0.1, 6.0, .1);
    this->wavetable_duration_slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    this->wavetable_duration_slider_.setPopupDisplayEnabled(true, true, nullptr);
    this->wavetable_duration_slider_.setTextValueSuffix(" secs");
    this->wavetable_duration_slider_.setValue(1.0);
    this->addAndMakeVisible(&this->wavetable_duration_slider_);

    //number_of_harmonics_slider_;
    this->number_of_harmonics_slider_.setSliderStyle(juce::Slider::LinearBar);
    this->number_of_harmonics_slider_.setRange(2.0, 64.0, 1);
    this->number_of_harmonics_slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    this->number_of_harmonics_slider_.setPopupDisplayEnabled(true, true, nullptr);
    this->number_of_harmonics_slider_.setTextValueSuffix(" harmonics.");
    this->number_of_harmonics_slider_.setValue(15.0);
    this->addAndMakeVisible(&this->number_of_harmonics_slider_);

    //midi_note_slider_;
    this->midi_note_number_slider_.setSliderStyle(juce::Slider::LinearBar);
    this->midi_note_number_slider_.setRange(60, 72, 1);
    this->midi_note_number_slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    this->midi_note_number_slider_.setPopupDisplayEnabled(true, true, nullptr);
    this->midi_note_number_slider_.setTextValueSuffix(" midi note number");
    this->midi_note_number_slider_.setValue(60);
    this->addAndMakeVisible(&this->midi_note_number_slider_);

    //frequency_slider_;
    this->frequency_slider_.setSliderStyle(juce::Slider::LinearBar);
    this->frequency_slider_.setRange(20.0, 1000.0, .01);
    this->frequency_slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    this->frequency_slider_.setPopupDisplayEnabled(true, true, nullptr);
    this->frequency_slider_.setTextValueSuffix(" Hz freq.");
    this->frequency_slider_.setValue(1.0);
    this->addAndMakeVisible(&this->frequency_slider_);

    //base_frequency_slider_;
    this->base_frequency_slider_.setSliderStyle(juce::Slider::LinearBar);
    this->base_frequency_slider_.setRange(20.0, 1000.0, .01);
    this->base_frequency_slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    this->base_frequency_slider_.setPopupDisplayEnabled(true, true, nullptr);
    this->base_frequency_slider_.setTextValueSuffix(" Hz base freq.");
    this->base_frequency_slider_.setValue(1.0);
    this->addAndMakeVisible(&this->base_frequency_slider_);

    //selected_channel_slider_;
    this->selected_channel_slider_.setSliderStyle(juce::Slider::LinearBar);
    this->selected_channel_slider_.setRange(0, 1, 1);
    this->selected_channel_slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    this->selected_channel_slider_.setPopupDisplayEnabled(true, true, nullptr);
    this->selected_channel_slider_.setTextValueSuffix(" selected channel");
    this->selected_channel_slider_.setValue(0);
    this->addAndMakeVisible(&this->selected_channel_slider_);

    this->mute_channel_button_.setButtonText("active channel");
    this->mute_channel_button_.setToggleState(true, false);
    //this->validate_button_.setClickingTogglesState(true);
    this->addAndMakeVisible(&this->mute_channel_button_);

    this->validate_button_.setButtonText("validate params");
    //this->validate_button_.setClickingTogglesState(true);
    this->addAndMakeVisible(&this->validate_button_);

    this->members_.push_back(&this->midi_note_number_slider_);
    this->members_.push_back(&this->wavetable_duration_slider_);
    this->members_.push_back(&this->number_of_harmonics_slider_);
    this->members_.push_back(&this->frequency_slider_);
    this->members_.push_back(&this->base_frequency_slider_);
    this->members_.push_back(&this->selected_channel_slider_);

    this->members_.push_back(&this->validate_button_);
    this->members_.push_back(&this->mute_channel_button_);

    
    this->midi_note_number_slider_.addListener(this);
    this->selected_channel_slider_.addListener(this);

    this->validate_button_.addListener(this);
    this->mute_channel_button_.addListener(this);

    this->audio_processor_.keyboardState.addListener(this);
}

SingleNoteSettingsComponent::~SingleNoteSettingsComponent()
{
}

void SingleNoteSettingsComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("SettingsComponent", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void SingleNoteSettingsComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //auto area = this->getLocalBounds();
    //this->midi_note_slider_.setBounds(area.removeFromTop(20));
    //this->wavetable_duration_slider_.setBounds(area.removeFromTop(20));
    //this->number_of_harmonics_slider_.setBounds(area.removeFromTop(20));
    //this->frequency_slider_.setBounds(area.removeFromTop(20));
    //this->base_frequency_slider_.setBounds(area.removeFromTop(20));
    //this->validate_button_.setBounds(area.removeFromTop(20));

    float nb_members = 6.f;
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    for (juce::Component* member : this->members_) {
        fb.items.add(juce::FlexItem(*member).withFlex(0, 1, (float)getHeight() / this->members_.size()));
    }
    fb.performLayout(this->getLocalBounds().toFloat());
}

void SingleNoteSettingsComponent::sliderValueChanged(juce::Slider* slider) {
    //TODO make _is_dirty if slider_move
    if (slider == &this->midi_note_number_slider_) {
        int midi_note_index = (int)(this->midi_note_number_slider_.getValue());
        //int sound_index = midi_note_index - (int)this->midi_note_number_slider_.getMinimum();
        int sound_index = midi_note_index - (int)this->audio_processor_.getSynthesiser().getLowestMidiNote();
        PadSynthSound* sound = dynamic_cast<PadSynthSound*>(this->audio_processor_.getSynthesiser().getSound(sound_index).get());
        if (sound != nullptr) {
            int selected_channel = (int)this->selected_channel_slider_.getValue();
            MyPadSynth::PadSynthParameters sound_params(sound->get_parameters(selected_channel));
            int sound_samplerate = sound_params.samplerate;

            this->wavetable_duration_slider_.setValue((double)sound_params.N / sound_samplerate);
            this->number_of_harmonics_slider_.setValue(sound_params.number_harmonics);
            this->frequency_slider_.setValue(sound_params.fundamental_frequency);
            this->base_frequency_slider_.setValue(sound_params.base_frequency);
            this->mute_channel_button_.setToggleState(sound->get_channel_mix(selected_channel), false);

            this->visualisation_component_->update(midi_note_index, selected_channel);

            //DBG("SettingsComponent::sliderValueChanged" << (double)sound_params.N / sound_samplerate);
            DBG("SingleNoteSettingsComponent::sliderValueChanged: sound_params.fundamental_frequency " << sound_params.fundamental_frequency);
        }
    }
    else if (slider == &this->selected_channel_slider_) {
        //DBG("SettingsComponent::sliderValueChanged selected_channel_slider_");
        int midi_note_index = (int)(this->midi_note_number_slider_.getValue());
        //int sound_index = midi_note_index - (int)this->midi_note_number_slider_.getMinimum();
        int sound_index = midi_note_index - (int)this->audio_processor_.getSynthesiser().getLowestMidiNote();
        PadSynthSound* sound = dynamic_cast<PadSynthSound*>(this->audio_processor_.getSynthesiser().getSound(sound_index).get());
        if (sound != nullptr) {
            int selected_channel = (int)this->selected_channel_slider_.getValue();
            MyPadSynth::PadSynthParameters sound_params(sound->get_parameters(selected_channel));
            int sound_samplerate = sound_params.samplerate;
            this->wavetable_duration_slider_.setValue((double)sound_params.N / sound_samplerate);
            this->number_of_harmonics_slider_.setValue(sound_params.number_harmonics);
            this->frequency_slider_.setValue(sound_params.fundamental_frequency);
            this->base_frequency_slider_.setValue(sound_params.base_frequency);
            this->mute_channel_button_.setToggleState(sound->get_channel_mix(selected_channel), false);

            this->visualisation_component_->update(midi_note_index, selected_channel);
            //DBG("SettingsComponent::sliderValueChanged" << (double)sound_params.N / sound_samplerate);
        }
    }
}

void SingleNoteSettingsComponent::buttonClicked(juce::Button* button) {
    //DBG("SettingsComponent::buttonClicked");
    if (button == &this->validate_button_)
    {
        DBG("SettingsComponent::buttonClicked validate_button_");
        int midi_note_index = (int)(this->midi_note_number_slider_.getValue());
        //int sound_index = midi_note_index - (int)this->midi_note_number_slider_.getMinimum();
        int sound_index = midi_note_index - (int)this->audio_processor_.getSynthesiser().getLowestMidiNote();
        if (sound_index <= (this->audio_processor_.getSynthesiser().getNumSounds() - 1)) {
            PadSynthSound* sound = dynamic_cast<PadSynthSound*>(this->audio_processor_.getSynthesiser().getSound(sound_index).get());
            if (sound != nullptr) {
                int selected_channel = (int)this->selected_channel_slider_.getValue();
                MyPadSynth::PadSynthParameters sound_params(sound->get_parameters(selected_channel));
                //int sound_samplerate = sound_params.samplerate;
                //TODO PadSynthSound::set_parameters on selected channel
                //sound->set_parameters ((int)(this->wavetab
                // le_duration_slider_.getValue() * sound_samplerate),
                //                       sound_samplerate,
                //                        (int)this->number_of_harmonics_slider_.getValue(),
                //                        (int)this->frequency_slider_.getValue(),
                //                        (int)this->base_frequency_slider_.getValue()
                //);
                sound_params.N = (int)(this->wavetable_duration_slider_.getValue() * sound_params.samplerate);
                sound_params.number_harmonics = (int)this->number_of_harmonics_slider_.getValue();
                sound_params.fundamental_frequency = (int)this->frequency_slider_.getValue();
                sound_params.base_frequency = (int)this->base_frequency_slider_.getValue();
                sound->set_parameters(sound_params, selected_channel);
            }
        }
    }
    else if (button == &this->mute_channel_button_) {
        //DBG("SettingsComponent::buttonClicked mute_channel_button_");
        int midi_note_index = (int)(this->midi_note_number_slider_.getValue());
        //int sound_index = midi_note_index - (int)this->midi_note_number_slider_.getMinimum();
        int sound_index = midi_note_index - (int)this->audio_processor_.getSynthesiser().getLowestMidiNote();
        if (sound_index <= (this->audio_processor_.getSynthesiser().getNumSounds() - 1)) {
            PadSynthSound* sound = dynamic_cast<PadSynthSound*>(this->audio_processor_.getSynthesiser().getSound(sound_index).get());
            sound->set_channel_mix(this->selected_channel_slider_.getValue(), this->mute_channel_button_.getToggleState());
        }
    }
}

void SingleNoteSettingsComponent::handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) {
    this->midi_note_number_slider_.setValue(midiNoteNumber);
}

void SingleNoteSettingsComponent::setMidiNoteRange(int midi_note_min, int midi_note_max)
{
    this->midi_note_number_slider_.setRange(midi_note_min, midi_note_max, 1.);
}
