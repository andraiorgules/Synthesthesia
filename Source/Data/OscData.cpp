/*
  ==============================================================================

    OscData.cpp
    Created: 23 Apr 2023 9:52:52pm
    Author:  andiwolf

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    fmOsc.prepare(spec);
    prepare (spec);
}

void OscData::setWaveType(const int choice)
{
    switch (choice)
    {
        case 0:
            //Sine Wave
            initialise ([](float x) { return std::sin (x); });
            break;
            
        case 1:
            //Sawtooth Wave
            initialise ([](float x) { return x / juce::MathConstants<float>::pi; });
            break;
            
        case 2:
            //Square Wave
            initialise ([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
            
        // case 3:
            //User can load their own samples
            //Choose File..
            //break;
            
        default:
            jassertfalse; //activates if the plugin breaks basically
            break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    //can add pitch parameter here
    setFrequency(juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock (juce::dsp::AudioBlock<float> block)
{
    for (int ch =0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            fmMod = fmOsc.processSample (block.getSample(ch, s)) * fmDepth;
        }
    }
    
    process (juce::dsp::ProcessContextReplacing<float> (block));
}

void OscData::setFmParams (const float depth, const float freq)
{
    fmOsc.setFrequency (freq);
    fmDepth = depth;
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + fmMod;
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}
