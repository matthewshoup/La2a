#pragma once
#include <JuceHeader.h>
#include <cmath>

class OptoTubeStage {
public:
 void prepare(double sr){ sampleRate=sr; reset(); }
 void reset(){ fast=slow=memory=0.0f; cathode=flux=0.0f; }
 float process(float x,float peakReduction,float tubeDrive,float hotBias,float age,float iron,bool limitMode){
   const float detector=std::abs(x)*(1.0f+peakReduction*7.0f);
   const float target=juce::jlimit(0.0f,1.0f,detector);
   // T4-inspired: near-instant illumination, dual program-dependent dark recovery.
   const float attackMs=0.01f;
   const float releaseFastMs=60.0f*(1.0f+age*0.5f);
   const float releaseSlowMs=1000.0f+age*14000.0f+memory*4000.0f;
   fast=smooth(target,fast,target>fast?attackMs:releaseFastMs);
   slow=smooth(target,slow,target>slow?attackMs*4.0f:releaseSlowMs);
   memory += (target-memory)*(target>memory?0.0025f:0.00002f);
   const float cell=juce::jlimit(0.0f,1.0f,0.72f*fast+0.28f*slow);
   const float maxGR=limitMode?40.0f:28.0f;
   const float grDb=-maxGR*std::pow(cell,limitMode?0.72f:1.05f);
   float y=x*juce::Decibels::decibelsToGain(grDb);

   // 12AX7/12BH7-inspired asymmetric stages with cathode-memory bias shift.
   const float drive=1.0f+tubeDrive*11.0f;
   cathode += (std::abs(y)*drive-cathode)*0.00035f;
   const float bias=(hotBias-0.5f)*0.7f-cathode*0.08f;
   float triode1=asym(y*drive+bias,1.65f,0.92f);
   float triode2=asym(triode1*(1.2f+tubeDrive*1.8f)-bias*0.35f,1.35f,0.96f);

   // Output transformer surrogate: flux memory + soft saturation.
   flux += (triode2-flux)*0.0012f;
   const float ironDrive=1.0f+iron*5.0f;
   float out=std::tanh((triode2+flux*iron*0.22f)*ironDrive)/std::tanh(ironDrive);
   return out;
 }
 float gainReductionDb() const { return -40.0f*juce::jlimit(0.0f,1.0f,0.72f*fast+0.28f*slow); }
private:
 double sampleRate=44100.0; float fast=0,slow=0,memory=0,cathode=0,flux=0;
 float smooth(float target,float state,float ms) const {
   const float a=std::exp(-1.0f/(0.001f*ms*(float)sampleRate));
   return target+(state-target)*a;
 }
 static float asym(float x,float pos,float neg){
   return x>=0.0f?std::tanh(x*pos)/pos:std::tanh(x*neg)/neg;
 }
};
