/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 23 Apr 2023 7:04:43pm
    Author:  andiwolf

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId)
{
    componentName = name;
    
    setSliderWithLabel(attackSlider, attackLabel, apvts, attackId, attackAttachment);
    setSliderWithLabel(decaySlider, decayLabel, apvts, decayId, decayAttachment);
    setSliderWithLabel(sustainSlider, sustainLabel, apvts, sustainId, sustainAttachment);
    setSliderWithLabel(releaseSlider, releaseLabel, apvts, releaseId, releaseAttachment);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced (5);
    auto labelSpace = bounds.removeFromTop (25.0f);
    
    g.fillAll (juce::Colours::black);
    g.setColour(juce::Colours::pink);
    g.drawText(componentName, labelSpace.withX(10), juce::Justification::left);
    g.drawRect (getLocalBounds());

}

void ADSRComponent::resized()
{
    //Sets location of sliders in window
    const auto padding = 7;
    const auto sliderWidth = 70;
    const auto sliderHeight = 100;
    const auto sliderStartX = 0;
    const auto sliderStartY = 40;
    
    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    
    attackLabel.setBounds(attackSlider.getX(), attackSlider.getY() - 15, attackSlider.getWidth(), 20 );
    decayLabel.setBounds(decaySlider.getX(), decaySlider.getY() - 15, decaySlider.getWidth(), 20 );
    sustainLabel.setBounds(sustainSlider.getX(), sustainSlider.getY() - 15, sustainSlider.getWidth(), 20 );
    releaseLabel.setBounds(releaseSlider.getX(), releaseSlider.getY() - 15, releaseSlider.getWidth(), 20 );
}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void ADSRComponent::setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::pink);
    addAndMakeVisible (slider);
    
    attachment = std::make_unique<Attachment>(apvts, paramId, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::pink);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
