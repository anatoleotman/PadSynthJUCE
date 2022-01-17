/*
  ==============================================================================

    LayerSynth.cpp
    Created: 8 Apr 2021 8:50:52pm
    Author:  jonny

  ==============================================================================
*/

#include "LayerSynth.h"

LayerSynth::LayerSynth() : Synthesiser(),
                           shouldStealNotes_(true)
{

}

//==============================================================================
void LayerSynth::noteOn(const int midiChannel,
                        const int midiNoteNumber,
                        const float velocity)
{
    const juce::ScopedLock sl(lock);

    for (auto* sound : sounds)
    {
        if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
        {
            // If hitting a note that's still ringing, stop it first (it could be
            // still playing because of the sustain or sostenuto pedal).
            for (auto* voice : voices)
                if ((voice->getCurrentlyPlayingNote() == midiNoteNumber) && voice->isPlayingChannel(midiChannel) && (voice->getCurrentlyPlayingSound() == sound)) {
                    stopVoice(voice, 1.0f, true);
                }

            startVoice(findFreeVoice(sound, midiChannel, midiNoteNumber, this->shouldStealNotes_),
                sound, midiChannel, midiNoteNumber, velocity);
        }
    }
}
//==============================================================================
void LayerSynth::populate_synth(LayerSynth::populateSynthType populate_synth_type)
{
    auto populate_synth = [this] (size_t lowest_midi_note_number, size_t highest_midi_note_number, double nb_seconds) {
        this->midi_notes_lowest_and_highest_.first = lowest_midi_note_number;
        this->midi_notes_lowest_and_highest_.second = highest_midi_note_number;
        double sample_rate = this->getSampleRate();
        //double nb_seconds = .5;
        int N = (int)(sample_rate * nb_seconds);
        int number_harmonics = 64;
        float fundamental_frequency;
        float base_frequency;
        auto start = std::chrono::high_resolution_clock::now();

        this->clearSounds();
        //this->synth_sounds_.reserve(highest_midi_note_number - lowest_midi_note_number);
        for (int midi_note_number = lowest_midi_note_number; midi_note_number != highest_midi_note_number; ++midi_note_number) {
            fundamental_frequency = (float)juce::MidiMessage::getMidiNoteInHertz(midi_note_number);
            base_frequency = fundamental_frequency;
            //this->synth_sounds_.push_back(PadSynthSound(N, sample_rate, number_harmonics, fundamental_frequency, base_frequency, midi_note_number));
            this->addSound(new PadSynthSound(N, sample_rate, number_harmonics, fundamental_frequency, base_frequency, midi_note_number));
            DBG("LayerSynth::populate_synth: addSound" << " midi_note_number " << midi_note_number << " fundamental_frequency " << fundamental_frequency);
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        DBG("LayerSynth::populate_synth: Time taken " << duration.count() * .000001 << " sec");
    };
    switch (populate_synth_type) {
    case populateSynthType::onenote:
        populate_synth(60, 61,.5);
        //juce::Thread::launch([this, populate_synth] { populate_synth(60, 72, .5); });
        break;

    case populateSynthType::minimal:
        //populate_synth(60, 72, .5);

        juce::Thread::launch([this, populate_synth] { 
            populate_synth(60, 72, .5); });
        break;

    case populateSynthType::complete:
        //populate_synth(8, 96, 6.);
        juce::Thread::launch([this, populate_synth] { 
            populate_synth(8, 96, 6.);
        });
        this->thread_.startThread();
        break;
    }
}
//==============================================================================