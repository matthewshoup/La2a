#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
class La2aAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
 explicit La2aAudioProcessorEditor(La2aAudioProcessor&);
 void paint(juce::Graphics&) override; void resized() override;
private:
 La2aAudioProcessor& p;
 juce::Slider peak,gain,tube,bias,age,iron; juce::ToggleButton limit{"LIMIT"};
 using SA=juce::AudioProcessorValueTreeState::SliderAttachment; using BA=juce::AudioProcessorValueTreeState::ButtonAttachment;
 std::unique_ptr<SA> aPeak,aGain,aTube,aBias,aAge,aIron; std::unique_ptr<BA> aLimit;
 void setup(juce::Slider&,const juce::String&);
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(La2aAudioProcessorEditor)
};
