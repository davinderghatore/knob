#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
build_dir=${KNOB_TEST_BUILD_DIR:-/private/tmp/knob-engine-unit-tests}
library=${1:-/private/tmp/knob-dsp-independent-tsan/libknob-dsp.a}
mkdir -p "$build_dir"
xcrun clang++ -std=c++17 -O2 -g -Wall -Wextra -Werror -fsanitize=thread -I "$root/dsp/core/include" "$root/tests/dsp-concurrency-test.cpp" "$library" -o "$build_dir/dsp-concurrency-test"
TSAN_OPTIONS=halt_on_error=1 "$build_dir/dsp-concurrency-test" "$root/presets/Default.fac"
