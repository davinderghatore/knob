#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
build_dir=${KNOB_TEST_BUILD_DIR:-/private/tmp/knob-engine-unit-tests}
mkdir -p "$build_dir"
xcrun clang++ -std=c++17 -O1 -g -Wall -Wextra -Werror -fsanitize=address,undefined "$root/driver-bundle-test.cpp" -framework CoreAudio -framework CoreFoundation -o "$build_dir/driver-bundle-test"
"$build_dir/driver-bundle-test" "${1:-/private/tmp/knob-driver-artifacts/Knob.driver}"
