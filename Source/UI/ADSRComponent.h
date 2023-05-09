/*
  ==============================================================================

    ADSRComponent.h
    Created: 23 Apr 2023 7:04:43pm
    Author:  andiwolf

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(juce::AudioProcessorValueTreeState& apvts);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    void setSliderParams (juce::Slider& slider);
    void setSliderLabel (juce::Label& label);
    
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    
    juce::Label attackLabel {"Attack", "Attack"};
    juce::Label decayLabel {"Decay", "Decay"};
    juce::Label sustainLabel {"Sustain", "Sustain"};
    juce::Label releaseLabel {"Release", "Release"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
