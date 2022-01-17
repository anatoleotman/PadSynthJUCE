/*
  ==============================================================================

    LayerSynth.h
    Created: 8 Apr 2021 8:50:51pm
    Author:  jonny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PadSynthSound.h"
#include "WorkerThread.h"

class LayerSynth : public juce::Synthesiser
{
public:
    LayerSynth();
    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
    enum class populateSynthType { onenote, minimal, complete };
    void populate_synth(LayerSynth::populateSynthType populate_synth_type);
    const int getLowestMidiNote() { return this->midi_notes_lowest_and_highest_.first; };
private:
    WorkerThread thread_;
    bool shouldStealNotes_;
    std::pair<int, int> midi_notes_lowest_and_highest_;
};