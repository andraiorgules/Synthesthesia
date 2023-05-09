/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthesthesiaAudioProcessor::SynthesthesiaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParams())
                        , waveViewer(1)
#endif
{
    synth.addSound ( new SynthSound() );
    for (int i = 0; i < 5; i++)
      {
          synth.addVoice (new SynthVoice());
      }

    waveViewer.setRepaintRate(30);
    waveViewer.setBufferSize(512);
    
    //Delay stuff
   // apvts.addParameterListener ("RATE", this);
   // apvts.addParameterListener ("FEEDBACK", this);
    //apvts.addParameterListener ("MIX", this);
}

SynthesthesiaAudioProcessor::~SynthesthesiaAudioProcessor()
{
   // apvts.removeParameterListener ("RATE", this);
   // apvts.removeParameterListener ("FEEDBACK", this);
   // apvts.removeParameterListener ("MIX", this);
}

//==============================================================================
const juce::String SynthesthesiaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthesthesiaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthesthesiaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthesthesiaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthesthesiaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthesthesiaAudioProcessor::getNumPrograms()
{
    return 1;   
                
}

int SynthesthesiaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthesthesiaAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthesthesiaAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthesthesiaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthesthesiaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
    
    /*
    //Circular Buffer
    auto delayBufferSize = sampleRate * 2.0f;
    delayBuffer.setSize(getTotalNumOutputChannels(), (int)delayBufferSize);
    
    
    juce::dsp::ProcessSpec spec;
       spec.maximumBlockSize = samplesPerBlock;
       spec.sampleRate = sampleRate;
       spec.numChannels = 2;
       
       delay.prepare (spec);
       linear.prepare (spec);
       mixer.prepare (spec);
       
       for (auto& volume : delayFeedbackVolume)
           volume.reset (spec.sampleRate, 0.05);
       
       linear.reset();
       std::fill (lastDelayOutput.begin(), lastDelayOutput.end(), 0.0f);
    */
    
}

void SynthesthesiaAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthesthesiaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthesthesiaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            //Amplitude ADSR for Osc
            auto& attack = *apvts.getRawParameterValue ("ATTACK");
            auto& decay = *apvts.getRawParameterValue ("DECAY");
            auto& sustain = *apvts.getRawParameterValue ("SUSTAIN");
            auto& release = *apvts.getRawParameterValue ("RELEASE");
            
            //Oscillator 1
            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& fmDepth = *apvts.getRawParameterValue("OSC1FMDEPTH");
            auto& fmFreq = *apvts.getRawParameterValue("OSC1FMFREQ");
            
            //Filter Code
            auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
            auto& cutoff = *apvts.getRawParameterValue("FILTERCUTOFF");
            auto& resonance = *apvts.getRawParameterValue("FILTERRES");
            
            //Modulator ADSR
            auto& modAttack = *apvts.getRawParameterValue ("MODATTACK");
            auto& modDecay = *apvts.getRawParameterValue ("MODDECAY");
            auto& modSustain = *apvts.getRawParameterValue ("MODSUSTAIN");
            auto& modRelease = *apvts.getRawParameterValue ("MODRELEASE");
            
            //Delay
            //auto& delay = *apvts.getRawParameterValue ("DELAY");
            
            //Update Parameters
            voice->getOscillator().setWaveType (oscWaveChoice);
            voice->getOscillator().setFmParams(fmDepth, fmFreq);
            voice->updateAdsr(attack.load(), decay.load(), sustain.load(), release.load());
            voice->updateFilter(filterType.load(), cutoff.load(), resonance.load());
            voice->updateModAdsr(modAttack, modDecay, modSustain, modRelease);
            //voice->updateDelay(delay);
        }
    }
    
    
    buffer.clear();
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    //Drawing Sound waves
    waveViewer.pushBuffer(buffer);
    
    /*
    //Circular Buffer
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        fillBuffer(buffer, channel);
        readFromBuffer(buffer, delayBuffer, channel);
        //create feedback loop
        fillBuffer(buffer, channel);
    }
    
    updateBufferPositions (buffer, delayBuffer);
    
    
    const auto numChannels = jmax (totalNumInputChannels, totalNumOutputChannels);

        auto audioBlock = juce::dsp::AudioBlock<float> (buffer).getSubsetChannelBlock (0, (size_t) numChannels);
        auto context = juce::dsp::ProcessContextReplacing<float> (audioBlock);
        const auto& input = context.getInputBlock();
        const auto& output = context.getOutputBlock();
        
        mixer.pushDrySamples (input);
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* samplesIn = input.getChannelPointer (channel);
            auto* samplesOut = output.getChannelPointer (channel);
            
            for (size_t sample = 0; sample < input.getNumSamples(); ++sample)
            {
                auto input = samplesIn[sample] - lastDelayOutput[channel];
                auto delayAmount = delayValue[channel];

                linear.pushSample (int (channel), input);
                linear.setDelay ((float) delayAmount);
                samplesOut[sample] = linear.popSample ((int) channel);
                            
                lastDelayOutput[channel] = samplesOut[sample] * delayFeedbackVolume[channel].getNextValue();
            }
        }
        
        mixer.mixWetSamples (output);
    */
}

/*
void SynthesthesiaAudioProcessor::fillBuffer(juce::AudioBuffer<float>& buffer, int channel)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    
    auto* channelData = buffer.getWritePointer(channel);
    
    if (delayBufferSize > bufferSize + writePosition)
    {
        delayBuffer.copyFrom(channel, writePosition, buffer.getWritePointer(channel), bufferSize);
    }
    else
    {
        auto numSamplesToEnd = delayBufferSize - writePosition;
        
        delayBuffer.copyFrom(channel, writePosition, buffer.getWritePointer(channel), numSamplesToEnd);
        
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        
        delayBuffer.copyFrom(channel, 0, buffer.getWritePointer(channel, numSamplesToEnd), numSamplesAtStart);
    }
}
*/
/*
void SynthesthesiaAudioProcessor::readFromBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    
    auto readPosition = writePosition - (getSampleRate() * 0.5f);
    
    if (readPosition < 0)
    {
        readPosition += delayBufferSize;
    }

    auto g = 0.7f;
    
    if (readPosition + bufferSize < delayBufferSize)
    {
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, g, g);
    }
    else
    {
        auto numSamplesToEnd = delayBufferSize - readPosition;
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSamplesToEnd, g, g);
        
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        buffer.addFromWithRamp(channel, numSamplesToEnd, delayBuffer.getReadPointer(channel, 0), numSamplesAtStart, g, g);
    }
}
*/
/*
void SynthesthesiaAudioProcessor::updateBufferPositions(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    
    writePosition += bufferSize;
    writePosition %= delayBufferSize;
}
*/
//==============================================================================
bool SynthesthesiaAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SynthesthesiaAudioProcessor::createEditor()
{
    return new SynthesthesiaAudioProcessorEditor (*this);
}

//==============================================================================
void SynthesthesiaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void SynthesthesiaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthesthesiaAudioProcessor();
}
     
/*
void BasicDelayAudioProcessor::parameterChanged (const String& parameterID, float newValue)
{
    if (parameterID == "RATE")
        std::fill (delayValue.begin(), delayValue.end(), newValue / 1000.0 * getSampleRate());
    
    if (parameterID == "MIX")
        mixer.setWetMixProportion (newValue);
    
    if (parameterID == "FEEDBACK")
    {
        const auto feedbackGain = Decibels::decibelsToGain (newValue, -100.0f);
        
        for (auto& volume : delayFeedbackVolume)
            volume.setTargetValue (feedbackGain);
    }
}
*/

juce::AudioProcessorValueTreeState::ParameterLayout SynthesthesiaAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //Frequency Modulation
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC1FMFREQ", "Osc 1 FM Frequency", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f  }, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC1FMDEPTH", "Osc 1 FM Depth", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f }, 0.0f));

    //ADSR
    params.push_back (std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> { 0.1f, 1.0f, 0.1f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> { 0.1f, 1.0f, 0.1f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> { 0.1f, 1.0f, 0.1f }, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> { 0.1f, 3.0f, 0.1f }, 0.4f));
                     
    //Oscillator 1 Selection
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray { "Sine", "Sawtooth", "Square" }, 0));
    
    //Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray { "Low-Pass", "Band-Pass", "High-Pass" }, 0));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f }, 20000.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float> { 0.1f, 2.0f, 0.1f }, 0.1f));
    
    //Modulator ADSR
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODATTACK", "Mod Attack", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.1f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODDECAY", "Mod Decay", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.1f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODSUSTAIN", "Mod Sustain", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.1f }, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODRELEASE", "Mod Release", juce::NormalisableRange<float> { 0.0f, 3.0f, 0.1f }, 0.1f));
    
    //Delay
    //params.push_back (std::make_unique<juce::AudioParameterFloat>("DELAY", "Delay", juce::NormalisableRange<float> { 0.0f, 3.0f, 0.1f }, 0.1f));
   // using Range = NormalisableRange<float>;
    //params.add (std::make_unique<AudioParameterFloat>   ("RATE", "Rate", 0.01f, 1000.0f, 0));
    //params.add (std::make_unique<AudioParameterFloat> ("FEEDBACK", "Feedback", -100.0f, 0.0f, -100.0f));
    //params.add (std::make_unique<AudioParameterFloat> ("MIX", "Mix", Range { 0.0f, 1.0f, 0.01f }, 0.0f));
    
    
    return { params.begin(), params.end() };
}
