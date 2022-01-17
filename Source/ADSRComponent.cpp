/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 10 Apr 2021 5:12:23pm
    Author:  jonny

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor) :
    processor(audio_processor)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    //setSize(200, 200);
    
        //slider initialization values
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    attackSlider.setRange(0.1f, 5.0f);
    attackSlider.setValue(0.1f);
    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    this->attackSlider.setPopupDisplayEnabled(true, true, nullptr);
    this->attackSlider.setTextValueSuffix(" attack");
    addAndMakeVisible(&attackSlider);
    
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    decaySlider.setRange(0.1f, 2.0f);
    decaySlider.setValue(0.8f);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    this->decaySlider.setPopupDisplayEnabled(true, true, nullptr);
    this->decaySlider.setTextValueSuffix(" decay");
    addAndMakeVisible(&decaySlider);
    
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    sustainSlider.setRange(0.1f, 1.0f);
    sustainSlider.setValue(0.8f);
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    this->sustainSlider.setPopupDisplayEnabled(true, true, nullptr);
    this->sustainSlider.setTextValueSuffix(" sustain");
    addAndMakeVisible(&sustainSlider);
    
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    releaseSlider.setRange(0.1f, 5.0f);
    releaseSlider.setValue(0.8f);
    releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    this->releaseSlider.setPopupDisplayEnabled(true, true, nullptr);
    this->releaseSlider.setTextValueSuffix(" release");
    addAndMakeVisible(&releaseSlider);

    this->members_.push_back(&this->attackSlider);
    this->members_.push_back(&this->decaySlider);
    this->members_.push_back(&this->sustainSlider);
    this->members_.push_back(&this->releaseSlider);
    
    //sends value of the sliders to the tree state in the processor
    attackVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.apvts_, "attack", attackSlider);
    decayVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.apvts_, "decay", decaySlider);
    sustainVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.apvts_, "sustain", sustainSlider);
    releaseVal = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.apvts_, "release", releaseSlider);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{             
    //fancy stuff for the UI background etc
    //juce::Rectangle<int> titleArea (0, 10, getWidth(), 20);
    

    //g.fillAll (juce::Colours::black);
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
    //g.setColour(juce::Colours::white);
    //g.drawText("Envelope", titleArea, juce::Justification::centredTop);
    
    //static positioning for now due to time, make dynamic later/
    //g.drawText ("A", 53, 150, 20, 20, juce::Justification::centredTop);
    //g.drawText ("D", 77, 150, 20, 20, juce::Justification::centredTop);
    //g.drawText ("S", 103, 150, 20, 20, juce::Justification::centredTop);
    //g.drawText ("R", 128, 150, 20, 20, juce::Justification::centredTop);
    
    //juce::Rectangle <float> area (25, 25, 150, 150);
    //
    //g.setColour(juce::Colours::yellow);
    //g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void ADSRComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //draws the sliders...we use a rectangle object to dynamically size the UI (if we want to resize for IPad etc without needing to change ALL settings
    //juce::Rectangle<int> area = getLocalBounds().reduced(10);
    //
    //int sliderWidth = 25;
    //int sliderHeight = 175;
    //
    ////draw sliders by reducing area from rectangle above
    //attackSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    //decaySlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    //sustainSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
    //releaseSlider.setBounds (area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));

    float nb_members = 4.f;
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    for (juce::Component* member : this->members_) {
        fb.items.add(juce::FlexItem(*member).withFlex(0, 1, (float)getWidth() / this->members_.size()));
    }
    fb.performLayout(this->getLocalBounds().toFloat());
}
