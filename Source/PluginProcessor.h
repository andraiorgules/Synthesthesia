/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
/**
*/
class SynthesthesiaAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    SynthesthesiaAudioProcessor();
    ~SynthesthesiaAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts;

    juce::AudioVisualiserComponent waveViewer;
    
private:
    juce::Synthesiser synth;
    
    //static constexpr auto effectDelaySamples = 192000;
     //  juce::dsp::DelayLine<float> delay { effectDelaySamples };
    //   juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> linear { effectDelaySamples };
     //  juce::dsp::DryWetMixer<float> mixer;
       
       //std::array<float, 2> delayValue { {} };
       //std::array<float, 2> lastDelayOutput;
      // std::array<LinearSmoothedValue<float>, 2> delayFeedbackVolume;
       
      // void parameterChanged (const String& parameterID, float newValue) override;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    /*
    juce::AudioBuffer<float> delayBuffer;
    int writePosition {0};
    
    
    void fillBuffer(juce::AudioBuffer<float>& buffer, int channel);
    void readFromBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel);
    void updateBufferPositions(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer);
     */
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthesthesiaAudioProcessor)
};
