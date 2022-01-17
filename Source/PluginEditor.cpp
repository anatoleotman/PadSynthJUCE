/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PadSynthJustIntonationPlugInAudioProcessorEditor::PadSynthJustIntonationPlugInAudioProcessorEditor (PadSynthJustIntonationPlugInAudioProcessor& p) : 
      AudioProcessorEditor (p), 
      audioProcessor (p),
      //ADSRComponent_ (p),
      global_settings_component_(p),
      pad_synth_UI_component_ (p),
      midiKeyboardComponent(this->audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      midiKeyboardComponentBaseOctave_(6),
      single_note_settings_component_ (p, &visualisation_component_),
      visualisation_component_(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 400);
    // these define the parameters of our slider object

    // this function adds the slider to the editor
    //this->addAndMakeVisible(&ADSRComponent_);

    this->addAndMakeVisible(&this->global_settings_component_);

    //TODO 
    this->global_settings_component_.onUpdate = [this]() { this->single_note_settings_component_.setMidiNoteRange(8, 96); };

    this->addAndMakeVisible(&pad_synth_UI_component_);

    this->addAndMakeVisible(&midiInputListLabel);
    midiInputListLabel.setText("MIDI Input:", juce::dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);

    auto midiInputs = juce::MidiInput::getAvailableDevices();
    this->addAndMakeVisible(&midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");

    juce::StringArray midiInputNames;
    for (auto input : midiInputs) {
        midiInputNames.add(input.name);
    }
    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };
    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled(input.identifier))
        {
            setMidiInput(midiInputs.indexOf(input));
            break;
        }
    }
    if (midiInputList.getSelectedId() == 0) {
        setMidiInput(0);
    }


    this->midiKeyboardComponent.setKeyPressForNote(juce::KeyPress('q'), 0);
    this->midiKeyboardComponent.setKeyPressForNote(juce::KeyPress('z'), 1);
    this->midiKeyboardComponent.setKeyPressForNote(juce::KeyPress('m'), 16);
    //this->midiKeyboardComponent.setWantsKeyboardFocus(true);
    this->addAndMakeVisible(&this->midiKeyboardComponent);

    this->addAndMakeVisible(&this->single_note_settings_component_);
    //this->slider1_attachement_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts_, "SLIDER1", this->volume_track2);
    
    // add the listener to the slider
    //volume_track1.addListener(this);
    //this->volume_track2.addListener(this);

    this->addAndMakeVisible(&this->visualisation_component_);

    this->setResizable(true, true);
    this->setResizeLimits(20, 20, 1000, 600);
}

PadSynthJustIntonationPlugInAudioProcessorEditor::~PadSynthJustIntonationPlugInAudioProcessorEditor()
{

}

//==============================================================================
void PadSynthJustIntonationPlugInAudioProcessorEditor::paint (juce::Graphics& g)
{
    //// (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    // fill the whole window white
    //g.fillAll(juce::Colours::white);
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    // set the current drawing colour to black
    //g.setColour(juce::Colours::white);

    //// set the font size and draw text to the screen
    //g.setFont(15.0f);

    //g.drawFittedText("Volume", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void PadSynthJustIntonationPlugInAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // sets the position and size of the slider with arguments (x, y, width, height)
    juce::Rectangle<int> area = this->getLocalBounds();
    juce::Rectangle<int> area1 = area.removeFromTop(200);

    //this->ADSRComponent_.setBounds(area1.removeFromLeft(200));
    this->global_settings_component_.setBounds(area1.removeFromLeft(100));
    this->pad_synth_UI_component_.setBounds(area1.removeFromRight(20));
    this->single_note_settings_component_.setBounds(area1.removeFromLeft(200));
    this->visualisation_component_.setBounds(area1.removeFromLeft(400));

    midiInputList.setBounds(area.removeFromTop(20));// .withTrimmedLeft(40));
    this->midiKeyboardComponent.setBounds(area.removeFromTop(200));
}

bool PadSynthJustIntonationPlugInAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    //DBG("PadSynthJustIntonationPlugInAudioProcessorEditor::keyPressed: " << key.getTextCharacter());
    bool answer = true;;
    switch (key.getTextCharacter())
    {
    case 'w':
        this->midiKeyboardComponent.setKeyPressBaseOctave(--this->midiKeyboardComponentBaseOctave_);
        break;
    case 'x':
        this->midiKeyboardComponent.setKeyPressBaseOctave(++this->midiKeyboardComponentBaseOctave_);
        break;
    default:
        answer = false;
        break;
    }
    return answer;
    //if (key.getTextCharacter() == 'w') {
    //    this->midiKeyboardComponent.setKeyPressBaseOctave(--this->midiKeyboardComponentBaseOctave_);
    //    return true;
    //}
    //else
    //{
    //    return false;
    //}
}

void PadSynthJustIntonationPlugInAudioProcessorEditor::setMidiInput(int index)
{
    auto list = juce::MidiInput::getAvailableDevices();
    
    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, &audioProcessor.midiCollector); // [12]

    auto newInput = list[index];

    if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

    deviceManager.addMidiInputDeviceCallback(newInput.identifier, &audioProcessor.midiCollector); // [13]
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);

    lastInputIndex = index;
}
