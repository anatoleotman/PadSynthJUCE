/*
  ==============================================================================

    PadSynthJustIntonationUIComponent.h
    Created: 27 Mar 2021 8:49:37pm
    Author:  jonny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================
/*
*/
class PadSynthJustIntonationUIComponent  : public juce::Component
{
public:
    PadSynthJustIntonationUIComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor);
    ~PadSynthJustIntonationUIComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PadSynthJustIntonationPlugInAudioProcessor& audio_processor_;
    juce::Slider volume_slider1_;
    juce::Slider volume_slider0_;
    std::vector<juce::Component*> members_;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> slider_attachement0_;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> slider_attachement1_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadSynthJustIntonationUIComponent)
};
