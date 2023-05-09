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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthesthesiaAudioProcessorEditor)
};
