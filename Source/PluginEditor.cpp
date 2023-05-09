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
    
    addAndMakeVisible(audioProcessor.waveViewer);
    audioProcessor.waveViewer.setColours(juce::Colours::black, juce::Colours::pink);
    
    button.setClickingTogglesState(true);
    button.onClick = [&]()
    {
        const auto message = button.getToggleState() ? "More Info:" : "Filters";
        printf(message);
        button.setButtonText(message);
        
        alertWindow = std::make_unique<juce::AlertWindow>("Filters", "Filters are used to manipulate the frequency content of a sound. Low-pass filters allow frequencies below the cutoff point to pass through, while frequencies above get quieter. High-pass filters allow frequencies above the cutoff point to pass through, while frequencies below get quieter. Band-pass filters allow frequencies within the chosen range to pass through, while quieting all frequencies outside of that range.", juce::MessageBoxIconType::InfoIcon, nullptr);
        alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, "Filters", "Filters are used to manipulate the frequency content of a sound. Low-pass filters allow frequencies below the cutoff point to pass through, while frequencies above get quieter. High-pass filters allow frequencies above the cutoff point to pass through, while frequencies below get quieter. Band-pass filters allow frequencies within the chosen range to pass through, while quieting all frequencies outside of that range.");
    };
    addAndMakeVisible(button);
    
    button2.setClickingTogglesState(true);
    button2.onClick = [&]()
    {
        const auto message = button2.getToggleState() ? "More Info:" : "Frequency Modulation";
        printf(message);
        button2.setButtonText(message);
        
        alertWindow = std::make_unique<juce::AlertWindow>("Frequency Modulation", "Frequency Modulation, or FM Synthesis, is when an oscillator is used to generate a carrier signal that is shifted up and down by a modulating signal (usually another oscillator)", juce::MessageBoxIconType::InfoIcon, nullptr);
        alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, "Frequency Modulation", "Frequency Modulation, or FM Synthesis, is when an oscillator is used to generate a carrier signal that is shifted up and down by a modulating signal (usually another oscillator)");
    };
    addAndMakeVisible(button2);
    
    button3.setClickingTogglesState(true);
    button3.onClick = [&]()
    {
        const auto message = button3.getToggleState() ? "More Info:" : "Sound Waves";
        printf(message);
        button3.setButtonText(message);
        
        alertWindow = std::make_unique<juce::AlertWindow>("Sound Waves", "Sine Wave: A pure tone of a single frequency, with no harmonics. Sawtooth Wave: Made up of a series of harmonics that decrease in amplitude as frequency increases. Square Wave: Alternaates between 2 fixed amplitudes, resulting in a sound that can be described as 'buzzy' or 'beeping' ", juce::MessageBoxIconType::InfoIcon, nullptr);
        alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, "Sound Waves",  "Sine Wave: A pure tone of a single frequency, with no harmonics. Sawtooth Wave: Made up of a series of harmonics that decrease in amplitude as frequency increases. Square Wave: Alternaates between 2 fixed amplitudes, resulting in a sound that can be described as 'buzzy' or 'beeping' ");
    };
    addAndMakeVisible(button3);
    
    button4.setClickingTogglesState(true);
    button4.onClick = [&]()
    {
        const auto message = button4.getToggleState() ? "More Info:" : "Envelope";
        printf(message);
        button4.setButtonText(message);
        
        alertWindow = std::make_unique<juce::AlertWindow>("Envelope (ADSR Component)", "Shapes the amplitude of a sound over time by controlling how quickly the sound starts, how quickly it fades out, and how long it remains at a steady level.", juce::MessageBoxIconType::InfoIcon, nullptr);
        alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, "Envelope",  "Shapes the amplitude of a sound over time by controlling how quickly the sound starts, how quickly it fades out, and how long it remains at a steady level.");
    };
    addAndMakeVisible(button4);
    
    button5.setClickingTogglesState(true);
    button5.onClick = [&]()
    {
        const auto message = button5.getToggleState() ? "More Info:" : "Modulation Envelope";
        printf(message);
        button5.setButtonText(message);
        
        alertWindow = std::make_unique<juce::AlertWindow>("Modulation Envelope", "An envelope generator that is used to modulate one or more parameters of a sound over time, including (but not limited to): frequency, amplitude, or filter cutoff. This is used to control the parameter being modulated, and results in a dynamic and evolving sound.", juce::MessageBoxIconType::InfoIcon, nullptr);
        alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, "Modulating Envelope",  "An envelope generator that is used to modulate one or more parameters of a sound over time, including (but not limited to): frequency, amplitude, or filter cutoff. This is used to control the parameter being modulated, and results in a dynamic and evolving sound.");
    };
    addAndMakeVisible(button5);
    
    //addAndMakeVisible(audioProcessor.delay);
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
    osc.setBounds(750, 600, getWidth() / 3, 200);
    adsr.setBounds(100, 600, getWidth() / 3, 200);
    filter.setBounds(100, 775, getWidth() / 3, 200);
    modAdsr.setBounds(750, 775, getWidth() / 3, 200);
    
    audioProcessor.waveViewer.setBounds(10, 100, getWidth(), 300);
    
    button.setBounds(400, 780, getWidth() / 10, 20);
    button2.setBounds(755, 700, getWidth() / 10, 20);
    button3.setBounds(755, 680, getWidth() / 10, 20);
    button4.setBounds(105, 605, getWidth() / 10, 20);
    button5.setBounds(755, 780, getWidth() / 10, 20);
    
    //audioProcessor.delay.setBounds(550, 775, getWidth() / 4, 175);
}


