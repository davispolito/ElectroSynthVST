/* Copyright 2013-2019 Matt Tytel
 *
 * vital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * vital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with vital.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "synth_base.h"
#include "synth_gui_interface.h"


#include "startup.h"
#include "../synthesis/synth_engine/sound_engine.h"

#include "Identifiers.h"


SynthBase::SynthBase() : expired_(false) {

  self_reference_ = std::make_shared<SynthBase*>();
  *self_reference_ = this;

  engine_ = std::make_unique<electrosynth::SoundEngine>();

  LEAF_init(&leaf, 44100.0f, dummy_memory, 60000, [](){return (float)rand()/RAND_MAX;});

  float params[OscNumParams] = {0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

  tOscModule_init(reinterpret_cast<void** const> (&module), params, 0, &leaf);


  osc = new OscillatorModuleProcessor(*module);


  keyboard_state_ = std::make_unique<MidiKeyboardState>();
  midi_manager_ = std::make_unique<MidiManager>( keyboard_state_.get(), this);

  last_played_note_ = 0.0f;
  last_num_pressed_ = 0;





  Startup::doStartupChecks(midi_manager_.get());
  tree = ValueTree(IDs::TREE);
  tree.addListener(this);
}

SynthBase::~SynthBase() {
  tree.removeListener(this);
}

//void SynthBase::valueChanged(const std::string& name, float value) {
//
//}
//
//void SynthBase::valueChangedInternal(const std::string& name, float value) {
//  valueChanged(name, value);
//  setValueNotifyHost(name, value);
//}

//void SynthBase::valueChangedThroughMidi(const std::string& name, float value) {
//
//  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, name, value);
//  setValueNotifyHost(name, value);
//  callback->post();
//}

void SynthBase::pitchWheelMidiChanged(float value) {
  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, "pitch_wheel", value);
  callback->post();
}

void SynthBase::modWheelMidiChanged(float value) {
  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, "mod_wheel", value);
  callback->post();
}

void SynthBase::pitchWheelGuiChanged(float value) {
  engine_->setZonedPitchWheel(value, 0, electrosynth::kNumMidiChannels - 1);
}

void SynthBase::modWheelGuiChanged(float value) {
  engine_->setModWheelAllChannels(value);
}

void SynthBase::presetChangedThroughMidi(File preset) {
  SynthGuiInterface* gui_interface = getGuiInterface();
  if (gui_interface) {
    gui_interface->updateFullGui();
    gui_interface->notifyFresh();
  }
}
//
//void SynthBase::valueChangedExternal(const std::string& name, float value) {
//  valueChanged(name, value);
//  if (name == "mod_wheel")
//    engine_->setModWheelAllChannels(value);
//  else if (name == "pitch_wheel")
//    engine_->setZonedPitchWheel(value, 0, electrosynth::kNumMidiChannels - 1);
//
//  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, name, value);
//  callback->post();
//}














void SynthBase::initEngine()
{
  checkOversampling();
}












void SynthBase::setMpeEnabled(bool enabled) {
  midi_manager_->setMpeEnabled(enabled);
}


juce::AudioProcessorGraph::Node::Ptr SynthBase::addProcessor(std::unique_ptr<juce::AudioProcessor> processor)
{
    processor->prepareToPlay(engine_->getSampleRate(), engine_->getBufferSize());
    return engine_->addNode(std::move(processor));
}

juce::AudioProcessorGraph::Node * SynthBase::getNodeForId(AudioProcessorGraph::NodeID id)
{
return engine_->processorGraph->getNodeForId(id);
}

void SynthBase::addConnection(AudioProcessorGraph::Connection &connect)
{
    engine_->processorGraph->addConnection(connect);
}
void SynthBase::processAudio(AudioSampleBuffer* buffer, int channels, int samples, int offset) {
  if (expired_)
    return;
  AudioThreadAction action;
  while (processorInitQueue.try_dequeue (action))
      action();
  engine_->process(samples, *buffer);
  writeAudio(buffer, channels, samples, offset);
}
void SynthBase::processAudioAndMidi(juce::AudioBuffer<float>& audio_buffer, juce::MidiBuffer& midi_buffer) //, int channels, int samples, int offset, int start_sample = 0, int end_sample = 0)
{

    if (expired_)
        return;
    AudioThreadAction action;
    while (processorInitQueue.try_dequeue (action))
        action();

    engine_->processorGraph->processBlock(audio_buffer, midi_buffer);

}
void SynthBase::processAudioWithInput(AudioSampleBuffer* buffer, const float* input_buffer,
                                      int channels, int samples, int offset) {
  if (expired_)
    return;

  engine_->processWithInput(input_buffer, samples);
  writeAudio(buffer, channels, samples, offset);
}

void SynthBase::writeAudio(AudioSampleBuffer* buffer, int channels, int samples, int offset) {
  //const float* engine_output = (const float*)engine_->output(0)->buffer;
  /* get output of engine here */
  for (int channel = 0; channel < channels; ++channel) {
    float* channel_data = buffer->getWritePointer(channel, offset);
    //this line actually sends audio to the JUCE AudioSamplerBuffer to get audio out of the plugin
    for (int i = 0; i < samples; ++i) {
      //channel_data[i] = engine_output[float::kSize * i + channel];
      ELECTROSYNTH_ASSERT(std::isfinite(channel_data[i]));
    }
  }
 /*this line would send audio out to draw and get info from */
  //updateMemoryOutput(samples, engine_->output(0)->buffer);
}

void SynthBase::processMidi(MidiBuffer& midi_messages, int start_sample, int end_sample) {
  bool process_all = end_sample == 0;
  for (const MidiMessageMetadata message : midi_messages) {
    int midi_sample = message.samplePosition;
    if (process_all || (midi_sample >= start_sample && midi_sample < end_sample))
      midi_manager_->processMidiMessage(message.getMessage(), midi_sample - start_sample);
  }
}

void SynthBase::processKeyboardEvents(MidiBuffer& buffer, int num_samples) {
  midi_manager_->replaceKeyboardMessages(buffer, num_samples);
}



void SynthBase::updateMemoryOutput(int samples, const float* audio) {
//  for (int i = 0; i < samples; ++i)
//    audio_memory_->push(audio[i]);
//
//  float last_played = engine_->getLastActiveNote();
//  last_played = electrosynth::utils::clamp(last_played, kOutputWindowMinNote, kOutputWindowMaxNote);
//
//  int num_pressed = engine_->getNumPressedNotes();
//  int output_inc = std::max<int>(1, engine_->getSampleRate() / electrosynth::kOscilloscopeMemorySampleRate);
//  int oscilloscope_samples = 2 * electrosynth::kOscilloscopeMemoryResolution;
//
//  if (last_played && (last_played_note_ != last_played || num_pressed > last_num_pressed_)) {
//    last_played_note_ = last_played;
//
//    //electrosynth::utils::copyBuffer(oscilloscope_memory_, oscilloscope_memory_write_, oscilloscope_samples);
//  }
//  last_num_pressed_ = num_pressed;
//
////  for (; memory_input_offset_ < samples; memory_input_offset_ += output_inc) {
////    int input_index = electrosynth::utils::iclamp(memory_input_offset_, 0, samples);
////    memory_index_ = electrosynth::utils::iclamp(memory_index_, 0, oscilloscope_samples - 1);
////    ELECTROSYNTH_ASSERT(input_index >= 0);
////    ELECTROSYNTH_ASSERT(input_index < samples);
////    ELECTROSYNTH_ASSERT(memory_index_ >= 0);
////    ELECTROSYNTH_ASSERT(memory_index_ < oscilloscope_samples);
////    //oscilloscope_memory_write_[memory_index_++] = audio[input_index];
////
////    if (memory_index_ * output_inc >= memory_reset_period_) {
////      memory_input_offset_ += memory_reset_period_ - memory_index_ * output_inc;
////      memory_index_ = 0;
////      //electrosynth::utils::copyBuffer(oscilloscope_memory_, oscilloscope_memory_write_, oscilloscope_samples);
////    }
////  }
//
//  memory_input_offset_ -= samples;
}

//void SynthBase::armMidiLearn(const std::string& name) {
//  midi_manager_->armMidiLearn(name);
//}
//
//void SynthBase::cancelMidiLearn() {
//  midi_manager_->cancelMidiLearn();
//}
//
//void SynthBase::clearMidiLearn(const std::string& name) {
//  midi_manager_->clearMidiLearn(name);
//}

void SynthBase::valueChanged(const std::string& name, float value) {
//  controls_[name]->set(value);
}



void SynthBase::valueChangedThroughMidi(const std::string& name, float value) {
//  controls_[name]->set(value);
//  ValueChangedCallback* callback = new ValueChangedCallback(self_reference_, name, value);
//  setValueNotifyHost(name, value);
//  callback->post();
}

int SynthBase::getSampleRate() {
  return engine_->getSampleRate();
}

bool SynthBase::isMidiMapped(const std::string& name) {
  return midi_manager_->isMidiMapped(name);
}

void SynthBase::setAuthor(const String& author) {
  save_info_["author"] = author;
}

void SynthBase::setComments(const String& comments) {
  save_info_["comments"] = comments;
}

void SynthBase::setStyle(const String& style) {
  save_info_["style"] = style;
}

void SynthBase::setPresetName(const String& preset_name) {
  save_info_["preset_name"] = preset_name;
}

void SynthBase::setMacroName(int index, const String& macro_name) {
  save_info_["macro" + std::to_string(index + 1)] = macro_name;
}

String SynthBase::getAuthor() {
  return save_info_["author"];
}

String SynthBase::getComments() {
  return save_info_["comments"];
}

String SynthBase::getStyle() {
  return save_info_["style"];
}

String SynthBase::getPresetName() {
  return save_info_["preset_name"];
}






void SynthBase::notifyOversamplingChanged() {
  pauseProcessing(true);
  engine_->allSoundsOff();
  checkOversampling();
  pauseProcessing(false);
}

void SynthBase::checkOversampling() {
  return engine_->checkOversampling();
}

void SynthBase::ValueChangedCallback::messageCallback() {
  if (auto synth_base = listener.lock()) {
    SynthGuiInterface* gui_interface = (*synth_base)->getGuiInterface();
    if (gui_interface) {
      gui_interface->updateGuiControl(control_name, value);
      if (control_name != "pitch_wheel")
        gui_interface->notifyChange();
    }
  }
}

juce::ValueTree& SynthBase::getValueTree()
{
  return tree;
}

juce::UndoManager& SynthBase::getUndoManager()
{
  return um;
}
