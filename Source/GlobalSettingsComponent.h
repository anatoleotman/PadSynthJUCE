/*
  ==============================================================================

    GlobalSettingsComponent.h
    Created: 6 May 2021 8:11:07pm
    Author:  jonny

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
/*
*/
class GlobalSettingsComponent  : public juce::Component, juce::Button::Listener
{
public:
    GlobalSettingsComponent(PadSynthJustIntonationPlugInAudioProcessor& audio_processor);
    ~GlobalSettingsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    std::function<void()> onUpdate;

private:
    ADSRComponent ADSRComponent_;
    juce::ToggleButton populate_button_;
    //std::vector<juce::Component*> members_;

    PadSynthJustIntonationPlugInAudioProcessor& audio_processor_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalSettingsComponent)
};
