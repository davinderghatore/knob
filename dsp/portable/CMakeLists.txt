cmake_minimum_required(VERSION 3.22)
project(KnobDSP LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
get_filename_component(KNOB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
include(dsp-sources.cmake)
add_library(knob-dsp STATIC ${KNOB_DSP_SOURCES} preset-validation.cpp eq-resize.cpp dsp-layout.cpp float32-engine.cpp portable-runtime.cpp portable-files.cpp portable-settings.cpp portable-shared.cpp portable-logging.cpp
 "${KNOB_ROOT}/dsp/math/MthUtil.cpp"
 "${KNOB_ROOT}/dsp/math/Mthfreq.cpp"
 "${KNOB_ROOT}/dsp/math/MthBuffer.cpp")
target_compile_features(knob-dsp PUBLIC cxx_std_17)
target_include_directories(knob-dsp PUBLIC "${KNOB_ROOT}/dsp/core/include" "${CMAKE_CURRENT_SOURCE_DIR}"
 PRIVATE "${KNOB_ROOT}/dsp/core/ptutil/include")
target_compile_definitions(knob-dsp PRIVATE PT_PORTABLE_DSP PT_NON_MFC DSPSOFT_TARGET PT_DSP_BUILD=PT_DSP_KNOB)
target_compile_options(knob-dsp PRIVATE -O2 -fno-fast-math -ffp-contract=off
 -Wno-writable-strings -Wno-deprecated-declarations
 -Werror=pointer-to-int-cast -Werror=int-to-pointer-cast
 "SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/portable-runtime.h")
option(KNOB_SANITIZE "Enable address and undefined behavior sanitizers" OFF)
if(KNOB_SANITIZE)
 target_compile_options(knob-dsp PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
 target_link_options(knob-dsp PUBLIC -fsanitize=address,undefined)
endif()
add_executable(knob-render ../tools/offline-render.cpp ../tools/wav-file.cpp)
target_link_libraries(knob-render PRIVATE knob-dsp)
add_executable(knob-compare ../tools/compare-renders.cpp ../tools/wav-file.cpp)
target_compile_features(knob-compare PRIVATE cxx_std_17)
