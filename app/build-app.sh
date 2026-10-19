#!/bin/bash
# Builds build/Knob.app from the Swift package. Usage: app/build-app.sh [debug|release]
set -euo pipefail
app_dir=$(cd -- "$(dirname -- "$0")" && pwd)
repo_dir=$(cd -- "$app_dir/.." && pwd)
config=${1:-release}
swift build -c "$config" --package-path "$app_dir"
bundle="$repo_dir/build/Knob.app"
rm -rf "$bundle"
mkdir -p "$bundle/Contents/MacOS" "$bundle/Contents/Resources/presets"
cp "$app_dir/.build/$config/Knob" "$bundle/Contents/MacOS/Knob"
cp "$repo_dir/presets/"*.fac "$bundle/Contents/Resources/presets/"
cp "$app_dir/Resources/AppIcon.icns" "$bundle/Contents/Resources/AppIcon.icns"
cat > "$bundle/Contents/Info.plist" <<'PLIST'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0"><dict>
<key>CFBundleIdentifier</key><string>io.github.simranxdhindsa.knob</string>
<key>CFBundleName</key><string>Knob</string>
<key>CFBundleDisplayName</key><string>Knob</string>
<key>CFBundleExecutable</key><string>Knob</string>
<key>CFBundleIconFile</key><string>AppIcon</string>
<key>CFBundlePackageType</key><string>APPL</string>
<key>CFBundleShortVersionString</key><string>0.1.0</string>
<key>CFBundleVersion</key><string>1</string>
<key>LSMinimumSystemVersion</key><string>14.0</string>
<key>LSUIElement</key><true/>
<key>NSHumanReadableCopyright</key><string>Free software under the GNU AGPL v3.</string>
<key>NSMicrophoneUsageDescription</key><string>Knob processes the sound from your apps and plays it on your speakers or display.</string>
<key>NSAudioCaptureUsageDescription</key><string>Knob processes the sound from your apps and plays it on your speakers or display.</string>
</dict></plist>
PLIST
plutil -lint "$bundle/Contents/Info.plist" >/dev/null
# The engine helpers (built by CMake into build/) go inside the app, which starts them.
entitlements="$repo_dir/packaging/audio-input.entitlements"
if [ -x "$repo_dir/build/knob-engine" ] && [ -x "$repo_dir/build/knob-supervisor" ]; then
    mkdir -p "$bundle/Contents/Helpers"
    for helper in knob-engine knob-supervisor; do
        cp "$repo_dir/build/$helper" "$bundle/Contents/Helpers/$helper"
        codesign --force --sign - --options runtime --timestamp=none --entitlements "$entitlements" \
            "$bundle/Contents/Helpers/$helper"
    done
else
    echo "Note: engine helpers not built yet (cmake --build build); the power switch can't start the engine." >&2
fi
# The audio device rides inside the app, which installs it on first use (one password prompt).
if [ -d "$repo_dir/build/driver/Knob.driver" ]; then
    ditto "$repo_dir/build/driver/Knob.driver" "$bundle/Contents/Resources/Knob.driver"
else
    echo "Note: audio device not built yet (bash driver/build.sh build/driver); Knob can't install it." >&2
fi
# Release builds carry their complete source code (AGPL-3.0), as the About window says.
if [ "$config" = release ] && git -C "$repo_dir" rev-parse --git-dir >/dev/null 2>&1; then
    python3 -I "$repo_dir/packaging/source-bundle.py" "$bundle/Contents/Resources" >/dev/null
    rm -f "$bundle/Contents/Resources/SOURCE_SHA256SUMS"
fi
codesign --force --sign - --options runtime --timestamp=none --entitlements "$entitlements" "$bundle"
codesign --verify --strict "$bundle"
echo "Built $bundle"
