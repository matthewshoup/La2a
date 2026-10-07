#include "PluginEditor.h"
La2aAudioProcessorEditor::La2aAudioProcessorEditor(La2aAudioProcessor& x):AudioProcessorEditor(&x),p(x){
 for(auto*s:{&peak,&gain,&tube,&bias,&age,&iron}){s->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);s->setTextBoxStyle(juce::Slider::TextBoxBelow,false,80,20);addAndMakeVisible(*s);}
 addAndMakeVisible(limit); limit.setButtonText("COMPRESS / LIMIT");
 aPeak=std::make_unique<SA>(p.apvts,"peak",peak);aGain=std::make_unique<SA>(p.apvts,"gain",gain);aTube=std::make_unique<SA>(p.apvts,"tube",tube);aBias=std::make_unique<SA>(p.apvts,"bias",bias);aAge=std::make_unique<SA>(p.apvts,"age",age);aIron=std::make_unique<SA>(p.apvts,"iron",iron);aLimit=std::make_unique<BA>(p.apvts,"limit",limit);
 setSize(760,420);
}
void La2aAudioProcessorEditor::paint(juce::Graphics& g){g.fillAll(juce::Colour(0xffd8d0bc));g.setColour(juce::Colours::black);g.setFont(32);g.drawText("LA2A  •  TUBE OPTO LEVELER",20,15,getWidth()-40,45,juce::Justification::centred);g.setFont(15);g.drawText("PEAK REDUCTION          GAIN             TUBE DRIVE          HOT BIAS            T4 AGE              IRON",25,305,getWidth()-50,25,juce::Justification::centred);}
void La2aAudioProcessorEditor::resized(){int y=95,w=110,gap=10,x=25;for(auto*s:{&peak,&gain,&tube,&bias,&age,&iron}){s->setBounds(x,y,w,190);x+=w+gap;}limit.setBounds(260,345,240,35);}
void La2aAudioProcessorEditor::setup(juce::Slider&,const juce::String&){}
