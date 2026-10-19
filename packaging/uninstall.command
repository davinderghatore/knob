#!/bin/bash
set -euo pipefail
if pgrep -f '(^|/)knob-(engine|supervisor)([[:space:]]|$)' >/dev/null; then
    printf '%s\n' 'Quit Knob before uninstalling.'
    exit 1
fi
guard=/Applications/Knob.app/Contents/Helpers/audio-install-guard
if [[ ! -x "$guard" ]]; then
    printf '%s\n' 'The installed safety helper is missing. Uninstall refused.'
    exit 1
fi
"$guard"
/usr/bin/osascript <<'APPLESCRIPT'
do shell script "/Applications/Knob.app/Contents/Helpers/audio-install-guard && /bin/rm -rf /Library/Audio/Plug-Ins/HAL/Knob.driver /Applications/Knob.app && /usr/sbin/pkgutil --forget io.github.simranxdhindsa.knob && if /usr/bin/pgrep -x coreaudiod >/dev/null; then /usr/bin/killall coreaudiod; fi" with administrator privileges
APPLESCRIPT
