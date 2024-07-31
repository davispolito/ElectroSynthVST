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


namespace ProjectInfo
{
    const char* const  projectName    = "ElectroSynth";
    const char* const  companyName    = "Snyderphonics";
    const char* const  versionString  = "0.1";
    const int          versionNumber  = 0x00100;
}
#include <map>

class SynthBase;


class MidiManager;

class Startup {
  public:
    static void doStartupChecks(MidiManager* midi_manager);
    static bool isComputerCompatible();

  private:
    Startup() = delete;
};

