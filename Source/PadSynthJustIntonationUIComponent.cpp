/*
  ==============================================================================

    PadSynthJustIntonationUIComponent.cpp
    Created: 27 Mar 2021 8:49:37pm
    Author:  jonny

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PadSynthJustIntonationUIComponent.h"

//==============================================================================
PadSynthJustIntonationUIComponent::PadSynthJustIntonationUIComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor):
    audio_processor_(audio_processor)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    this->volume_slider0_.setSliderStyle(juce::Slider::LinearBarVertical);
    this->volume_slider0_.setRange(0.0, 1.0, .01);
    this->volume_slider0_.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    this->volume_slider0_.setPopupDisplayEnabled(true, false, nullptr);
    this->volume_slider0_.setTextValueSuffix(" Vol0.");
    this->volume_slider0_.setValue(1.0);
    this->addAndMakeVisible(&this->volume_slider0_);

    volume_slider1_.setSliderStyle(juce::Slider::LinearBarVertical);
    volume_slider1_.setRange(0.0, 1.0, .01);
    volume_slider1_.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    volume_slider1_.setPopupDisplayEnabled(true, false, nullptr);
    volume_slider1_.setTextValueSuffix(" Vol1.");
    volume_slider1_.setValue(1.0);
    addAndMakeVisible(&this->volume_slider1_);

    this->members_.push_back(&this->volume_slider0_);
    this->members_.push_back(&this->volume_slider1_);

    this->slider_attachement0_ = new juce::AudioProcessorValueTreeState::SliderAttachment(this->audio_processor_.apvts_, "SLIDER0", this->volume_slider0_);
    this->slider_attachement1_ = new juce::AudioProcessorValueTreeState::SliderAttachment(this->audio_processor_.apvts_, "SLIDER1", this->volume_slider1_);
}

PadSynthJustIntonationUIComponent::~PadSynthJustIntonationUIComponent()
{
}

void PadSynthJustIntonationUIComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    //g.fillAll(juce::Colours::white);

    //g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("PadSynthJustIntonationUIComponent", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void PadSynthJustIntonationUIComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //auto area_global = getLocalBounds();
    //auto area = area_global.removeFromLeft(20);
    //auto area = getLocalBounds();
    //this->volume_slider0_.setBounds(area.removeFromTop(100));
    //volume_slider1_.setBounds(area.removeFromTop(100));

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    for (juce::Component* member : this->members_) {
        fb.items.add(juce::FlexItem(*member).withFlex(0, 1, (float)getHeight() / this->members_.size()));
    }
    fb.performLayout(this->getLocalBounds().toFloat());
}
