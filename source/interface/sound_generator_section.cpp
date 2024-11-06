//
// Created by Davis Polito on 10/22/24.
//
#include "sound_generator_section.h"
#include "ModuleSections/ModuleSection.h"
#include "OscillatorModuleProcessor.h"
#include "ParameterView/ParametersView.h"
class ModulesContainer : public SynthSection {
public:
    ModulesContainer(String name) : SynthSection(name) { }
    void paintBackground(Graphics& g) override {
        g.fillAll(findColour(Skin::kBackground, true));
        paintChildrenShadows(g);
        paintChildrenBackgrounds(g);
    }
};

ModulesInterface::ModulesInterface(juce::ValueTree &v) : SynthSection("modules"), tracktion::engine::ValueTreeObjectList<ModuleSection>(v) {
    container_ = std::make_unique<ModulesContainer>("container");
    
    addAndMakeVisible(viewport_);
    viewport_.setViewedComponent(container_.get());
    viewport_.addListener(this);
    viewport_.setScrollBarsShown(false, false, true, false);

//    chorus_section_ = std::make_unique<ChorusSection>("CHORUS", mono_modulations);
//    container_->addSubSection(chorus_section_.get());
//
//    compressor_section_ = std::make_unique<CompressorSection>("COMPRESSOR");
//    container_->addSubSection(compressor_section_.get());
//
//    delay_section_ = std::make_unique<DelaySection>("DELAY", mono_modulations);
//    container_->addSubSection(delay_section_.get());
//
//    distortion_section_ = std::make_unique<DistortionSection>("DISTORTION", mono_modulations);
//    container_->addSubSection(distortion_section_.get());
//
//    equalizer_section_ = std::make_unique<EqualizerSection>("EQ", mono_modulations);
//    container_->addSubSection(equalizer_section_.get());
//
//    flanger_section_ = std::make_unique<FlangerSection>("FLANGER", mono_modulations);
//    container_->addSubSection(flanger_section_.get());
//
//    phaser_section_ = std::make_unique<PhaserSection>("PHASER", mono_modulations);
//    container_->addSubSection(phaser_section_.get());
//
//    reverb_section_ = std::make_unique<ReverbSection>("REVERB", mono_modulations);
//    container_->addSubSection(reverb_section_.get());
//
//    filter_section_ = std::make_unique<FilterSection>("fx", mono_modulations);
//    container_->addSubSection(filter_section_.get());
//
//    effect_order_ = std::make_unique<DragDropEffectOrder>("effect_chain_order");
//    addSubSection(effect_order_.get());
//    effect_order_->addListener(this);
//
//    modules.push_back(std::make_unique<ModuleSection>());
//    container_->addSubSection(modules[0].get());
    addSubSection(container_.get(), false);

//    effects_list_[0] = chorus_section_.get();
//    effects_list_[1] = compressor_section_.get();
//    effects_list_[2] = delay_section_.get();
//    effects_list_[3] = distortion_section_.get();
//    effects_list_[4] = equalizer_section_.get();
//    effects_list_[5] = filter_section_.get();
//    effects_list_[6] = flanger_section_.get();
//    effects_list_[7] = phaser_section_.get();
//    effects_list_[8] = reverb_section_.get();

    scroll_bar_ = std::make_unique<OpenGlScrollBar>();
    scroll_bar_->setShrinkLeft(true);
    addAndMakeVisible(scroll_bar_.get());
    addOpenGlComponent(scroll_bar_->getGlComponent());
    scroll_bar_->addListener(this);

    setOpaque(false);

    factory.registerType<OscillatorModuleProcessor, juce::ValueTree, LEAF*>("OscModule");

//    setSkinOverride(Skin::kAllEffects);
}

ModulesInterface::~ModulesInterface() {
    freeObjects();
}

void ModulesInterface::paintBackground(Graphics& g) {
    Colour background = findColour(Skin::kBackground, true);
    g.setColour(background);
    g.fillRect(getLocalBounds().withRight(getWidth() - findValue(Skin::kLargePadding) / 2));
    //paintChildBackground(g, effect_order_.get());

    redoBackgroundImage();
}

void ModulesInterface::redoBackgroundImage() {
    Colour background = findColour(Skin::kBackground, true);

    int height = std::max(container_->getHeight(), getHeight());
    int mult = getPixelMultiple();
    Image background_image = Image(Image::ARGB, container_->getWidth() * mult, height * mult, true);

    Graphics background_graphics(background_image);
    background_graphics.addTransform(AffineTransform::scale(mult));
    background_graphics.fillAll(background);
    container_->paintBackground(background_graphics);
    background_.setOwnImage(background_image);
}

void ModulesInterface::resized() {
    static constexpr float kEffectOrderWidthPercent = 0.2f;

    ScopedLock lock(open_gl_critical_section_);

    int order_width = getWidth() * kEffectOrderWidthPercent;
//    effect_order_->setBounds(0, 0, order_width, getHeight());
//    effect_order_->setSizeRatio(size_ratio_);
    int large_padding = findValue(Skin::kLargePadding);
    int shadow_width = getComponentShadowWidth();
    int viewport_x = 0 + large_padding - shadow_width;
    int viewport_width = getWidth() - viewport_x - large_padding + 2 * shadow_width;
    viewport_.setBounds(viewport_x, 0, viewport_width, getHeight());
    setEffectPositions();

    scroll_bar_->setBounds(getWidth() - large_padding + 1, 0, large_padding - 2, getHeight());
    scroll_bar_->setColor(findColour(Skin::kLightenScreen, true));

    SynthSection::resized();
}

//void ModulesInterface::orderChanged(DragDropEffectOrder* order) {
//    ScopedLock lock(open_gl_critical_section_);
//
//    setEffectPositions();
//    repaintBackground();
//}
//
//void ModulesInterface::effectEnabledChanged(int order_index, bool enabled) {
//    ScopedLock lock(open_gl_critical_section_);
//
//    if (enabled)
//        DBG("aen");
//        //effects_list_[order_index]->activator()->setToggleState(true, sendNotification);
//
//    setEffectPositions();
//    repaintBackground();
//}
//
PopupItems ModulesInterface::createPopupMenu()
{
    PopupItems options;
    options.addItem(1, "add osc" );
    return options;
}
void ModulesInterface::mouseDown (const juce::MouseEvent& e)
{
    if(e.mods.isPopupMenu())
    {
        PopupItems options = createPopupMenu();
        showPopupSelector(this, e.getPosition(), options, [=](int selection) { handlePopupResult(selection); });
    }
}

void ModulesInterface::handlePopupResult(int result) {

    //std::vector<vital::ModulationConnection*> connections = getConnections();
    if (result == 1 )
    {
        juce::ValueTree t(IDs::MODULE);
        t.setProperty(IDs::type, "OscModule", nullptr);
        parent.appendChild(t,nullptr);
    }
//    if (result == kArmMidiLearn)
//        synth->armMidiLearn(getName().toStdString());
//    else if (result == kClearMidiLearn)
//        synth->clearMidiLearn(getName().toStdString());
//    else if (result == kDefaultValue)
//        setValue(getDoubleClickReturnValue());
//    else if (result == kManualEntry)
//        showTextEntry();
//    else if (result == kClearModulations) {
//        for (vital::ModulationConnection* connection : connections) {
//            std::string source = connection->source_name;
//            synth_interface_->disconnectModulation(connection);
//        }
//        notifyModulationsChanged();
//    }
//    else if (result >= kModulationList) {
//        int connection_index = result - kModulationList;
//        std::string source = connections[connection_index]->source_name;
//        synth_interface_->disconnectModulation(connections[connection_index]);
//        notifyModulationsChanged();
//    }
}

void ModulesInterface::setEffectPositions() {
    if (getWidth() <= 0 || getHeight() <= 0)
        return;

    int padding = getPadding();
    int large_padding = findValue(Skin::kLargePadding);
    int shadow_width = getComponentShadowWidth();
    int start_x = 0;
    int effect_width = getWidth() - start_x - large_padding;
    int knob_section_height = getKnobSectionHeight();
    int widget_margin = findValue(Skin::kWidgetMargin);
    int effect_height = 2 * knob_section_height - widget_margin;
    int y = 0;

    juce::Point<int> position = viewport_.getViewPosition();
    DBG("position viewport: x: " + juce::String(position.getX()) + "y: " + juce::String(position.getY()));
    DBG("shadwo width: " + String(shadow_width));
    for(auto& section : objects)
    {
      section->setBounds(shadow_width, y, effect_width, effect_height);
      y += effect_height + padding;
    }

    container_->setBounds(0, 0, viewport_.getWidth(), y - padding);
    viewport_.setViewPosition(position);

    for (Listener* listener : listeners_)
        listener->effectsMoved();

    container_->setScrollWheelEnabled(container_->getHeight() <= viewport_.getHeight());
    setScrollBarRange();
    repaintBackground();
}

void ModulesInterface::initOpenGlComponents(OpenGlWrapper& open_gl) {
    background_.init(open_gl);
    SynthSection::initOpenGlComponents(open_gl);
}

void ModulesInterface::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {
    ScopedLock lock(open_gl_critical_section_);

    OpenGlComponent::setViewPort(&viewport_, open_gl);

    float image_width = electrosynth::utils::nextPowerOfTwo(background_.getImageWidth());
    float image_height = electrosynth::utils::nextPowerOfTwo(background_.getImageHeight());
    int mult = getPixelMultiple();
    float width_ratio = image_width / (container_->getWidth() * mult);
    float height_ratio = image_height / (viewport_.getHeight() * mult);
    float y_offset = (2.0f * viewport_.getViewPositionY()) / getHeight();

    background_.setTopLeft(-1.0f, 1.0f + y_offset);
    background_.setTopRight(-1.0 + 2.0f * width_ratio, 1.0f + y_offset);
    background_.setBottomLeft(-1.0f, 1.0f - 2.0f * height_ratio + y_offset);
    background_.setBottomRight(-1.0 + 2.0f * width_ratio, 1.0f - 2.0f * height_ratio + y_offset);

    background_.setColor(Colours::white);
    background_.drawImage(open_gl);
    SynthSection::renderOpenGlComponents(open_gl, animate);
}

void ModulesInterface::destroyOpenGlComponents(OpenGlWrapper& open_gl) {
    background_.destroy(open_gl);
    SynthSection::destroyOpenGlComponents(open_gl);
}

void ModulesInterface::scrollBarMoved(ScrollBar* scroll_bar, double range_start) {
    viewport_.setViewPosition(juce::Point<int>(0, range_start));
}

void ModulesInterface::setScrollBarRange() {
    scroll_bar_->setRangeLimits(0.0, container_->getHeight());
    scroll_bar_->setCurrentRange(scroll_bar_->getCurrentRangeStart(), viewport_.getHeight(), dontSendNotification);
    DBG("container height: " + String(container_->getHeight()));
    DBG("viewport height: " + String(viewport_.getHeight()));
    DBG("scrollbar range: " + String(scroll_bar_->getCurrentRangeStart()) );
}

ModuleSection* ModulesInterface::createNewObject (const juce::ValueTree& v)
{
    auto parent = findParentComponentOfClass<SynthGuiInterface>();
    LEAF* leaf = parent->getLEAF();
    std::any args = std::make_tuple( v,leaf );
    juce::AudioProcessor *proc;
    try {
      proc = factory.create(v.getProperty(IDs::type).toString().toStdString(),std::make_tuple( v,leaf ));
    } catch (const std::bad_any_cast& e) {
    std::cerr << "Error during object creation: " << e.what() << std::endl;
    }
    auto *module_section = new ModuleSection(v.getProperty(IDs::type).toString(), v, dynamic_cast<electrosynth::ParametersViewEditor*>(proc->createEditor()));
    container_->addSubSection(module_section);

    return module_section;

}

void ModulesInterface::newObjectAdded (ModuleSection*)
{
    resized();
}

void ModulesInterface::deleteObject (ModuleSection* at)
{

}

void ModulesInterface::valueTreeRedirected (juce::ValueTree&)
{

}