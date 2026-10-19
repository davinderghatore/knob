#pragma once
#include "audio-bridge.h"
#include "routing-guard.h"
#include "dsp-controller.h"

namespace knob {
std::string dispatchCommand(const std::string& request, AudioBridge& bridge, RoutingGuard& routing,
                            DSPController& dsp, std::string& outputUID, bool& activate, bool& shutdown);
}
