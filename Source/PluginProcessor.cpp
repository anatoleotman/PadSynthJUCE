/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PadSynthJustIntonationPlugInAudioProcessor::PadSynthJustIntonationPlugInAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        apvts_(*this, nullptr, "Parameters", this->createParameterLayout()),
        //volume_track1_(1.),
        //volume_track2_(1.),
        //pad_synth_(48000 * 6, 48000, 64, 260.137f, 260.137f),
        synths_{MyPadSynth(48000 * 6, 48000, 64, 260.137f * 2, 260.137f),
                MyPadSynth(48000 * 6, 48000, 64, 260.137f, 260.137f)}
#endif
{
    for (auto i = 0; i < 4; ++i) {
        this->synthesiser_.addVoice(new PadSynthVoice());
    }
    //this->synthesiser_.addSound(new PadSynthSound(48000 * 6, 48000, 64, 260.137, 260.137, 60));
    //this->synthesiser_.addSound(new PadSynthSound(48000 * 6, 48000, 64, 260.137 * 2, 260.137, 61));
    //this->synthesiser_.addSound(new PadSynthSound(48000 * 6, 48000, 64, 260.137, 260.137, 62));
    //this->synthesiser_.addSound(new PadSynthSound(48000 * 6, 48000, 64, 260.137 * 3 / 2, 260.137, 62));
    //this->synthesiser_.addSound(new PadSynthSound(48000 * 6, 48000, 64, 260.137 * 3 / 2, 260.137, 67));

    //int sample_rate = 48000;
    //int N = sample_rate * 6;
    //int number_harmonics = 64;
    //float fundamental_frequency;
    //float base_frequency;
    //int lowest_midi_note_number = 60;//30;
    //int highest_midi_note_number = 72;// 80;
    //this->synth_sounds_.reserve(highest_midi_note_number - lowest_midi_note_number);
    //for (int midi_note_number = lowest_midi_note_number; midi_note_number != highest_midi_note_number; ++midi_note_number) {
    //    fundamental_frequency = juce::MidiMessage::getMidiNoteInHertz(midi_note_number);
    //    base_frequency = fundamental_frequency;
    //    //this->synth_sounds_.push_back(PadSynthSound(N, sample_rate, number_harmonics, fundamental_frequency, base_frequency, midi_note_number));
    //    this->synthesiser_.addSound(new PadSynthSound(N, sample_rate, number_harmonics, fundamental_frequency, base_frequency, midi_note_number));
    //    DBG("this->synth_sounds_.push_back" << "midi_note_number" << midi_note_number);
    //}

    //for (PadSynthSound& pad_synth_sound : this->synth_sounds_) {
    //    this->synthesiser_.addSound(pad_synth_sound);
    //}

    //this->synthesiser_.populate_synth(LayerSynth::populateSynthType::onenote);
    //this->synthesiser_.populate_synth(LayerSynth::populateSynthType::minimal);
}

PadSynthJustIntonationPlugInAudioProcessor::~PadSynthJustIntonationPlugInAudioProcessor()
{
}

//==============================================================================
const juce::String PadSynthJustIntonationPlugInAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PadSynthJustIntonationPlugInAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PadSynthJustIntonationPlugInAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PadSynthJustIntonationPlugInAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PadSynthJustIntonationPlugInAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PadSynthJustIntonationPlugInAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PadSynthJustIntonationPlugInAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PadSynthJustIntonationPlugInAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PadSynthJustIntonationPlugInAudioProcessor::getProgramName (int index)
{
    return {};
}

void PadSynthJustIntonationPlugInAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PadSynthJustIntonationPlugInAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    DBG("PadSynthJustIntonationPlugInAudioProcessor::prepareToPlay" << sampleRate << samplesPerBlock);

    this->synthesiser_.setCurrentPlaybackSampleRate(sampleRate);
    this->lastSampleRate_ = sampleRate;
    this->midiCollector.reset(sampleRate);

    this->synthesiser_.populate_synth(LayerSynth::populateSynthType::onenote);
    //this->synthesiser_.populate_synth(LayerSynth::populateSynthType::minimal);
}

void PadSynthJustIntonationPlugInAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PadSynthJustIntonationPlugInAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PadSynthJustIntonationPlugInAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //auto* channelData = buffer.getWritePointer (channel);

        //DBG("channel" << channel);
        // ..do something to the data...
    }
    //get the voice and get the params needed to make the voice
    for (int i = 0; i < this->synthesiser_.getNumVoices(); i++)
    {
        //if myVoice sucessfully casts as a SynthVoice*, get the voice and set the params
        if ((myVoice = dynamic_cast<PadSynthVoice*>(this->synthesiser_.getVoice(i)))) 
        {
            myVoice->setADSRSampleRate(this->lastSampleRate_);
            myVoice->getEnvelopeParams (this->apvts_.getRawParameterValue("attack"),
                                        this->apvts_.getRawParameterValue("decay"),
                                        this->apvts_.getRawParameterValue("sustain"),
                                        this->apvts_.getRawParameterValue("release"));
            //myVoice->getOscType(tree.getRawParameterValue("wavetype"));
            //myVoice->getOsc2Type(tree.getRawParameterValue("wavetype2"));

            //myVoice->getFilterParams(tree.getRawParameterValue("filterType"),
            //    tree.getRawParameterValue("filterCutoff"),
            //    tree.getRawParameterValue("filterRes"));

            //myVoice->getWillsParams(tree.getRawParameterValue("mastergain"),
            //    tree.getRawParameterValue("blend"),
            //    tree.getRawParameterValue("pbup"),
            //    tree.getRawParameterValue("pbdown"));
        }
    }

    float* channelDataLeft = buffer.getWritePointer(0);
    float* channelDataRight = buffer.getWritePointer(1);
    const int num_samples = buffer.getNumSamples();

    //int& index = this->pad_synth_.getCurrentIndex();
    //const size_t wavetable_size = this->pad_synth_.getWavetable().size();
    //const auto& wavetable = this->pad_synth_.getWavetable();
    
    int& index = this->synths_.at(0).getCurrentIndex();
    const size_t wavetable_size = this->synths_.at(0).getWavetable().size();
    const std::vector<float>& wavetable1 = this->synths_.at(1).getWavetable();
    const std::vector<float> &wavetable2 = this->synths_.at(0).getWavetable();
    float volume_track1 = *this->apvts_.getRawParameterValue("SLIDER0");
    float volume_track2 = *this->apvts_.getRawParameterValue("SLIDER1");
    for (int i = 0; i < num_samples; i++) {
        //channelDataLeft[i] = wavetable.at(index) * volume_track1_ / 2 + wavetable2.at(index) * volume_track2_ / 2;
        //channelDataRight[i] = wavetable.at(index) * volume_track1_ / 2 + wavetable2.at(index) * volume_track2_ / 2;
        channelDataLeft[i] = wavetable1.at(index) * volume_track1 / 2 + wavetable2.at(index) * volume_track2 / 2;
        channelDataRight[i] = wavetable1.at(index) * volume_track1 / 2 + wavetable2.at(index) * volume_track2 / 2;
        index = (index + 1) % (int)wavetable_size;
    }
    this->midiCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    this->keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    this->synthesiser_.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    //DBG("totalNumOutputChannels" << totalNumOutputChannels);
    //std::cout << "SLIDER" << volume_track1 << "SLIDER1" << volume_track2;

 /*   buffer.clear();

    juce::MidiBuffer processedMidi;

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;

        if (message.isNoteOn())
        {
            message = juce::MidiMessage::noteOn(message.getChannel(),
                message.getNoteNumber(),
                (juce::uint8)noteOnVel);
        }

        processedMidi.addEvent(message, time);
    }

    midiMessages.swapWith(processedMidi);*/
}

//==============================================================================
bool PadSynthJustIntonationPlugInAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PadSynthJustIntonationPlugInAudioProcessor::createEditor()
{
    return new PadSynthJustIntonationPlugInAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PadSynthJustIntonationPlugInAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PadSynthJustIntonationPlugInAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PadSynthJustIntonationPlugInAudioProcessor();
}
//==============================================================================
//juce::AudioProcessorValueTreeState::ParameterLayout PadSynthJustIntonationPlugInAudioProcessor::createParameters()
//{
//    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
//
//    //Float
//    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SLIDER", "Slider", 0.0f, 1.0f, 1.0f));
//
//    //Choice
//    parameters.push_back(std::make_unique<juce::AudioParameterChoice>("CHOICE", "Choice", juce::StringArray("Choice1", "Choice2", "Choice3"), 0));
//
//    //Bool
//    parameters.push_back(std::make_unique<juce::AudioParameterBool>("POWER", "Power", false));
//
//    //Int
//    parameters.push_back(std::make_unique<juce::AudioParameterInt>("DIAL", "Dial", 0, 10, 0));
//
//    return { parameters.begin(), parameters.end() };
//}

juce::AudioProcessorValueTreeState::ParameterLayout PadSynthJustIntonationPlugInAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //for (int i = 1; i < 9; ++i)
    //    layout.add(std::make_unique<AudioParameterInt>(String(i), String(i), 0, i, 0));

    //Float
    layout.add(std::make_unique<juce::AudioParameterFloat>("SLIDER0", "Slider0", 0.0f, 1.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("SLIDER1", "Slider1", 0.0f, 1.0f, 1.0f));
    //Choice
    layout.add(std::make_unique<juce::AudioParameterChoice>("CHOICE", "Choice", juce::StringArray("Choice1", "Choice2", "Choice3"), 0));

    //Bool
    layout.add(std::make_unique<juce::AudioParameterBool>("POWER", "Power", false));

    //Int
    layout.add(std::make_unique<juce::AudioParameterInt>("DIAL", "Dial", 0, 10, 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>("attack", "Attack", juce::NormalisableRange<float>(0.1f, 5.0f), .1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("decay", "Decay", juce::NormalisableRange<float>(0.1f, 2.0f), .8f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", juce::NormalisableRange<float>(0.1f, 1.0f), .8f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("release", "Release", juce::NormalisableRange<float>(0.1f, 5.0f), .1f));

    return layout;
}

//void PadSynthJustIntonationPlugInAudioProcessor::populate_synth(PadSynthJustIntonationPlugInAudioProcessor::populateSynthType populate_synth_type)
//{
//    auto populate_synth = [this] (size_t lowest_midi_note_number, size_t highest_midi_note_number) {
//        int sample_rate = (int)this->lastSampleRate_;
//        double nb_seconds = .5;
//        int N = (int)(sample_rate * nb_seconds);
//        int number_harmonics = 64;
//        float fundamental_frequency;
//        float base_frequency;
//        auto start = std::chrono::high_resolution_clock::now();
//        this->synth_sounds_.reserve(highest_midi_note_number - lowest_midi_note_number);
//        for (int midi_note_number = lowest_midi_note_number; midi_note_number != highest_midi_note_number; ++midi_note_number) {
//            fundamental_frequency = (float)juce::MidiMessage::getMidiNoteInHertz(midi_note_number);
//            base_frequency = fundamental_frequency;
//            //this->synth_sounds_.push_back(PadSynthSound(N, sample_rate, number_harmonics, fundamental_frequency, base_frequency, midi_note_number));
//            this->synthesiser_.addSound(new PadSynthSound(N, sample_rate, number_harmonics, fundamental_frequency, base_frequency, midi_note_number));
//            DBG("this->synth_sounds_.push_back" << " midi_note_number " << midi_note_number);
//        }
//        auto stop = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//        DBG("Time taken by PadSynthJustIntonationPlugInAudioProcessor::populate_synth: " << duration.count() * .000001 << " sec");
//    };
//    size_t lowest_midi_note_number;
//    size_t highest_midi_note_number;
//    switch (populate_synth_type) {
//    case populateSynthType::minimal:
//        lowest_midi_note_number = 60;//30;
//        highest_midi_note_number = 72;// 80;
//        populate_synth(lowest_midi_note_number, highest_midi_note_number);
//        break;
//
//    case populateSynthType::complete:
//        lowest_midi_note_number = 8;
//        highest_midi_note_number = 96;
//        populate_synth(lowest_midi_note_number, highest_midi_note_number);
//        break;
//    }
//}
