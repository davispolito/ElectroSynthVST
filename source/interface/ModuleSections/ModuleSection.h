//
// Created by Davis Polito on 10/22/24.
//

#ifndef ELECTROSYNTH_MODULESECTION_H
#define ELECTROSYNTH_MODULESECTION_H
#include "synth_section.h"
#include "PluginStateImpl_.h"
#include "ParameterView/ParametersView.h"
#include <juce_gui_basics/juce_gui_basics.h>
class ModuleSection : public SynthSection
{
public:
    ModuleSection(juce::String name, const juce::ValueTree &, electrosynth::ParametersViewEditor* editor);

    virtual ~ModuleSection();

    void paintBackground(Graphics& g) override;
//    void setParametersViewEditor(electrosynth::ParametersViewEditor&&);
    // void paintBackgroundShadow(Graphics& g) override { if (isActive()) paintTabShadow(g); }
    void resized() override;
  //  void setActive(bool active) override;
    //void sliderValueChanged(Slider* changed_slider) override;
    //void setAllValues(vital::control_map& controls) override;
    //void setFilterActive(bool active);
    juce::ValueTree state;
private:
    electrosynth::ParametersView* _view;
    electrosynth::ParametersViewEditor* _view_editor;


};

#endif //ELECTROSYNTH_MODULESECTION_H
