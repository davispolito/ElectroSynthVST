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

#include "utils.h"

namespace electrosynth {

     juce::String getMidiMessageDescription (const juce::MidiMessage& m)
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
     juce::String printMidi(juce::MidiMessage& message, const juce::String& source)
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

  } // namespace utils
} // namespace vital
