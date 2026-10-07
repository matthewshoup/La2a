#include "PluginProcessor.h"
#include "PluginEditor.h"
La2aAudioProcessor::La2aAudioProcessor():AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),apvts(*this,nullptr,"PARAMETERS",layout()){}
juce::AudioProcessorValueTreeState::ParameterLayout La2aAudioProcessor::layout(){
 juce::AudioProcessorValueTreeState::ParameterLayout l;
 l.add(std::make_unique<juce::AudioParameterFloat>("peak","Peak Reduction",0.0f,1.0f,0.35f));
 l.add(std::make_unique<juce::AudioParameterFloat>("gain","Gain",0.0f,40.0f,12.0f));
 l.add(std::make_unique<juce::AudioParameterBool>("limit","Limit",false));
 l.add(std::make_unique<juce::AudioParameterFloat>("tube","Tube Drive",0.0f,1.0f,0.35f));
 l.add(std::make_unique<juce::AudioParameterFloat>("bias","Hot Bias",0.0f,1.0f,0.5f));
 l.add(std::make_unique<juce::AudioParameterFloat>("age","T4 Age",0.0f,1.0f,0.35f));
 l.add(std::make_unique<juce::AudioParameterFloat>("iron","Iron",0.0f,1.0f,0.3f));
 return l;
}
void La2aAudioProcessor::prepareToPlay(double sr,int){for(auto&s:stage)s.prepare(sr);}
bool La2aAudioProcessor::isBusesLayoutSupported(const BusesLayout& l)const{return l.getMainInputChannelSet()==l.getMainOutputChannelSet()&&(l.getMainOutputChannelSet()==juce::AudioChannelSet::mono()||l.getMainOutputChannelSet()==juce::AudioChannelSet::stereo());}
void La2aAudioProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer&){
 juce::ScopedNoDenormals n;
 float peak=apvts.getRawParameterValue("peak")->load(), gain=juce::Decibels::decibelsToGain(apvts.getRawParameterValue("gain")->load());
 float tube=apvts.getRawParameterValue("tube")->load(),bias=apvts.getRawParameterValue("bias")->load(),age=apvts.getRawParameterValue("age")->load(),iron=apvts.getRawParameterValue("iron")->load();
 bool limit=apvts.getRawParameterValue("limit")->load()>.5f;
 for(int ch=0;ch<b.getNumChannels();++ch){auto*p=b.getWritePointer(ch);for(int i=0;i<b.getNumSamples();++i)p[i]=stage[juce::jmin(ch,1)].process(p[i],peak,tube,bias,age,iron,limit)*gain;}
}
void La2aAudioProcessor::getStateInformation(juce::MemoryBlock& d){auto x=apvts.copyState().createXml();copyXmlToBinary(*x,d);}
void La2aAudioProcessor::setStateInformation(const void*d,int s){auto x=getXmlFromBinary(d,s);if(x)apvts.replaceState(juce::ValueTree::fromXml(*x));}
juce::AudioProcessorEditor* La2aAudioProcessor::createEditor(){return new La2aAudioProcessorEditor(*this);}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new La2aAudioProcessor();}
