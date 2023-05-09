/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthesthesiaAudioProcessorEditor::SynthesthesiaAudioProcessorEditor (SynthesthesiaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), osc(audioProcessor.apvts, "OSC1WAVETYPE", "OSC1FMFREQ", "OSC1FMDEPTH"), adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"), filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES"), modAdsr("Mod Envelope", audioProcessor.apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE")
{
    setSize (1300, 1000);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
    addAndMakeVisible(modAdsr);
}

SynthesthesiaAudioProcessorEditor::~SynthesthesiaAudioProcessorEditor()
{
}

//==============================================================================
void SynthesthesiaAudioProcessorEditor::paint (juce::Graphics& g)
{
    //Sets background to black
    g.fillAll(juce::Colours::black);
}

void SynthesthesiaAudioProcessorEditor::resized()
{
    osc.setBounds(10, 400, getWidth() / 4, 200);
    adsr.setBounds(10, 600, getWidth() / 4, 175);
    filter.setBounds(10, 775, getWidth() / 4, 175);
    modAdsr.setBounds(10, 225, getWidth() / 4, 175);
}


