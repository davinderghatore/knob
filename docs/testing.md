# Testing

Run from the repository root after building (see the README). None of these tests change your Mac's audio settings; the device-read tests only read the current devices.

## Extra sound-processing builds

The tests use three builds of the sound processing library: optimized, memory-checked (ASan/UBSan) and thread-checked (TSan).

```sh
cmake -S dsp/portable -B build/dsp-opt -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_DEPLOYMENT_TARGET=14.0
cmake --build build/dsp-opt
cmake -S dsp/portable -B build/dsp-asan -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_DEPLOYMENT_TARGET=14.0 \
  -DKNOB_SANITIZE=ON
cmake --build build/dsp-asan
cmake -S dsp/portable -B build/dsp-tsan -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_DEPLOYMENT_TARGET=14.0 \
  -DCMAKE_C_FLAGS=-fsanitize=thread -DCMAKE_CXX_FLAGS=-fsanitize=thread
cmake --build build/dsp-tsan
```

In zsh, pass each `-D` flag as its own word. Putting both sanitizer flags in one variable silently drops the C++ one.

## Test suite

```sh
L=$PWD/build
KNOB_TEST_IPC=1 KNOB_TEST_DEVICE_READ=1 sh tests/build.sh
sh tests/dsp-build.sh $L/dsp-opt/libknob-dsp.a $L/dsp-asan/libknob-dsp.a
KNOB_TEST_DEVICE_READ=1 sh tests/engine-dsp-build.sh $L/dsp-opt/libknob-dsp.a
sh tests/dsp-preset-storage-build.sh $L/dsp-asan/libknob-dsp.a
sh tests/dsp-preset-switch-build.sh $L/dsp-opt/libknob-dsp.a
KNOB_TEST_FLAGS="-fsanitize=address,undefined" sh tests/dsp-preset-switch-build.sh $L/dsp-asan/libknob-dsp.a
sh tests/dsp-audible-signal-build.sh $L/dsp-asan/libknob-dsp.a
sh tests/dsp-concurrency-build.sh $L/dsp-tsan/libknob-dsp.a
sh tests/dsp-edit-gate-build.sh $L/dsp-tsan/libknob-dsp.a
KNOB_TEST_ALLOCATIONS=1 sh tests/dsp-edit-gate-build.sh $L/dsp-opt/libknob-dsp.a
sh tests/driver-build.sh $L/driver/Knob.driver
```

## Sound regression check

`build/dsp-opt/knob-render input.wav output.wav [preset.fac|-]` renders a WAV file through the sound processing, and `knob-compare reference.wav render.wav` compares two renders. Before changing anything in `dsp/`, render a test file with every preset in `presets/`, then confirm the renders afterwards are bit-identical (`cmp`) unless the change is meant to alter the sound.
