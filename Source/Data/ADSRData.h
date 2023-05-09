/*
  ==============================================================================

    ADSRData.h
    Created: 23 Apr 2023 7:05:16pm
    Author:  andiwolf

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ADSRData : public juce::ADSR
{
public:
    void updateADSR (const float attack, const float decay, const float sustain, const float release);
    
private:
    juce::ADSR::Parameters adsrParams;
    
};
