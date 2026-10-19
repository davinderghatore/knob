#!/bin/bash
# Builds the Knob installer: a disk image whose window says "Drag Knob into Applications".
# Usage: packaging/build-dmg.sh [path/to/Knob.app] [output-directory]
# Needs dmgbuild (uv tool install dmgbuild). The app installs its own audio device on first use.
set -euo pipefail
packaging_dir=$(cd -- "$(dirname -- "$0")" && pwd)
repo_dir=$(cd -- "$packaging_dir/.." && pwd)
app=$(cd -- "$(dirname -- "${1:-$repo_dir/build/Knob.app}")" && pwd)/$(basename -- "${1:-$repo_dir/build/Knob.app}")
output=${2:-$repo_dir/dist}

command -v dmgbuild >/dev/null || { echo "dmgbuild is missing: uv tool install dmgbuild" >&2; exit 1; }
[ -d "$app/Contents/Resources/Knob.driver" ] || { echo "Knob.app has no audio device inside; build it first." >&2; exit 1; }
codesign --verify --deep --strict "$app"

version=$(/usr/libexec/PlistBuddy -c 'Print :CFBundleShortVersionString' "$app/Contents/Info.plist")
work=$(mktemp -d /private/tmp/knob-dmg.XXXXXX)
trap 'rm -rf "$work"' EXIT

# The background is drawn by the app itself (same design system), at 1x and 2x for Retina.
"$app/Contents/MacOS/Knob" --icons "$work/art" >/dev/null
sips -z 440 660 "$work/art/installer-background.png" --out "$work/background.png" >/dev/null
cp "$work/art/installer-background.png" "$work/background@2x.png"
tiffutil -cathidpicheck "$work/background.png" "$work/background@2x.png" -out "$work/background.tiff" >/dev/null

mkdir -p "$output"
dmg="$output/Knob-$version.dmg"
rm -f "$dmg"
dmgbuild -s "$packaging_dir/dmg-settings.py" -D app="$app" -D background="$work/background.tiff" "Knob" "$dmg"
shasum -a 256 "$dmg" > "$dmg.sha256"
echo "Installer: $dmg"
