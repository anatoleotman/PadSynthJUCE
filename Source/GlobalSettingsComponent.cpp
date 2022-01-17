/*
  ==============================================================================

    GlobalSettingsComponent.cpp
    Created: 6 May 2021 8:11:07pm
    Author:  jonny

  ==============================================================================
*/

#include <JuceHeader.h>
#include <future>
#include "GlobalSettingsComponent.h"
#include "LayerSynth.h"

//==============================================================================
GlobalSettingsComponent::GlobalSettingsComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor) :
    ADSRComponent_(audio_processor),
    audio_processor_(audio_processor)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    this->addAndMakeVisible(this->ADSRComponent_);

    this->populate_button_.setButtonText("buildSynths");
    //this->populate_button_.setClickingTogglesState(true);
    this->addAndMakeVisible(&this->populate_button_);
    this->populate_button_.addListener(this);
}

GlobalSettingsComponent::~GlobalSettingsComponent()
{
}

void GlobalSettingsComponent::paint (juce::Graphics& g)
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
    //g.drawText ("GlobalSettingsComponent", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void GlobalSettingsComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //juce::Rectangle<int> area = this->getLocalBounds();
    //this->ADSRComponent_.setBounds(area.removeFromLeft(200));

    //float nb_members = 2.f;
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    //for (juce::Component* member : this->members_) {
    //    fb.items.add(juce::FlexItem(*member).withFlex(0, 1, (float)getWidth() / this->members_.size()));
    //}
    fb.items.add(juce::FlexItem(this->populate_button_).withFlex(0, 1, 40));
    fb.items.add(juce::FlexItem(this->ADSRComponent_).withFlex(0, 1, 200));
    fb.performLayout(this->getLocalBounds().toFloat());
}

void GlobalSettingsComponent::buttonClicked(juce::Button* button)
{
    if (button == &this->populate_button_) {
        if (this->populate_button_.getToggleState()) {
            //DBG("GlobalSettingsComponent::buttonClicked button == &this->populate_button_ == true");
            this->audio_processor_.getSynthesiser().populate_synth(LayerSynth::populateSynthType::complete);
            this->onUpdate();
        }
        else {

        }
    }
}
