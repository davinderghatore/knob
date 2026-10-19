/*
FxSound
Copyright (C) 2025  FxSound LLC

Contributors:
	www.theremino.com (2025)
	
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
#ifndef _KNOBPDEFS_H_
#define _KNOBPDEFS_H_

//#include "vendor_codes.h"

/* Specify extra functionality */
#define KNOB_FUNCTIONALITY_VOCALS      IS_FALSE

/* The different types of Remix meters */
#define KNOBP_REMIX_METER_TYPE_LOOPS     1
#define KNOBP_REMIX_METER_TYPE_SONG      2
#define KNOBP_REMIX_METER_TYPE_MIX		 3

/******************************************************
 * Registry information                               *
 ******************************************************/
#define KNOBP_REGISTRY_BUFFER_LENGTH					 PT_MAX_PATH_STRLEN

#define KNOBP_REGISTRY_TOP_WIDE                        L"SOFTWARE"
#define KNOBP_REGISTRY_LASTUSED_WIDE                   L"LASTUSED_KNOBP"
#define KNOBP_REGISTRY_DATE_LASTUSED_WIDE              L"date_last_used"
#define KNOBP_REGISTRY_DATE_INSTALLED_WIDE             L"date_installed"
#define KNOBP_REGISTRY_INSTALLED_LANGUAGE_WIDE			L"installed_language"

#define KNOBP_REGISTRY_SHARED_WIDE                     L"Shared"
#define KNOBP_REGISTRY_TOP_SHARED_FOLDER_WIDE          L"top_shared_folder"
#define KNOBP_REGISTRY_TOP_USERS_FOLDER_WIDE           L"top_users_folder"
#define KNOBP_REGISTRY_TOP_FOLDER_WIDE						L"top_folder"
#define KNOBP_REGISTRY_KNOB_UNIVERSAL_UI_PATH_WIDE		L"knobui_path"

/* Registry Values */
#define KNOBP_REGISTRY_VALUE_FIDELITY_WIDE             L"valFidelity"
#define KNOBP_REGISTRY_VALUE_AMBIENCE_WIDE             L"valAmbience"
#define KNOBP_REGISTRY_VALUE_DYNAMIC_BOOST_WIDE        L"valDynamicBoost"
#define KNOBP_REGISTRY_VALUE_SURROUND_WIDE             L"valSurround"
#define KNOBP_REGISTRY_VALUE_BASS_BOOST_WIDE           L"valBassBoost"
#define KNOBP_REGISTRY_VALUE_VOCAL_REDUCTION_WIDE      L"valVocalReduction"
#define KNOBP_REGISTRY_VALUE_SPECTRUM_BAND_WIDE        L"valSpectrumBand"

/* Registry Bypass */
#define KNOBP_REGISTRY_BYPASS_ALL_WIDE             L"byAll"
#define KNOBP_REGISTRY_BYPASS_FIDELITY_WIDE        L"byFidelity"
#define KNOBP_REGISTRY_BYPASS_AMBIENCE_WIDE        L"byAmbience"
#define KNOBP_REGISTRY_BYPASS_DYNAMIC_BOOST_WIDE   L"byDynamicBoost"
#define KNOBP_REGISTRY_BYPASS_SURROUND_WIDE        L"bySurround"
#define KNOBP_REGISTRY_BYPASS_BASS_BOOST_WIDE      L"byBassBoost"
#define KNOBP_REGISTRY_BYPASS_HEADPHONE_WIDE       L"byHeadphone"
#define KNOBP_REGISTRY_BYPASS_VOCAL_REDUCTION_WIDE L"byVocalReduction"

/* Registry Bypass - Remix */
#define KNOBP_REGISTRY_REMIX_BYPASS_ALL_WIDE				L"remixBypassAll"
#define KNOBP_REGISTRY_REMIX_MIX_LEVEL_WIDE				L"mix_level"
#define KNOBP_REGISTRY_MODE_VOCAL_REDUCTION_WIDE			L"modeVocalReduction"
#define KNOBP_REGISTRY_MODE_MUSIC_MODE_WIDE				L"modeMusicMode"
#define KNOBP_REGISTRY_TEMPORARY_BYPASS_ALL_WIDE			L"temporaryBypassAll"
#define KNOBP_REGISTRY_KNOB_TUNED_TRACK_PLAYING_WIDE		L"knobTunedTrackPlaying"

/* EQ Registry Strings */
#define KNOBP_REGISTRY_EQ_FOLDER_NAME_WIDE				L"EQ"
#define KNOBP_REGISTRY_EQ_ON_WIDE							L"on"
#define KNOBP_REGISTRY_EQ_BAND_NAME_WIDE				L"Band"

#define KNOBP_REGISTRY_REGISTRATION_WIDE          L"REGISTRATION"
#define KNOBP_REGISTRY_INSTALLATION_WIDE          L"INSTALLATION"
#define KNOBP_REGISTRY_SERIAL_NUM_WIDE            L"serialNumber"
#define KNOBP_REGISTRY_PASSWORD_WIDE              L"password"
#define KNOBP_REGISTRY_STATUS_WIDE                L"stat" 
#define KNOBP_REGISTRY_REG_COUNT_WIDE             L"regcount"
#define KNOBP_REGISTRY_EMAIL_ADDRESS_WIDE         L"email"
#define KNOBP_REGISTRY_EMAIL_SENT_WIDE            L"email_sent"
#define KNOBP_REGISTRY_QUICK_UNINSTALL_WIDE       L"quick_uninstall"
#define KNOBP_REGISTRY_TIMES_RUN_WIDE             L"times_run"

#define KNOBP_REGISTRY_LAST_BUFFER_SYSTEM_MSECS_WIDE L"last_buffer_system_msecs"
#define KNOBP_REGISTRY_LONGEST_BUFFER_MSECS_WIDE		 L"longest_buffer_msecs"

/* Recording Info */
#define KNOBP_REGISTRY_RECORDING_WIDE						 L"recording"
#define KNOBP_REGISTRY_RECORDING_BUTTON_ON_WIDE			 L"button_on"
#define KNOBP_REGISTRY_RECORDING_WRITTING_ON_WIDE		 L"writting_on"
#define KNOBP_REGISTRY_RECORDING_RESUME_ON_STARTUP_WIDE L"resume_on_startup"
#define KNOBP_REGISTRY_RECORDING_SECS_RECORDED_WIDE     L"secs_recorded"
#define KNOBP_REGISTRY_RECORDING_LIMIT_IN_MINUTES_WIDE  L"limit_minutes"
#define KNOBP_REGISTRY_RECORDING_COMPRESSION_RATE_WIDE  L"compression_rate"
#define KNOBP_REGISTRY_RECORDING_RAW_PATH_WIDE			 L"raw_fullpath"

#define KNOBP_REGISTRY_RECORDING_WAV_FULLPATH_WIDE L"wav_fullpath"
#define KNOBP_REGISTRY_RECORDING_WMA_FULLPATH_WIDE L"wma_fullpath"
#define KNOBP_REGISTRY_RECORDING_WMA_CHECKED_WIDE  L"wma_checked"

/* Remix Specific Info */
#define KNOBP_REGISTRY_VALUE_REMIX_FLASH_WIDE			   L"valRemixFlash"
#define KNOBP_REGISTRY_VALUE_REMIX_ACTIVE_MEASURE_WIDE L"valRemixActiveMeasure"
#define KNOBP_REGISTRY_VALUE_REMIX_VU_LOOPS_WIDE		   L"valRemixVuLoops"
#define KNOBP_REGISTRY_VALUE_REMIX_VU_SONG_WIDE		   L"valRemixVuSong"
#define KNOBP_REGISTRY_VALUE_REMIX_VU_MIX_WIDE		   L"valRemixVuMix"

/* Trial period defines */
#define KNOBP_STANDARD_NUM_FIRST_TRIAL_DAYS    14
#define KNOBP_STANDARD_NUM_EXTEND_TRIAL1_DAYS  1
#define KNOBP_STANDARD_NUM_EXTEND_TRIAL2_DAYS  7

/* Defines used for DSP math */
#define KNOBP_BYTES_PER_SAMPLE 2
#define KNOBP_BITS_PER_BYTE    8

/* Defines for music mode implementation */
#define KNOBP_MUSIC_MODE2_AMBIENCE_FACTOR 0.34
#define KNOBP_MUSIC_MODE2_DYNAMIC_BOOST_FACTOR 1.8
#define KNOBP_SPEECH_MODE_DYNAMIC_BOOST_FACTOR 1.8
#define KNOBP_SPEECH_MODE_BASS_BOOST_FACTOR 0.25
#define KNOBP_SPEECH_MODE_FIDELITY_FACTOR 1.6

/* Defines for the value passed to knobpSetProcessingOverride() */
#define KNOBP_PROCESSING_OVERRIDE_NONE                      0
#define KNOBP_PROCESSING_OVERRIDE_DYNAMIC_BOOST             1
#define KNOBP_PROCESSING_OVERRIDE_KNOB_ALL_EXCEPT_OPTIMIZER  2

/* The seeds which determine where the hidden trial period
 * files are.
 *
 * Use guidgen.exe to generate these numbers.  Guidgen.exe can be
 * found under C:\Program Files\Microsoft Visual Studio\Common\Tools
 *
 */
#define KNOBP_FIRST_TRIAL_SEED                0x419b30d8
#define KNOBP_EXTEND_TRIAL1_SEED              0xd6f65c1e
#define KNOBP_EXTEND_TRIAL2_SEED              0x8537c0aa

/****************************
 * SPECTRUM SETTINGS        *
 ****************************/
#define KNOBP_SPECTRUM_NUM_BANDS						10


/* Number of samples per each save of the spectrum to the registry */
/* Note - up to KNOB release 9.204, this was set at 512 */
/* It was raised to 1024 to reduce the number of registry writes required */
#define KNOBP_SAMPLE_SETS_PER_SAVE_SPECTRUM 1024

/* 
 * This number should be based on the KNOBP_SAMPLE_SETS_PER_SAVE_SPECTRUM using 
 * roughly the following formula:
 * refresh in ms = (num_sample_sets * 1000 ) / 44100.0 = num_sample_sets/44.1 ms
 *
 * Note: We don't want to refesh faster than 40 msec
 */
#define KNOBP_SPECTRUM_REFRESH_RATE_MSECS		40

/****************************
 * REMIX METER SETTINGS     *
 ****************************/
#define KNOBP_REMIX_METER_NUM_VALUES 20

/*******************************************
 * REMIX SAMPLE RATE RATIO LIMITS SETTINGS *
 *******************************************/
#define KNOBP_REMIX_SAMPLE_RATE_RATIO_MAX 4
#define KNOBP_REMIX_SAMPLE_RATE_RATIO_MIN 1

/********************************
 * PLAYER SPECIFIC BUFFER DELAY *
 ********************************/
#define KNOBP_HOST_BUFFER_DELAY_MSECS_CALC_AT_RUNTIME -1L

#define KNOBP_HOST_BUFFER_DELAY_MSECS_PRE_VISTA_WMP  930L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_VISTA_WMP		 200L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_JRIVER			 700L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_WINAMP			 2000L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_JETAUDIO		 2000L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_BSPLAYER		 2000L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_DSOUND			 300L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_WINMM			 0L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_FOOBAR2000		 1500L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_GOM_PLAYER		 1500L
#define KNOBP_HOST_BUFFER_DELAY_MSECS_MEDIA_MONKEY	 1500L

/* If unable to read the buffer delay at runtime then use this setting */
#define KNOBP_HOST_BUFFER_DELAY_MSECS_WINAMP_WHEN_UNABLE_TO_READ 2000L

/********************************
 * INITIAL KNOB SETTINGS        *
 ********************************/
#define KNOBP_INIT_FIDELITY_MIDI_VAL        51
#define KNOBP_INIT_AMBIENCE_MIDI_VAL        51
#define KNOBP_INIT_SURROUND_MIDI_VAL        26
#define KNOBP_INIT_DYNAMIC_BOOST_MIDI_VAL   51
#define KNOBP_INIT_BASS_BOOST_MIDI_VAL      68

/* DEFAULT DB VALUE FOR BAND1 - IT MUST MATCH DEFAULT FOR BASS BOOST */
#define KNOBP_DEFAULT_BAND1_DB_VAL			 5.35

#define KNOBP_REMIX_INIT_FIDELITY_MIDI_VAL        25
#define KNOBP_REMIX_INIT_AMBIENCE_MIDI_VAL        0
#define KNOBP_REMIX_INIT_SURROUND_MIDI_VAL        0
#define KNOBP_REMIX_INIT_DYNAMIC_BOOST_MIDI_VAL   0
#define KNOBP_REMIX_INIT_BASS_BOOST_MIDI_VAL      34

#define KNOBP_REMIX_INIT_MIX_LEVEL					 0.5

/*********************************
 * RECORDING SETTINGS            *
 *********************************/
#define KNOBP_DEFAULT_RECORDING_LIMIT_IN_MINUTES  10.0

/*********************************
 * FOLDER AND FILE NAMES         *
 *********************************/
#define KNOBP_APPS_DIR_WIDE                    L"Apps"  
#define KNOBP_WAV_TO_WMA_EXE_WIDE              L"WavToWma.exe"  
#define KNOBP_WM_ENCODER_INSTALLATION_EXE_WIDE L"WMEncoder.exe" 
#define KNOBP_CONTROL_WINDOW_APP_EXE_WIDE      L"knobgApp.exe"
#define KNOBP_LAST_BUFFER_TIMESTAMP_FILENAME	 L"knobBufferTimestamp.txt"
#define KNOBP_EQ_SKIN_DIR_WIDE						 L"EqSkin"  
#define KNOBP_KNOB_SKIN_DIR_WIDE				 L"KnobSkin"  

/*********************************
 * Third Party Defines           *
 *********************************/
#define KNOBP_SRS_SETTING_REG_FOLDER_WIDE	L"Software\\Microsoft\\MediaPlayer\\Preferences\\EqualizerSettings"
#define KNOBP_SRS_SETTING_REG_KEYNAME_WIDE L"EnhancedAudio"

/*********************************
 * Shared Memory Defines         *
 *********************************/
#define KNOBP_SHARED_MEMORY_FLAG_PROCESSING_STARTED 2

/*********************************
 * KNOB UNIVERSAL UI APP Defines  *
 *********************************/
#define KNOB_UNIVERSAL_UI_EXE_FILENAME_WIDE L"knob.exe"

/*******************************************
 * Universal Processing Module Types       *
 *******************************************/
#define KNOB_UNIVERSAL_PROCESSING_TYPE_UNDEFINED	 0
#define KNOB_UNIVERSAL_PROCESSING_TYPE_DSOUND_DLL 1
#define KNOB_UNIVERSAL_PROCESSING_TYPE_WINMM_DLL	 2

/*******************************************
 * EQ Defines                              *
 *******************************************/
extern int KNOBP_GRAPHIC_EQ_NUM_BANDS;
#define KNOBP_GRAPHIC_EQ_MIN_BOOST_OR_CUT_DB	-12.0 
#define KNOBP_GRAPHIC_EQ_MAX_BOOST_OR_CUT_DB	12.0 

/*******************************************
 * Storage Types                           *
 *******************************************/
#define KNOBP_STORAGE_TYPE_REGISTRY	1
#define KNOBP_STORAGE_TYPE_MEMORY		2
#define KNOBP_STORAGE_TYPE_ALL			3

#endif //_KNOBPDEFS_H_