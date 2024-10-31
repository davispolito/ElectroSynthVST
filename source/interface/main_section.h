//
// Created by Davis Polito on 2/1/24.
//

#ifndef ELECTROSYNTH2_MAIN_SECTION_H
#define ELECTROSYNTH2_MAIN_SECTION_H
#include "synth_section.h"
#include "sound_generator_section.h"
#include "synth_slider.h"
class MainSection : public SynthSection
{
public:
    class Listener {
    public:
        virtual ~Listener() { }

        //virtual void showAboutSection() = 0;
    };

    MainSection(juce::ValueTree v, juce::UndoManager &um, OpenGlWrapper &open_gl, SynthGuiData * data);

    void paintBackground(Graphics& g) override;
    void resized() override;
//    void reset() override;
//
//    void buttonClicked(Button* clicked_button) override;
//
//    void notifyChange();
//    void notifyFresh();

    void addListener(Listener* listener) { listeners_.push_back(listener); }
private:
    juce::ValueTree v;
    juce::UndoManager &um;
    std::unique_ptr<ModulesInterface> modules_interface;
    std::unique_ptr<SynthSlider> s;
    std::unique_ptr<SynthButton> button;
    std::vector<Listener*> listeners_;

};

#endif //ELECTROSYNTH2_MAIN_SECTION_H
