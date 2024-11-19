//
// Created by Davis Polito on 11/19/24.
//

#ifndef ELECTROSYNTH_MODULATORBASE_H
#define ELECTROSYNTH_MODULATORBASE_H
#include "PluginStateImpl_.h"
#include "leaf.h"
#include "ParameterView/ParametersView.h"
template <class PluginStateType>
class ModulatorBase : public juce::AudioSource
{
public:
    explicit ModulatorBase( LEAF* leaf,juce::ValueTree& tree, juce::UndoManager* um = nullptr);
    ~ModulatorBase() override = default;
    LEAF* leaf;
    juce::ValueTree vt;
    leaf::Processor proc;
    PluginStateType state;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo &bufferToFill) override {}
    void prepareToPlay (int samplesPerBlock, double sampleRate ) override {}
    void releaseResources() override {}
    virtual electrosynth::ParametersView* createEditor() = 0;

};
template <class State>
ModulatorBase<State>::ModulatorBase (LEAF* leaf,juce::ValueTree& tree, juce::UndoManager* um)
    :
      state (leaf),
      leaf(leaf),
      vt(tree)

{
}

#endif //ELECTROSYNTH_MODULATORBASE_H
