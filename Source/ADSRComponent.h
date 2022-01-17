/*
  ==============================================================================

    ADSRComponent.h
    Created: 10 Apr 2021 5:12:23pm
    Author:  jonny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainVal;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseVal;
    
    std::vector<juce::Component*> members_;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PadSynthJustIntonationPlugInAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
