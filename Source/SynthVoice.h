/*
  ==============================================================================

    SynthVoice.h
    Created: 1 Mar 2023 12:44:50am
    Author:  andrada

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/OscData.h"
#include "Data/ADSRData.h"
#include "Data/FilterData.h"


class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    
    void reset();
    
    void updateAdsr (const float attack, const float decay, const float sustain, const float release);
    void updateFilter (const int filterType, const float cutoff, const float resonance);
    void updateModAdsr (const float attack, const float decay, const float sustain, const float release);
    
    OscData& getOscillator() { return osc; }

private:
    juce::AudioBuffer<float> synthBuffer;
    
    OscData osc;
    ADSRData adsr;
    FilterData filter;
    ADSRData modAdsr;
    juce::dsp::Gain<float> gain;
    
    bool isPrepared { false };
    
 
    
};
