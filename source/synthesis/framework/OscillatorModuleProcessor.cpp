//
// Created by Davis Polito on 8/8/24.
//

#include "OscillatorModuleProcessor.h"
float electrosynth::utils::stringToHarmonicVal(const juce::String &s){
    if(!s.contains("/"))
    {
        return s.getFloatValue();
    }
    else
    {
        juce::StringArray tokens;
        tokens.addTokens(s,"/","\"");
        return tokens[1].getFloatValue();
    }
}

juce::String electrosynth::utils::harmonicValToString(float harmonic)
{
    if(harmonic < 0.f)
        return "1 / " + juce::String(harmonic);
    else
        return juce::String(harmonic);
}
OscillatorModuleProcessor::OscillatorModuleProcessor(tOscModule& module) : _PluginBase<PluginStateImpl_<OscillatorParams, tOscModule>, tOscModule>(module),
module(module)
{


}
