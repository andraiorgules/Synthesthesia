/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"


//==============================================================================
/**
*/
class SynthesthesiaAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthesthesiaAudioProcessorEditor (SynthesthesiaAudioProcessor&);
    ~SynthesthesiaAudioProcessorEditor() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    
private:
    SynthesthesiaAudioProcessor& audioProcessor;
    OscComponent osc;
    ADSRComponent adsr;
    FilterComponent filter;
    ADSRComponent modAdsr;

    juce::TextButton button {"Filters"};
    juce::TextButton button2 {"Frequency Modulation"};
    juce::TextButton button3 {"Sound Waves"};
    juce::TextButton button4 {"Envelope"};
    juce::TextButton button5 {"Modulation Envelope"};
    std::unique_ptr<juce::AlertWindow> alertWindow;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthesthesiaAudioProcessorEditor)
};
