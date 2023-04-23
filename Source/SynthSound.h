/*
  ==============================================================================

    SynthSound.h
    Created: 1 Mar 2023 12:45:06am
    Author:  andrada

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override{ return true; }
    bool appliesToChannel (int midiChannel) override{ return true; }
    
};
