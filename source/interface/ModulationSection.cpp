//
// Created by Davis Polito on 11/19/24.
//


#include "ModulationSection.h"
ModulationSection::ModulationSection(juce::String name, const juce::ValueTree &v, electrosynth::ParametersViewEditor* editor) : SynthSection(name), state(v), _view_editor(editor)
{


    addSubSection(&_view_editor->view);
}

ModulationSection::~ModulationSection() = default;

void ModulationSection::paintBackground(juce::Graphics &g)
{
    SynthSection::paintBackground(g);
}

void ModulationSection::resized()
{
    int widget_margin = findValue(Skin::kWidgetMargin);
    int title_width = getTitleWidth();
    int section_height = getKnobSectionHeight();

    Rectangle<int> bounds = getLocalBounds().withLeft(title_width);
    Rectangle<int> knobs_area = getDividedAreaBuffered(bounds, 2, 1, widget_margin);
    Rectangle<int> settings_area = getDividedAreaUnbuffered(bounds, 4, 0, widget_margin);
    _view_editor->view.setBounds(getLocalBounds());
    int knob_y2 =0;
    SynthSection::resized();
}

//void ModulationSection::setParametersViewEditor (electrosynth::ParametersViewEditor&& editor)
//{
//   _view_editor = editor;
//   addSubSection(_view);
//
//}