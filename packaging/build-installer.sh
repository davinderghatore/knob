#!/bin/bash
set -euo pipefail
packaging_dir=$(cd -- "$(dirname -- "$0")" && pwd)
repo_dir=$(cd -- "$packaging_dir/.." && pwd)
app=${1:?Pass the built Knob.app path}
driver=${2:?Pass the built Knob.driver path}
output=${3:-$repo_dir/dist}
# Same certificate as app/build-app.sh (KNOB_SIGN_IDENTITY), so the installed app keeps its permissions.
identity=${KNOB_SIGN_IDENTITY:--}
version=$(python3 - "$app/Contents/Info.plist" <<'PY'
import plistlib
import sys
with open(sys.argv[1], 'rb') as source:
    print(plistlib.load(source)['CFBundleShortVersionString'])
PY
)
for bundle in "$app" "$driver"; do
    codesign --verify --deep --strict "$bundle"
done
mkdir -p "$output"
output=$(cd -- "$output" && pwd)
staging=$(mktemp -d /private/tmp/knob-package.XXXXXX)
trap 'rm -rf "$staging"' EXIT
mkdir -p "$staging/root/Applications" "$staging/root/Library/Audio/Plug-Ins/HAL" "$staging/scripts" "$staging/dmg"
ditto --norsrc --noextattr --noacl "$app" "$staging/root/Applications/Knob.app"
ditto --norsrc --noextattr --noacl "$driver" "$staging/root/Library/Audio/Plug-Ins/HAL/Knob.driver"
xattr -cr "$staging/root"
cp "$packaging_dir/scripts/preinstall" "$staging/scripts/preinstall"
cp "$packaging_dir/scripts/postinstall" "$staging/scripts/postinstall"
for arch in x86_64 arm64; do
    xcrun clang -O2 -Wall -Wextra -Werror -arch "$arch" -mmacosx-version-min=14.0 \
        -framework CoreAudio -framework CoreFoundation "$packaging_dir/audio-install-guard.c" \
        -o "$staging/guard-$arch"
done
xcrun lipo -create "$staging/guard-x86_64" "$staging/guard-arm64" -output "$staging/scripts/audio-install-guard"
codesign --sign - --options runtime --timestamp=none "$staging/scripts/audio-install-guard"
chmod 755 "$staging/scripts/"*
cp "$staging/scripts/audio-install-guard" "$staging/root/Applications/Knob.app/Contents/Helpers/audio-install-guard"
for target in knob-engine knob-supervisor; do
    codesign --force --sign "$identity" ${KNOB_SIGN_KEYCHAIN:+--keychain "$KNOB_SIGN_KEYCHAIN"} --options runtime --timestamp=none \
        --entitlements "$packaging_dir/audio-input.entitlements" \
        "$staging/root/Applications/Knob.app/Contents/Helpers/$target"
done
codesign --force --sign "$identity" ${KNOB_SIGN_KEYCHAIN:+--keychain "$KNOB_SIGN_KEYCHAIN"} --options runtime --timestamp=none \
    --entitlements "$packaging_dir/app.entitlements" "$staging/root/Applications/Knob.app"
codesign --verify --deep --strict "$staging/root/Applications/Knob.app"
# The same app, zipped, is what installed copies download as an update (no driver change).
ditto -c -k --sequesterRsrc --keepParent "$staging/root/Applications/Knob.app" "$output/Knob-$version.zip"
codesign --verify --strict "$staging/root/Library/Audio/Plug-Ins/HAL/Knob.driver"
# Install exactly into /Applications and the HAL folder: without this, macOS may "update" some other
# copy of Knob.app it finds on the disk (a build folder, a drag-installed copy) instead.
pkgbuild --analyze --root "$staging/root" "$staging/components.plist" >/dev/null
python3 - "$staging/components.plist" <<'PY2'
import plistlib
import sys
with open(sys.argv[1], 'rb') as source:
    components = plistlib.load(source)
for component in components:
    component['BundleIsRelocatable'] = False
with open(sys.argv[1], 'wb') as output:
    plistlib.dump(components, output)
PY2
pkgbuild --root "$staging/root" --component-plist "$staging/components.plist" --scripts "$staging/scripts" --ownership recommended \
    --identifier io.github.simranxdhindsa.knob --version "$version" --install-location / \
    "$staging/knob-component.pkg"
python3 - "$staging/root/Applications/Knob.app/Contents/MacOS/Knob" "$staging/requirements.plist" <<'PY'
import plistlib
import subprocess
import sys
architectures = subprocess.check_output(['xcrun', 'lipo', '-archs', sys.argv[1]], text=True).split()
with open(sys.argv[2], 'wb') as output:
    plistlib.dump({'os': ['14.0'], 'arch': architectures}, output)
PY
productbuild --synthesize --product "$staging/requirements.plist" \
    --package "$staging/knob-component.pkg" "$staging/Distribution.xml"
productbuild --distribution "$staging/Distribution.xml" --package-path "$staging" \
    "$staging/dmg/Knob-Install.pkg"
cp "$packaging_dir/installer-readme.txt" "$staging/dmg/READ-ME.txt"
cp "$packaging_dir/uninstall.command" "$staging/dmg/Uninstall-Knob.command"
chmod 755 "$staging/dmg/Uninstall-Knob.command"
python3 -I "$packaging_dir/source-bundle.py" "$staging/dmg"
hdiutil create -ov -format UDZO -volname Knob -srcfolder "$staging/dmg" "$output/Knob.dmg"
cp "$staging/dmg/Knob-Install.pkg" "$output/Knob-Install.pkg"
shasum -a 256 "$output/Knob.dmg" "$output/Knob-Install.pkg" > "$output/SHA256SUMS"
pkgutil --payload-files "$staging/knob-component.pkg" > "$output/package-payload.txt"
printf 'Installer: %s/Knob.dmg\n' "$output"
