//
// Created by Davis Polito on 10/22/24.
//

#ifndef ELECTROSYNTH_SOUND_GENERATOR_SECTION_H
#define ELECTROSYNTH_SOUND_GENERATOR_SECTION_H
#include "synth_section.h"
#include "Identifiers.h"
#include <tracktion_engine.h>
#include "ModuleSections/ModuleSection.h"
#include <functional>
#include <map>
#include <string>
#include <iostream>
//template <class Base>
//class Factory {
//public:
//    using CreateFunction = std::function<Base*(std::any)>;
//
//    template <typename T, typename... Args>
//    void registerType(const std::string& typeName) {
//        creators[typeName] = [](std::any args) -> Base* {
//            try {
//                auto tupleArgs = std::any_cast<std::tuple<Args...>>(args); // Unpack std::any into tuple
//                return std::apply([](auto&&... unpackedArgs) {
//                    return new T(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);  // Forward arguments to constructor
//                }, tupleArgs);  // Apply the arguments
//            } catch (const std::bad_any_cast& e) {
//                std::cerr << "std::bad_any_cast: " << e.what() << " (expected tuple)" << std::endl;
//                return nullptr;
//            }
//        };
//    }
//
//    // Create object with arguments wrapped in std::any
//    Base* create(const std::string& typeName, std::any args) const {
//        auto it = creators.find(typeName);
//        if (it != creators.end()) {
//            return it->second(args);  // Call the creation function with arguments
//        }
//        return nullptr;  // Type not found
//    }
//
//
//private:
//    std::map<std::string, CreateFunction> creators;
//};

template <class Base>
class Factory {
public:
    using CreateFunction = std::function<std::shared_ptr<Base>(std::any)>;

    template <typename T, typename... Args>
    void registerType(const std::string& typeName) {
        creators[typeName] = [](std::any args) -> std::shared_ptr<Base> {
            try {
                auto tupleArgs = std::any_cast<std::tuple<Args...>>(args); // Unpack std::any into tuple
                return std::apply([](auto&&... unpackedArgs) {
                    return std::make_shared<T>(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);  // Create shared_ptr with forwarded arguments
                }, tupleArgs);  // Apply the arguments
            } catch (const std::bad_any_cast& e) {
                std::cerr << "std::bad_any_cast: " << e.what() << " (expected tuple)" << std::endl;
                return nullptr;
            }
        };
    }

    // Create object with arguments wrapped in std::any
    std::shared_ptr<Base> create(const std::string& typeName, std::any args) const {
        auto it = creators.find(typeName);
        if (it != creators.end()) {
            return it->second(args);  // Call the creation function with arguments
        }
        return nullptr;  // Type not found
    }

private:
    std::map<std::string, CreateFunction> creators;
};
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
                         public juce::ScrollBar::Listener, EffectsViewport::Listener,
                         public tracktion::engine::ValueTreeObjectList<ModuleSection>  {
public:
    class Listener {
    public:
        virtual ~Listener() { }
        virtual void effectsMoved() = 0;
    };
    ModuleSection* createNewObject(const juce::ValueTree& v) override;
    void deleteObject (ModuleSection* at) override;


   // void reset() override;
    void newObjectAdded (ModuleSection*) override;
    void objectRemoved (ModuleSection*) override     { resized();}//resized(); }
    void objectOrderChanged() override              {resized(); }//resized(); }
    void valueTreeParentChanged (juce::ValueTree&) override{};
    void valueTreeRedirected (juce::ValueTree&) override;
    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::MODULE);
    }


    ModulesInterface(juce::ValueTree &);
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
    void mouseDown(const juce::MouseEvent&e);
    PopupItems createPopupMenu();
    void handlePopupResult(int result);
private:
    std::vector<Listener*> listeners_;
    EffectsViewport viewport_;
    std::unique_ptr<ModulesContainer> container_;
    OpenGlImage background_;
    CriticalSection open_gl_critical_section_;

    std::unique_ptr<OpenGlScrollBar> scroll_bar_;
//
//    std::vector<std::unique_ptr<SynthSection>> modules;


    Factory<juce::AudioProcessor> factory;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulesInterface)
};
#endif //ELECTROSYNTH_SOUND_GENERATOR_SECTION_H
