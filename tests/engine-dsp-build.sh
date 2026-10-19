#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
build_dir=${KNOB_TEST_BUILD_DIR:-/private/tmp/knob-engine-unit-tests}
library=${1:-/private/tmp/knob-dsp-independent-optimized/libknob-dsp.a}
mkdir -p "$build_dir"
xcrun clang++ -std=c++20 -O1 -g -Wall -Wextra -Werror -fobjc-arc -DKNOB_HAVE_DSP -I "$root/dsp/core/include" -I "$root/dsp/portable" "$root/tests/command-dsp-schema-test.mm" "$root/engine/command-dispatcher.mm" "$root/engine/dsp-controller.cpp" "$root/engine/dsp-controller-controls.cpp" "$root/engine/dsp-controller-storage.cpp" "$root/engine/audio-bridge.cpp" "$root/engine/audio-devices.cpp" "$library" -framework CoreAudio -framework CoreFoundation -framework Foundation -o "$build_dir/command-dsp-schema-test"
if [ "${KNOB_TEST_DEVICE_READ:-0}" = 1 ]; then "$build_dir/command-dsp-schema-test"; fi
