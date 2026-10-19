#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
build_dir=${KNOB_TEST_BUILD_DIR:-/private/tmp/knob-engine-unit-tests}
mkdir -p "$build_dir"
optimized=${1:-/private/tmp/knob-dsp-independent-optimized/libknob-dsp.a}
sanitized=${2:-/private/tmp/knob-dsp-independent-sanitize/libknob-dsp.a}
xcrun clang++ -std=c++17 -O2 -fno-fast-math -ffp-contract=off -Wall -Wextra -Werror -I "$root/dsp/core/include" "$root/tests/dsp-engine-test.cpp" "$optimized" -o "$build_dir/dsp-engine-test"
"$build_dir/dsp-engine-test" "$root"
xcrun clang++ -std=c++17 -O2 -fno-fast-math -ffp-contract=off -Wall -Wextra -Werror -I "$root/dsp/core/include" "$root/tests/dsp-allocation-test.cpp" "$optimized" -o "$build_dir/dsp-allocation-test"
"$build_dir/dsp-allocation-test" "$root/presets/Default.fac"
xcrun clang++ -std=c++17 -O2 -g -fno-fast-math -ffp-contract=off -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer -I "$root/dsp/core/include" "$root/tests/dsp-engine-test.cpp" "$sanitized" -o "$build_dir/dsp-engine-test-sanitize"
UBSAN_OPTIONS=halt_on_error=1 "$build_dir/dsp-engine-test-sanitize" "$root"
xcrun clang++ -std=c++17 -O2 -g -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer -I "$root/dsp/core/include" "$root/tests/dsp-eq-resize-test.cpp" "$sanitized" -o "$build_dir/dsp-eq-resize-test"
UBSAN_OPTIONS=halt_on_error=1 "$build_dir/dsp-eq-resize-test" "$root/presets/Default.fac"
