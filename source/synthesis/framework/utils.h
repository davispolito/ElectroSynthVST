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

#pragma once
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "common.h"

#include <cmath>
#include <complex>
#include <cstdlib>
#include <random>
using namespace juce;


  namespace electrosynth::utils {
    constexpr float kDbGainConversionMult = 20.0f;
    constexpr int kMaxOrderLength = 10;
    constexpr float kLogOf2 = 0.69314718056f;
    constexpr float kInvLogOf2 = 1.44269504089f;

    constexpr int factorial(int value) {
      int result = 1;
      for (int i = 2; i <= value; ++i)
        result *= i;

      return result;
    }

    typedef union {
      int i;
      float f;
    } int_float;

    class RandomGenerator {
      public:
        static int next_seed_;
          
        RandomGenerator(float min, float max) : engine_(next_seed_++), distribution_(min, max) { }
        RandomGenerator(const RandomGenerator& other) :
            engine_(next_seed_++), distribution_(other.distribution_.min(), other.distribution_.max()) { }

        force_inline float next() {
          return distribution_(engine_);
        }


        force_inline void seed(int new_seed) {
          engine_.seed(new_seed);
        }

      private:
        std::mt19937 engine_;
        std::uniform_real_distribution<float> distribution_;

        JUCE_LEAK_DETECTOR(RandomGenerator)
    };

    force_inline float intToFloatBits(int i) {
      int_float convert;
      convert.i = i;
      return convert.f;
    }

    force_inline int floatToIntBits(float f) {
      int_float convert;
      convert.f = f;
      return convert.i;
    }

    force_inline float min(float one, float two) {
      return fmin(one, two);
    }

    force_inline float max(float one, float two) {
      return fmax(one, two);
    }

    force_inline float clamp(float value, float min, float max) {
      return fmin(max, fmax(value, min));
    }

    template<class T>
    force_inline T pass(T input) {
      return input;
    }

    force_inline int imax(int one, int two) {
      return (one > two) ? one : two;
    }

    force_inline int imin(int one, int two) {
      return (one > two) ? two : one;
    }

    force_inline double interpolate(double from, double to, double t) {
      return t * (to - from) + from;
    }

    force_inline float interpolate(float from, float to, float t) {
      return from + t * (to - from);
    }
    
    force_inline float mod(double value, double* divisor) {
      return modf(value, divisor);
    }

    force_inline float mod(float value, float* divisor) {
      return modff(value, divisor);
    }

    force_inline int iclamp(int value, int min, int max) {
      return value > max ? max : (value < min ? min : value);
    }

    force_inline int ilog2(int value) {
    #if defined(__GNUC__) || defined(__clang__)
      constexpr int kMaxBitIndex = sizeof(int) * 8 - 1;
      return kMaxBitIndex - __builtin_clz(std::max(value, 1));
    #elif defined(_MSC_VER)
      unsigned long result = 0;
      _BitScanReverse(&result, value);
      return result;
    #else
      int num = 0;
      while (value >>= 1)
        num++;
      return num;
    #endif
    }

    force_inline bool closeToZero(float value) {
      return value <= kEpsilon && value >= -kEpsilon;
    }

    force_inline float magnitudeToDb(float magnitude) {
      return kDbGainConversionMult * log10f(magnitude);
    }

    force_inline float dbToMagnitude(float decibels) {
      return powf(10.0f, decibels / kDbGainConversionMult);
    }

    force_inline float centsToRatio(float cents) {
      return powf(2.0f, cents / kCentsPerOctave);
    }

    force_inline float noteOffsetToRatio(float cents) {
      return powf(2.0f, cents / kNotesPerOctave);
    }

    force_inline float ratioToMidiTranspose(float ratio) {
      return logf(ratio) * (kInvLogOf2 * kNotesPerOctave);
    }

    force_inline float midiCentsToFrequency(float cents) {
      return kMidi0Frequency * centsToRatio(cents);
    }

    force_inline float midiNoteToFrequency(float note) {
      return midiCentsToFrequency(note * kCentsPerNote);
    }

    force_inline float frequencyToMidiNote(float frequency) {
      return kNotesPerOctave * logf(frequency / kMidi0Frequency) * kInvLogOf2;
    }

    force_inline float frequencyToMidiCents(float frequency) {
      return kCentsPerNote * frequencyToMidiNote(frequency);
    }

    force_inline int nextPowerOfTwo(float value) {
      return roundf(powf(2.0f, ceilf(logf(value) * kInvLogOf2)));
    }

    force_inline bool isSilent(const float* buffer, int length) {
      for (int i = 0; i < length; ++i) {
        if (!closeToZero(buffer[i]))
          return false;
      }
      return true;
    }





      static juce::String getMidiMessageDescription (const juce::MidiMessage& m)
      {
        if (m.isNoteOn())           return "Note on "          + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())          return "Note off "         + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())    return "Program change "   + juce::String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return "Pitch wheel "      + juce::String (m.getPitchWheelValue());
        if (m.isAftertouch())       return "After touch "      + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + juce::String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return "Channel pressure " + juce::String (m.getChannelPressureValue());
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";

        if (m.isController())
        {
          juce::String name (juce::MidiMessage::getControllerName (m.getControllerNumber()));

          if (name.isEmpty())
            name = "[" + juce::String (m.getControllerNumber()) + "]";

          return "Controller " + name + ": " + juce::String (m.getControllerValue());
        }

        return juce::String::toHexString (m.getRawData(), m.getRawDataSize());
      }
      static juce::String printMidi(juce::MidiMessage& message, const juce::String& source)
      {

        auto time = message.getTimeStamp(); //- startTime;

        auto hours   = ((int) (time / 3600.0)) % 24;
        auto minutes = ((int) (time / 60.0)) % 60;
        auto seconds = ((int) time) % 60;
        auto millis  = ((int) (time * 1000.0)) % 1000;

        auto timecode = juce::String::formatted ("%02d:%02d:%02d.%03d",
            hours,
            minutes,
            seconds,
            millis);

        auto description = getMidiMessageDescription (message);

        return juce::String(timecode + "  -  " + description + " (" + source + ")"); // [7]
      }


  } // namespace electrosynth::utils
// namespace vital

