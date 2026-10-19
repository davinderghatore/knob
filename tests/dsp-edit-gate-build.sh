#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
build_dir=${KNOB_TEST_BUILD_DIR:-/private/tmp/knob-engine-unit-tests}
library=${1:-/private/tmp/knob-dsp-independent-tsan/libknob-dsp.a}
mkdir -p "$build_dir"
flags="-fsanitize=thread"
if [ "${KNOB_TEST_ALLOCATIONS:-0}" = 1 ]; then flags="-DKNOB_TEST_ALLOCATIONS"; fi
xcrun clang++ -std=c++20 -O1 -g -Wall -Wextra -Werror $flags -DKNOB_HAVE_DSP -I "$root/dsp/core/include" -I "$root/dsp/portable" "$root/tests/dsp-edit-gate-test.cpp" "$root/engine/dsp-controller.cpp" "$root/engine/dsp-controller-controls.cpp" "$root/engine/dsp-controller-storage.cpp" "$root/engine/audio-bridge.cpp" "$root/engine/audio-devices.cpp" "$library" -framework CoreAudio -framework CoreFoundation -o "$build_dir/dsp-edit-gate-test"
TSAN_OPTIONS=halt_on_error=1 "$build_dir/dsp-edit-gate-test" "$root/presets/Default.fac"
