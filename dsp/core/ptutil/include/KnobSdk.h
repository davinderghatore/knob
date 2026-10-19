/*
FxSound
Copyright (C) 2025  FxSound LLC

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _KNOB_SDK_H_
#define _KNOB_SDK_H_

// BEGIN TIME TRIAL CODE
#include "dynamic_build_defines.h"
// END TIME TRIAL CODE

/* Boolean values defines */
#define KNOB_SDK_TRUE     1
#define KNOB_SDK_FALSE    0

/* KNOB SDK function return values */
#define KNOB_SDK_OKAY     0
#define KNOB_SDK_NOT_OKAY 1

/* KNOB Handle Definition */
typedef int KNOB_SDK_HANDLE;

/* 
 * The different types of knobs 
 */
#define KNOB_UI_KNOB_NONE            0
#define KNOB_UI_KNOB_FIDELITY        1
#define KNOB_UI_KNOB_SURROUND        2
#define KNOB_UI_KNOB_AMBIENCE        4
#define KNOB_UI_KNOB_DYNAMIC_BOOST   5 
#define KNOB_UI_KNOB_BASS_BOOST      6
#define KNOB_UI_KNOB_VOCAL_REDUCTION 7

/* HACK */
#define KNOB_UI_KNOB_VIDEO_PROCESS_AUDIO 8
#define KNOB_UI_KNOB_VIDEO_PROCESS_VIDEO 9

/* The different types of buttons - KNOB UI */
#define KNOB_UI_BUTTON_BYPASS                 12
#define KNOB_UI_BUTTON_FIDELITY               20
#define KNOB_UI_BUTTON_AMBIENCE               21
#define KNOB_UI_BUTTON_SURROUND               22
#define KNOB_UI_BUTTON_DYNAMIC_BOOST          23   
#define KNOB_UI_BUTTON_BASS_BOOST             24 
#define KNOB_UI_BUTTON_HEADPHONE              25 
#define KNOB_UI_BUTTON_MUSIC_MODE             26
#define KNOB_UI_BUTTON_VOCAL_REDUCTION_ON     27 
#define KNOB_UI_BUTTON_VOCAL_REDUCTION_MODE   28 

/* HACK */
#define KNOB_UI_BUTTON_VIDEO_PROCESS_AUDIO    29
#define KNOB_UI_BUTTON_VIDEO_PROCESS_VIDEO    30

/* The different types of buttons - Remix UI */
#define KNOB_UI_BUTTON_REMIX_BYPASS           100

/* 
 * The different modes for novox processing 
 */
#define KNOB_UI_VOCAL_REDUCTION_MODE_1       1
#define KNOB_UI_VOCAL_REDUCTION_MODE_2       2

/* 
 * The different modes for music processing 
 */
#define KNOB_UI_MUSIC_MODE_MUSIC1      1
#define KNOB_UI_MUSIC_MODE_MUSIC2      2
#define KNOB_UI_MUSIC_MODE_SPEECH      3

/* 
 * Button values.
 */
#define KNOB_UI_OFF_VALUE 0
#define KNOB_UI_ON_VALUE  1

/* 
 * Min and max values of knob settings
 */
#define KNOB_UI_MIN_VALUE						     0.0
#define KNOB_UI_MAX_VALUE							  1.0
/* BEGINE TIME TRIAL CODE */
#ifdef TIME_TRIAL_VERSION
#define KNOB_UI_MAX_TRIAL_VALUE					  1.0
#define KNOB_UI_MAX_TRIAL_VALUE_DETECT_CHEATING 1.0
#else
#define KNOB_UI_MAX_TRIAL_VALUE					  0.55
#define KNOB_UI_MAX_TRIAL_VALUE_DETECT_CHEATING 0.56
#endif
/* END TIME TIRAL CODE */

/*
 * Warping vals for mobile presets and mobile preset defines.
 */
#define KNOB_MUSIC_MODE2_AMBIENCE_FACTOR      (float)0.34
#define KNOB_MUSIC_MODE2_DYNAMIC_BOOST_FACTOR (float)1.8
#define KNOB_SPEECH_MODE_DYNAMIC_BOOST_FACTOR (float)1.8
#define KNOB_SPEECH_MODE_BASS_BOOST_FACTOR    (float)0.25
#define KNOB_SPEECH_MODE_FIDELITY_FACTOR      (float)1.6
#define KNOB_NUM_TRIAL_PRESETS 10
#define KNOB_NUM_PRO_PRESETS   46

/*
 * Factory preset names
 */
#define KNOB_FACTORY_PRESET_1 "No processing"
#define KNOB_FACTORY_PRESET_2 "MP3 Enhancer - Standard"
#define KNOB_FACTORY_PRESET_3 "MP3 Enhancer - Hi Volume"
#define KNOB_FACTORY_PRESET_4 "MP3 Enhancer - No Ambience"
#define KNOB_FACTORY_PRESET_5 "Streamcast - High Bit Rate Music"
#define KNOB_FACTORY_PRESET_6 "Streamcast - Low Bit Rate Music"
#define KNOB_FACTORY_PRESET_7 "Streamcast - Dialog"
#define KNOB_FACTORY_PRESET_8 "Large Ambience"
#define KNOB_FACTORY_PRESET_9 "Very Large Ambience"
#define KNOB_FACTORY_PRESET_10 "Concert Hall Ambience"

/* Registation modes */
#define KNOB_REG_UNREGISTERED           1
#define KNOB_REG_VERIFIED               2
#define KNOB_REG_NOT_YET_VERIFIED       3
#define KNOB_REG_TOO_MANY_REGISTRATIONS 4
#define KNOB_REG_ILLEGAL_SERIAL         5
#define KNOB_REG_OLD_STYLE_SERIAL       6
#define KNOB_REG_NO_ADMIN_PRIVILEDGES   7

/* Registration message max length */
#define KNOB_MESSAGE_MAX_LENGTH         1024

/* Functions */

#ifndef __ANDROID__
#ifdef __cplusplus
extern "C" {
#endif
int knobgInit(PT_HANDLE **, HWND, HINSTANCE, int, int, int, wchar_t *, wchar_t *, int, int, int, int, int, int, long, int, long, int, PT_HANDLE *, int *);
int knobgGetKnobHwnd(PT_HANDLE *, HWND *);
int knobgGetKnobpHandle(PT_HANDLE *, PT_HANDLE **);
int knobgSetParentWindow(PT_HANDLE *, HWND);
int knobgSetButtonValue(PT_HANDLE *, int, int);
int knobgGetPlaybackStopped(PT_HANDLE *, int *);
int knobgQuit(PT_HANDLE **);

int knobpBeginProcess(PT_HANDLE *, int, int, int);
int knobpModifyShortIntSamples(PT_HANDLE *, short int *, short int *, int);

int knobgHideMainWindow(PT_HANDLE *);
int knobgShowMainWindow(PT_HANDLE *);
int knobgRestoreMainWindow(PT_HANDLE *, int);
int knobgMinimizeMainWindow(PT_HANDLE *, int, int);

int knobgRemixCommunicateCheckForChanges(PT_HANDLE *);

int knobgBringUpRegisterDlgIfNotRegistered(PT_HANDLE *);

#ifdef __cplusplus
}
#endif
#endif //WIN32

#endif //_KNOB_SDK_H
