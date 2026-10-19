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
cat > "$bundle/Contents/Info.plist" <<'PLIST'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0"><dict>
<key>CFBundleIdentifier</key><string>io.github.simranxdhindsa.knob</string>
<key>CFBundleName</key><string>Knob</string>
<key>CFBundleDisplayName</key><string>Knob</string>
<key>CFBundleExecutable</key><string>Knob</string>
<key>CFBundlePackageType</key><string>APPL</string>
<key>CFBundleShortVersionString</key><string>0.1.0</string>
<key>CFBundleVersion</key><string>1</string>
<key>LSMinimumSystemVersion</key><string>14.0</string>
<key>LSUIElement</key><true/>
<key>NSHumanReadableCopyright</key><string>Free software under the GNU AGPL v3.</string>
</dict></plist>
PLIST
plutil -lint "$bundle/Contents/Info.plist" >/dev/null
codesign --force --sign - --options runtime --timestamp=none "$bundle"
codesign --verify --strict "$bundle"
echo "Built $bundle"
