//
// Created by Davis Polito on 8/1/24.
//

#include "ProcessorModuleWrapper.h"

template <typename ParameterState, typename nonParameterState>
ProcessorModuleWrapper<ParameterState, nonParameterState>::ProcessorModuleWrapper (LEAF* leaf, leaf::Processor* proc)
:leaf(leaf), proc(proc)
{
}

template <typename ParameterState, typename nonParameterState>
void ProcessorModuleWrapper<ParameterState, nonParameterState>::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}
template <typename ParameterState, typename nonParameterState>
void ProcessorModuleWrapper<ParameterState, nonParameterState>::processAudioBlock (juce::AudioBuffer<float>& buffer)
{
}
template <typename ParameterState, typename nonParameterState>
juce::AudioProcessorEditor* ProcessorModuleWrapper<ParameterState, nonParameterState>::createEditor()
{
    return nullptr;
}