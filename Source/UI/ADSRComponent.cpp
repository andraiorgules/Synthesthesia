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
ADSRComponent::ADSRComponent(juce::AudioProcessorValueTreeState& apvts)
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE", releaseSlider);
    
    //Create ADSR Sliders in UI
    setSliderParams(attackSlider);
    setSliderParams(decaySlider);
    setSliderParams(sustainSlider);
    setSliderParams(releaseSlider);
    
    //Add ADSR Labels
    setSliderLabel(attackLabel);
    setSliderLabel(decayLabel);
    setSliderLabel(sustainLabel);
    setSliderLabel(releaseLabel);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
   
    g.fillAll (juce::Colours::black);
    g.setColour(juce::Colours::pink);
    g.drawRect (getLocalBounds());

}

void ADSRComponent::resized()
{
    //Sets location of sliders in window
    //const auto bounds = getLocalBounds().reduced (10);
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

void ADSRComponent::setSliderParams (juce::Slider& slider)
{
    //Create ADSR sliders in UI
    slider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 25);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::pink);
    addAndMakeVisible(slider);
}

void ADSRComponent::setSliderLabel (juce::Label& label)
{
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::pink);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}