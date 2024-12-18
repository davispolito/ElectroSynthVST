//
// Created by Davis Polito on 12/17/24.
//

#ifndef ELECTROSYNTH_MODULATIONWRAPPER_H
#define ELECTROSYNTH_MODULATIONWRAPPER_H
#include "processors/mapping.h"
#include <juce_data_structures/juce_data_structures.h>

struct MappingWrapper
    {
       leaf::Mapping mapping;
       std::string destination;
    };



#endif //ELECTROSYNTH_MODULATIONWRAPPER_H
