//
// Created by Davis Polito on 8/1/24.
//

#ifndef ELECTROSYNTH_PROCESSORMODULEWRAPPER_H
#define ELECTROSYNTH_PROCESSORMODULEWRAPPER_H
#include <juce_core/juce_core.h>
#include <chowdsp_plugin_base/chowdsp_plugin_base.h>
#include <chowdsp_plugin_state/chowdsp_plugin_state.h>
#include "../../third_party/leaf_modules/LEAF/leaf/leaf.h"
#include "../../third_party/leaf_modules/processors/processor.h"


struct ParamsPointer : public chowdsp::ParamHolder
{

};
template<typename ParameterState,typename nonParameterState>
class ProcessorModuleWrapper : public chowdsp::PluginBase<chowdsp::PluginStateImpl<ParameterState, nonParameterState>>
{
    ProcessorModuleWrapper(LEAF* leaf, Processor* proc);


    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    void processAudioBlock (juce::AudioBuffer<float>& buffer) override;

    juce::AudioProcessorEditor* createEditor() override;
protected :
    LEAF* leaf;
    Processor* proc;
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorModuleWrapper)
};


#endif //ELECTROSYNTH_PROCESSORMODULEWRAPPER_H
