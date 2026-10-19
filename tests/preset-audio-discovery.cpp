#include "../engine/audio-devices.h"
std::string presetTestDefaultUID() { return knob::deviceUID(knob::defaultOutput()); }
std::string presetTestDevicesJSON() { return knob::devicesJSON(); }
