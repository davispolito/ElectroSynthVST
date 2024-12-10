//
// Created by Davis Polito on 12/10/24.
//

#ifndef ELECTROSYNTH_MODULATIONCONNECTION_H
#define ELECTROSYNTH_MODULATIONCONNECTION_H
#include <juce_data_structures/juce_data_structures.h>
#include "Identifiers.h"
namespace electrosynth {
    struct ModulationConnection {
        ModulationConnection(juce::ValueTree &t);

        void setSource(int uuid_from)
        {
            state.setProperty(IDs::src, uuid_from, nullptr);
        }

        void setDestination(int uuid_to)
        {
            state.setProperty(IDs::src, uuid_to, nullptr);
        }

        void setModulationAmount(float amt)
        {
            state.setProperty(IDs::modAmt, amt, nullptr);
        }
        void setPolarity(bool isBipolar)
        {
            state.setProperty(IDs::isBipolar, isBipolar, nullptr);
        }
        juce::ValueTree state;
    };


}
#endif //ELECTROSYNTH_MODULATIONCONNECTION_H
