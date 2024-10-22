//
// Created by Davis Polito on 10/22/24.
//

#ifndef ELECTROSYNTH_SOUND_GENERATOR_SECTION_H
#define ELECTROSYNTH_SOUND_GENERATOR_SECTION_H
#include "synth_section.h"
class ModulesContainer;
class EffectsViewport : public juce::Viewport {
public:
    class Listener {
    public:
        virtual ~Listener() { }
        virtual void effectsScrolled(int position) = 0;
    };

    void addListener(Listener* listener) { listeners_.push_back(listener); }
    void visibleAreaChanged(const juce::Rectangle<int>& visible_area) override {
        for (Listener* listener : listeners_)
            listener->effectsScrolled(visible_area.getY());

        Viewport::visibleAreaChanged(visible_area);
    }

private:
    std::vector<Listener*> listeners_;
};

class ModulesInterface : public SynthSection,
                         public juce::ScrollBar::Listener, EffectsViewport::Listener {
public:
    class Listener {
    public:
        virtual ~Listener() { }
        virtual void effectsMoved() = 0;
    };

    ModulesInterface();
    virtual ~ModulesInterface();

    void paintBackground(juce::Graphics& g) override;
    void paintChildrenShadows(juce::Graphics& g) override { }
    void resized() override;
    void redoBackgroundImage();


    void setFocus() { grabKeyboardFocus(); }
    void setEffectPositions();

    void initOpenGlComponents(OpenGlWrapper& open_gl) override;
    void renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) override;
    void destroyOpenGlComponents(OpenGlWrapper& open_gl) override;

    void scrollBarMoved(ScrollBar* scroll_bar, double range_start) override;
    void setScrollBarRange();

    void addListener(Listener* listener) { listeners_.push_back(listener); }
    void effectsScrolled(int position) override {
        setScrollBarRange();
        scroll_bar_->setCurrentRange(position, viewport_.getHeight());

        for (Listener* listener : listeners_)
            listener->effectsMoved();
    }

private:
    std::vector<Listener*> listeners_;
    EffectsViewport viewport_;
    std::unique_ptr<ModulesContainer> container_;
    OpenGlImage background_;
    CriticalSection open_gl_critical_section_;

    std::unique_ptr<OpenGlScrollBar> scroll_bar_;

    std::vector<std::unique_ptr<SynthSection>> modules;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulesInterface)
};
#endif //ELECTROSYNTH_SOUND_GENERATOR_SECTION_H
