#!/bin/sh
set -eu
source_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
output_dir=${1:-/private/tmp/knob-driver-artifacts}
mkdir -p "$output_dir/Knob.driver/Contents/MacOS" "$output_dir/slices" "$output_dir/Knob.driver/Contents/Resources"
sdk=$(xcrun --sdk macosx --show-sdk-path)
for architecture in x86_64 arm64 arm64e; do
    xcrun clang -std=c11 -fvisibility=hidden -O2 -Wall -Wextra -Werror -arch "$architecture" \
        -isysroot "$sdk" -mmacosx-version-min=14.0 -bundle \
        -framework CoreAudio -framework CoreFoundation \
        "$source_dir"/src/*.c -o "$output_dir/slices/Knob-$architecture"
done
xcrun lipo -create "$output_dir/slices/Knob-x86_64" \
    "$output_dir/slices/Knob-arm64" "$output_dir/slices/Knob-arm64e" \
    -output "$output_dir/Knob.driver/Contents/MacOS/Knob"
cp "$source_dir/Info.plist" "$output_dir/Knob.driver/Contents/Info.plist"
cp "$source_dir/apple-sample-license.txt" "$output_dir/Knob.driver/Contents/Resources/Apple-License.txt"
plutil -lint "$output_dir/Knob.driver/Contents/Info.plist"
codesign --force --sign - --options runtime --timestamp=none "$output_dir/Knob.driver"
codesign --verify --strict --verbose=2 "$output_dir/Knob.driver"
xcrun lipo -info "$output_dir/Knob.driver/Contents/MacOS/Knob"
printf 'Built offline bundle: %s/Knob.driver\n' "$output_dir"
