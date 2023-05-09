/*
  ==============================================================================

    FilterComponent.h
    Created: 6 May 2023 2:48:50pm
    Author:  andiwolf

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectId, juce::String filterCutoffId, juce::String filterResId);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox filterTypeSelect {"Filter Type"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectAttachment;
    
    juce::Slider filterCutoffSlider;
    juce::Slider filterResSLider;
    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<Attachment> filterCutoffAttachment;
    std::unique_ptr<Attachment> filterResAttachment;
    
    juce::Label filterTypeSelectLabel {"Filter Type", "Filter Type"};
    juce::Label filterCutoffLabel {"Filter Cutoff", "Filter Cutoff"};
    juce::Label filterResLabel {"Filter Resonance", "Filter Resonance"};
    
    void setSliderWithLabel (juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
