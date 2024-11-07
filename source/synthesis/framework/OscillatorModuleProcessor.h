//
// Created by Davis Polito on 8/8/24.
//

#ifndef ELECTROSYNTH_OSCILLATORMODULEPROCESSOR_H
#define ELECTROSYNTH_OSCILLATORMODULEPROCESSOR_H
#include "SimpleOscModule.h"
#include "_PluginBase.h"
#include "PluginStateImpl_.h"
#include "ParameterView/ParametersView.h"
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
//typedef enum {
//    OscMidiPitch,
//    OscHarmonic,
//    OscPitchOffset,
//    OscPitchFine,
//    OscFreqOffset,
//    OscShapeParam,
//    OscAmpParam,
//    OscGlide,
//    OscStepped,
//    OscSyncMode,
//    OscSyncIn,
//    OscType,
//    OscNumParams
//} OscParams;
struct OscillatorParams : public LEAFParams<_tOscModule >
{
    OscillatorParams(LEAF* leaf) : LEAFParams<_tOscModule>(leaf)
    {
       add(harmonic, pitchOffset, pitchFine, freqOffset, glide, shape, harmonicstepped, amp);
        //add(pitchOffset);
    }
    chowdsp::FloatParameter::Ptr harmonic {
        juce::ParameterID{"harmonic" , 100},
        "Harmonic",
        chowdsp::ParamUtils::createNormalisableRange(-16.f, 16.f, 0.f),
        0.f,
        &module->params[OscParams::OscHarmonic],
        [this](float val)
        {module->setterFunctions[OscParams::OscHarmonic](this->module,val);
        DBG("harm [0 - 1]" + juce::String(val) + " .. .  harm actual Val" + juce::String(this->module->harmonicMultiplier));
        },
        &electrosynth::utils::harmonicValToString,
        &electrosynth::utils::stringToHarmonicVal
    };
    chowdsp::FloatParameter::Ptr pitchOffset {
        juce::ParameterID{"pitch" , 100},
        "Pitch",
        chowdsp::ParamUtils::createNormalisableRange(-12.f, 12.f, 0.f,1.f),
        0.0f,
        &module->params[OscParams::OscPitchOffset],
        [this](float val)
        {module->setterFunctions[OscParams::OscPitchOffset](this->module,val);
        DBG("pitch [0 - 1] " + juce::String(val)  + " ... pitch actual " + juce::String(this->module->pitchOffset));
       },

        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };

    chowdsp::FloatParameter::Ptr pitchFine {
        juce::ParameterID{"pitch_fine" , 100},
        "Pitch Fine",
        chowdsp::ParamUtils::createNormalisableRange(-1.f, 1.f,0.f),
        0.f,
        &module->params[OscParams::OscPitchFine],
        [this]( float val)
        {module->setterFunctions[OscParams::OscPitchFine](this->module,val);
        DBG("fine [0 - 1] " + juce::String(val) + " ..... fine actual " + juce::String(this->module->fine));
    },
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };

    chowdsp::FloatParameter::Ptr freqOffset
        {
        juce::ParameterID{"freq_offset" , 100},
        "Freq Offset",
        chowdsp::ParamUtils::createNormalisableRange(-2000.f, 2000.f,0.f),
        0.0f,
        &module->params[OscParams::OscFreqOffset],
        [this]( float val)
        {module->setterFunctions[OscParams::OscFreqOffset](this->module,val);
            DBG("freq [0 - 1] " + juce::String(val) + " .... freq actual Val" + juce::String(this->module->freqOffset));},
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };

    chowdsp::FloatParameter::Ptr glide
    {
        juce::ParameterID{"glide" , 100},
        "Freq Glide",
        chowdsp::ParamUtils::createNormalisableRange(0.f, 1.f,0.5f),
        0.0f,
        &module->params[OscParams::OscGlide],
        [this]( float val)
        {module->setterFunctions[OscParams::OscGlide](this->module,val);
            //DBG("freq [0 - 1] " + juce::String(val) + " .... glide actual Val" + juce::String(this->module->glide));
        },
        &chowdsp::ParamUtils::floatValToString,
        &chowdsp::ParamUtils::stringToFloatVal
    };
    chowdsp::FloatParameter::Ptr shape
        {
            juce::ParameterID{"shape" , 100},
            "Shape",
            chowdsp::ParamUtils::createNormalisableRange(0.0f, 1.f ,0.5f),
            0.5f,
            &module->params[OscParams::OscShapeParam],
            [this]( float val)
            {module->setterFunctions[OscParams::OscShapeParam](this->module->theOsc,val);
            DBG("sghape [0 - 1]" + juce::String(val) + ".... cant see actual val");
   },
            &chowdsp::ParamUtils::floatValToString,
            &chowdsp::ParamUtils::stringToFloatVal
        };
    chowdsp::FloatParameter::Ptr amp
        {
            juce::ParameterID{"amp" , 100},
            "amplitude",
            chowdsp::ParamUtils::createNormalisableRange(0.0f, 2.f ,1.f),
            0.8f,
            &module->params[OscParams::OscAmpParam],
            [this]( float val)
            {module->setterFunctions[OscParams::OscAmpParam](this->module,val);
            DBG("amp [0 - 1] " + juce::String(val) + ".. .... amp actual " + juce::String(this->module->amp));
            },
            &chowdsp::ParamUtils::floatValToString,
            &chowdsp::ParamUtils::stringToFloatVal
        };
    chowdsp::BoolParameter::Ptr harmonicstepped
    {
        juce::ParameterID{"harmoniStepped" , 100},
        "Harmonic stepped",
        0.0,
        &module->params[OscParams::OscSteppedHarmonic],
        [this]( float val)
        {module->setterFunctions[OscParams::OscSteppedHarmonic](this->module,val);
            //harmonic->range.interval = val;
            DBG("amp [0 - 1] " + juce::String(val) + ".. .... stepped harmonic actual " + juce::String(this->module->hStepped));
        }
    };
    chowdsp::BoolParameter::Ptr pitchStepped
{
    juce::ParameterID{"harmoniStepped" , 100},
    "Harmonic stepped",
    0.0,
    &module->params[OscParams::OscSteppedPitch],
    [this]( float val)
    {module->setterFunctions[OscParams::OscSteppedPitch](this->module,val);
        //harmonic->range.interval = val;
        DBG("amp [0 - 1] " + juce::String(val) + ".. .... stepped pitch actual " + juce::String(this->module->pStepped));
    }
};




};
class OscillatorModuleProcessor : public _PluginBase<PluginStateImpl_<OscillatorParams, _tOscModule>, _tOscModule>
{
public:
    OscillatorModuleProcessor(const juce::ValueTree&, LEAF* leaf);


    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override {};
    void releaseResources() override {}
    void processAudioBlock (juce::AudioBuffer<float>& buffer) override {};
    bool acceptsMidi() const override
    {
       return true;
    }
    juce::AudioProcessorEditor* createEditor() override {return new electrosynth::ParametersViewEditor{*this};};
    leaf::tProcessor processor;
};

#endif //ELECTROSYNTH_OSCILLATORMODULEPROCESSOR_H
