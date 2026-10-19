#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
build_dir=${KNOB_TEST_BUILD_DIR:-/private/tmp/knob-engine-unit-tests}
library=${1:-/private/tmp/knob-dsp-independent-sanitize/libknob-dsp.a}
mkdir -p "$build_dir"
xcrun clang++ -std=c++20 -O1 -g -Wall -Wextra -Werror -fsanitize=address,undefined -DKNOB_HAVE_DSP -I "$root/dsp/core/include" -I "$root/dsp/portable" "$root/tests/dsp-preset-storage-test.cpp" "$root/engine/dsp-controller.cpp" "$root/engine/dsp-controller-controls.cpp" "$root/engine/dsp-controller-storage.cpp" "$root/engine/audio-bridge.cpp" "$root/engine/audio-devices.cpp" "$library" -framework CoreAudio -framework CoreFoundation -o "$build_dir/dsp-preset-storage-test"
UBSAN_OPTIONS=halt_on_error=1 "$build_dir/dsp-preset-storage-test" "$root/presets/Default.fac"
