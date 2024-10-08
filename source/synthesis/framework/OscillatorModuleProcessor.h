//
// Created by Davis Polito on 8/8/24.
//

#ifndef ELECTROSYNTH_OSCILLATORMODULEPROCESSOR_H
#define ELECTROSYNTH_OSCILLATORMODULEPROCESSOR_H
#include "SimpleOscModule.h"
#include "_PluginBase.h"
#include "PluginStateImpl_.h"
namespace electrosynth{
    namespace utils
    {
        float stringToHarmonicVal(const juce::String &s);
        juce::String harmonicValToString(float harmonic);
    }
}






//template <typename ParameterState,  typename T>
//class PluginStateImpl_ : public chowdsp::PluginStateImpl<ParameterState>
//{
//    static_assert (std::is_base_of_v<LEAFParams<T>, ParameterState>, "ParameterState must be a chowdsp::ParamHolder!");
//
//    PluginStateImpl_(T t, juce::AudioProcessor * proc,  juce::UndoManager* um = nullptr) : chowdsp::PluginStateImpl<ParameterState> (proc, um)
//    {
//        this->params(t);
//    }
//};

struct OscillatorParams : public LEAFParams<tOscModule >
{
    OscillatorParams(tOscModule _module) : LEAFParams<tOscModule>(_module)
    {
       add(harmonic);
    }
    chowdsp::FloatParameter::Ptr harmonic {
        juce::ParameterID{"harmonic" , 100},
        "Harmonic",
        chowdsp::ParamUtils::createNormalisableRange(-12.f, 12.f, 0.f),
        0.8f,
        &module->params[OscParams::OscHarmonic],
        [this](void * obj, float val)
        {module->setterFunctions[OscParams::OscHarmonic](obj,val);},
        &electrosynth::utils::harmonicValToString,
        &electrosynth::utils::stringToHarmonicVal
    };
    chowdsp::FloatParameter::Ptr pitchOffset;
    chowdsp::FloatParameter::Ptr pitchFine;
    chowdsp::FloatParameter::Ptr freqOffset;
    chowdsp::FloatParameter::Ptr shape;
    chowdsp::FloatParameter::Ptr amp;
    chowdsp::FloatParameter::Ptr glide;
    //chowdsp::FloatParameter stepped;
};
class OscillatorModuleProcessor : public _PluginBase<PluginStateImpl_<OscillatorParams, tOscModule>, tOscModule>
{
public:
    OscillatorModuleProcessor(tOscModule& module);


    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override{};
    void prepareToPlay (double sampleRate, int samplesPerBlock) override {};
    void releaseResources() override {}
    void processAudioBlock (juce::AudioBuffer<float>& buffer) override {};

    juce::AudioProcessorEditor* createEditor() override {return nullptr;};
    tOscModule &module;
};

#endif //ELECTROSYNTH_OSCILLATORMODULEPROCESSOR_H
