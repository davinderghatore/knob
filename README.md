# Knob

A macOS menu bar app for system-wide sound. Knob gives you a working volume control on outputs that have none (like HDMI monitors), plus presets, effects and a 10-band equalizer.

## How it works

```
apps ──▶ Knob audio device (driver/)      the Mac's default output; the volume keys control it
           │  loopback
           ▼
         knob-engine (engine/ + dsp/)       processes the sound and plays it on your real output
           ▲  local socket
           │
         Knob menu bar app (app/)           the controls you see
```

## Status

| Part | State |
|---|---|
| Audio device (driver) | Working, tests pass |
| Engine and sound processing | Working, tests pass |
| Menu bar app | In progress |
| Installer | Scripts ready, used once the app exists |

## Build

Needs macOS 14 or later, Apple Command Line Tools and CMake (`brew install cmake`).

```sh
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=14.0
cmake --build build -j8                  # knob-engine, knob-supervisor, sound processing library
bash driver/build.sh "$PWD/build/driver" # Knob.driver
```

Tests: see [docs/testing.md](docs/testing.md).

## License

Free software under the GNU Affero General Public License v3. See [LICENSE](LICENSE) and [NOTICE.md](NOTICE.md). If you share Knob with anyone, share its source code too.
