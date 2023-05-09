/*
  ==============================================================================

    OscComponent.cpp
    Created: 23 Apr 2023 10:46:16pm
    Author:  andiwolf

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId)
{
    juce::StringArray choices { "Sine", "Sawtooth", "Square" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible (oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);
    
    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::pink);
    waveSelectorLabel.setFont(15.0f);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(waveSelectorLabel);
    
    setSliderWithLabel(fmFreqSlider, fmFreqLabel, apvts, fmFreqId, fmFreqAttachment);
    setSliderWithLabel(fmDepthSlider, fmDepthLabel, apvts, fmDepthId, fmDepthAttachment);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour(juce::Colours::pink);
    g.drawRect (getLocalBounds());

}

void OscComponent::resized()
{
    const auto sliderWidth = 100;
    const auto sliderHeight = 140;
    
    oscWaveSelector.setBounds(10, 30, 90, 20);
    waveSelectorLabel.setBounds(10, 5, 90, 20);
    
    fmFreqSlider.setBounds(200, 30, sliderWidth, sliderHeight);
    fmFreqLabel.setBounds( fmFreqSlider.getX(), fmFreqSlider.getY() - 20, fmFreqSlider.getWidth(), 20 );
    
    fmDepthSlider.setBounds(fmFreqSlider.getRight(), 30, sliderWidth, sliderHeight);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - 20, fmDepthSlider.getWidth(), 20);
}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void OscComponent::setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::pink);
    addAndMakeVisible (slider);
    
    attachment = std::make_unique<Attachment>(apvts, paramId, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::pink);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
