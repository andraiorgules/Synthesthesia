/*
  ==============================================================================

    FilterComponent.cpp
    Created: 6 May 2023 2:48:50pm
    Author:  andiwolf

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectId, juce::String filterCutoffId, juce::String filterResId)
{
    juce::StringArray choices { "Low-Pass", "Band-Pass", "High-Pass" };
    filterTypeSelect.addItemList(choices, 1);
    addAndMakeVisible (filterTypeSelect);

    filterTypeSelectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectId, filterTypeSelect);
    
    setSliderWithLabel (filterCutoffSlider, filterCutoffLabel, apvts, filterCutoffId, filterCutoffAttachment);
    setSliderWithLabel(filterResSLider, filterResLabel, apvts, filterResId, filterResAttachment);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour(juce::Colours::pink);
    g.drawRect (getLocalBounds());
}

void FilterComponent::resized()
{
    const auto sliderWidth = 100;
    const auto sliderHeight = 140;
    
    filterTypeSelect.setBounds(0, 0, 90, 20);
    filterTypeSelectLabel.setBounds(10, 35, filterTypeSelect.getWidth(), 20);
    
    filterCutoffSlider.setBounds(filterTypeSelect.getRight() + 20, 60, sliderWidth, sliderHeight);
    filterCutoffLabel.setBounds( filterCutoffSlider.getX(), filterCutoffSlider.getY() - 20, filterCutoffSlider.getWidth(), 20 );
    
    filterResSLider.setBounds(filterCutoffSlider.getRight(), 60, sliderWidth, sliderHeight);
    filterResLabel.setBounds(filterResSLider.getX(), filterResSLider.getY() - 20, filterResSLider.getWidth(), 20);
}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void FilterComponent::setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::pink);
    addAndMakeVisible (slider);
    
    attachment = std::make_unique<Attachment>(apvts, paramId, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::pink);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
