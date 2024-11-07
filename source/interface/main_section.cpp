//
// Created by Davis Polito on 2/1/24.
//
#include "main_section.h"
#include "synth_slider.h"

MainSection::MainSection(juce::ValueTree v, juce::UndoManager &um, OpenGlWrapper & open_gl, SynthGuiData* data) : SynthSection("main_section"), v(v), um(um)
{
    //constructionSite_ = std::make_unique<ConstructionSite>(v, um, open_gl, data);
    modules_interface = std::make_unique<ModulesInterface>(v);
    addSubSection(modules_interface.get());
    //addAndMakeVisible(constructionPort);
//    ValueTree t(IDs::PREPARATION);
//
//    t.setProperty(IDs::type,electrosynth::BKPreparationType::PreparationTypeDirect, nullptr);
//    t.setProperty(IDs::x,255, nullptr);
//    t.setProperty(IDs::y,255, nullptr);
//    v.addChild(t,-1, nullptr);

    //s->setAlwaysOnTop(true);
    setSkinOverride(Skin::kNone);
}

void MainSection::paintBackground(juce::Graphics& g)
{
    paintContainer(g);

    g.setColour(findColour(Skin::kBody, true));
    paintChildrenBackgrounds(g);
    paintKnobShadows(g);
    g.saveState();

    g.restoreState();


}

void MainSection::resized()
{

    int height = getHeight();
    int width = getWidth();
    int widget_margin = findValue(Skin::kWidgetMargin);
    int large_padding = findValue(Skin::kLargePadding);
    int padding = getPadding();
    int active_width = getWidth() - padding;
    int width_left = (active_width - padding) / 2;
    int width_right = active_width - width_left;
    int right_x = width_left + padding;
//    s->setBounds(0, 0, 100 *size_ratio_, 100* size_ratio_);
//    button->setBounds(right_x, 100, 20, 20);
    modules_interface->setBounds(0, 0, width,height);
    //constructionPort.setBounds(large_padding, 0,getDisplayScale()* width, getDisplayScale() * height);
    //constructionPort.setBounds(large_padding, 0,width, height);
    DBG (":");
    DBG("main section");
    DBG("display scale" + String(getDisplayScale()));
    DBG("width" + String(getWidth()));
    DBG("height" + String(getHeight()));
    //SynthSection::resized();
}