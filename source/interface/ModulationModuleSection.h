//
// Created by Davis Polito on 11/19/24.
//

#ifndef ELECTROSYNTH_ModulationMODULESECTION_H
#define ELECTROSYNTH_ModulationMODULESECTION_H
#include "sound_generator_section.h"
class ModulationSection;
class ModulationModuleSection : public ModulesInterface<ModulationSection>
{
public:
    ModulationModuleSection(juce::ValueTree &);
    virtual ~ModulationModuleSection();
    ModulationSection* createNewObject(const juce::ValueTree& v) override;
    void deleteObject (ModulationSection* at) override;


    // void reset() override;
    void objectRemoved (ModulationSection*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }
    void valueTreeParentChanged (juce::ValueTree&) override{};
    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return false;
     }
    void setEffectPositions() override;

     PopupItems createPopupMenu() override;
     void handlePopupResult(int result) override;
};

#endif //ELECTROSYNTH_ModulationMODULESECTION_H
