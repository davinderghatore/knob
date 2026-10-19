#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
build_dir=${KNOB_TEST_BUILD_DIR:-/private/tmp/knob-engine-unit-tests}
library=${1:-$root/build/dsp-opt/libknob-dsp.a}
flags=${KNOB_TEST_FLAGS:--O2 -g}
mkdir -p "$build_dir"
xcrun clang++ -std=c++17 -Wall -Wextra -Werror $flags -DPT_PORTABLE_DSP -DPT_NON_MFC \
    -DDSPSOFT_TARGET -DPT_DSP_BUILD=PT_DSP_KNOB -include "$root/dsp/portable/portable-runtime.h" \
    -I "$root/dsp/core/include" -I "$root/dsp/core/ptutil/include" -I "$root/dsp/portable" \
    "$root/tests/dsp-preset-switch-test.cpp" "$library" -o "$build_dir/dsp-preset-switch-test"
UBSAN_OPTIONS=halt_on_error=1 "$build_dir/dsp-preset-switch-test" "$root/presets"
