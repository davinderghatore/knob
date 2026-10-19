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
#ifndef _U_KNOBP_H_
#define _U_KNOBP_H_ 

#include "pt_defs.h"
//#include "CWaveFile.h" // This must come first because it must come before any codedefs.h due to the mmgr module */
#include "slout.h"
//#include "daw.h"
#include "knobp.h"

// Defines the KNOB shared globals structure.
//#include "knobSharedGlobals.h"
 
/*************************
 *  Constants            *
 *************************/

 // This sets the size of allocated signal buffers, per input, so for a stereo signal the buffer is twice this size.
 // Up through KNOB 9.301 this was set to 65536L, but "smart" buffer handling allowed us to set this to a smaller size.
#define DAW_MAX_BUFFER_SIZE 16384L

 // Maximum supported internal (ie used for internal processing) and external
 // (ie used by application in calls to us) sampling rates.
 // When the application sample rate is larger then our max internal rate, down sampling is used.
#define DAW_INIT_SAMPLING_FREQ         44100.0
#define DAW_MAX_SAMPLING_FREQ				192000.0
#define DAW_MAX_INTERNAL_SAMPLING_FREQ 48000.0
#define DAW_MIN_SAMPLING_FREQ				16000.0
#define DAW_INIT_BITS_PER_SAMPLE        16
#define DAW_MAX_BITS_PER_SAMPLE         32
#define DAW_MIN_BITS_PER_SAMPLE         8
#define DAW_INIT_VALID_BITS             16
#define DAW_INIT_NUM_CHANNELS           2
#define DAW_MAX_NUM_CHANNELS            8


// Make buffer large enough to handle 7.1 8 channel surround sound
#define KNOBP_SAMPLE_BUFFER_SIZE (DAW_MAX_BUFFER_SIZE * 8)

#define KNOBP_AURAL_CONTROL_HERTZ_MIN_VAL 500.0
#define KNOBP_AURAL_CONTROL_HERTZ_MAX_VAL 10000.0 

#define KNOBP_INIT_SAMPLING_FREQ          DAW_INIT_SAMPLING_FREQ
#define KNOBP_MAX_SAMPLING_FREQ			  DAW_MAX_SAMPLING_FREQ
#define KNOBP_MAX_INTERNAL_SAMPLING_FREQ  DAW_MAX_INTERNAL_SAMPLING_FREQ
#define KNOBP_MIN_SAMPLING_FREQ			  DAW_MIN_SAMPLING_FREQ
#define KNOBP_INIT_BITS_PER_SAMPLE        DAW_INIT_BITS_PER_SAMPLE
#define KNOBP_MAX_BITS_PER_SAMPLE         DAW_MAX_BITS_PER_SAMPLE
#define KNOBP_MIN_BITS_PER_SAMPLE         DAW_MIN_BITS_PER_SAMPLE
#define KNOBP_INIT_VALID_BITS             DAW_INIT_VALID_BITS
#define KNOBP_INIT_NUM_CHANNELS           DAW_INIT_NUM_CHANNELS
#define KNOBP_MAX_NUM_CHANNELS            DAW_MAX_NUM_CHANNELS

#define KNOBP_DSP_FUNCTION_NAME           "ply0"

#define KNOBP_DSP_INTERNAL_BIT_WIDTH      32

#define KNOBP_SERIAL_NUMBER_LENGTH        8

/* NOTE: For Vocal Reduction the display value is used. NOT MIDI VALUE */
#define KNOBP_INIT_VOCAL_REDUCTION_MIDI_VAL       5 
#define KNOBP_REMIX_INIT_VOCAL_REDUCTION_MIDI_VAL 5 

/* Secret serial number for extending trial */
#define KNOBP_SECRET_EXTEND_TRIAL_SERIAL_WIDE L"extend"

/*
 * Default BPM to use if no accurate bmp has been determined yet.
 */
#define KNOBP_REMIX_DEFAULT_BPM                                 100.0

/* Beat Filtering Settings */
#define KNOBP_REMIX_FILTERED_FULL_BEAT_MIN_RYHTHMIC_NOTE         1.0

/* Factor by which to multiple real sampling frequency to get Beat Filtering samp freq */
#define KNOBP_REMIX_BEAT_FILTERING_BPM_FUDGE_FACTOR	1.04

/* Number of msecs since last buffer processed to assume playback has stopped */
#define KNOBP_MSECS_SINCE_LAST_BUFFER_TO_ASSUME_STOPPED 2000

/* Universal Format inital setting */
#define KNOBP_UNIVERSAL_FORMAT_UNSET -1

/* Size of the array of previous buffer hash values */
#define KNOBP_UNIVERSAL_HASH_QUEUE_SIZE 10

/**************************/
/* Structure definitions  */
/**************************/

struct midi_to_dsp_qnt_hdls {
   
   /* Knob based qnt handles */
   PT_HANDLE *fidelity_qnt_hdl;
	PT_HANDLE *spaciousness_qnt_hdl;
	PT_HANDLE *ambience_qnt_hdl;
	PT_HANDLE *dynamic_boost_qnt_hdl;
	PT_HANDLE *bass_boost_qnt_hdl;

	/* Fixed Aural Activation Specific */
	PT_HANDLE *aural_filter_gain_qnt_hdl;
	PT_HANDLE *aural_filter_a1_qnt_hdl;
	PT_HANDLE *aural_filter_a0_qnt_hdl;

	/* Fixed Reverb Specific */
   PT_HANDLE *room_size_qnt_hdl;
	PT_HANDLE *damping_bandwidth_qnt_hdl;
   PT_HANDLE *rolloff_bandwidth_qnt_hdl;
	PT_HANDLE *motion_rate_qnt_hdl;
	PT_HANDLE *motion_depth_qnt_hdl;
	/* Set up like full plugin screen display qnt hdls to init special dsp qnt hdls. */
	PT_HANDLE *screen_lex_main_knob3_qnt_hdl;
   PT_HANDLE *screen_lex_main_knob4_qnt_hdl;

	/* Fixed Optimizer Specific */
	PT_HANDLE *release_time_beta_qnt_hdl;
	/* Set up like full plugin screen display qnt hdls to init special dsp qnt hdls. */
   PT_HANDLE *screen_opt_main_knob3_qnt_hdl;

	/* Fixed Widener Specific */
	PT_HANDLE *dispersion_delay_qnt_hdl;
	PT_HANDLE *wid_filter_gain_qnt_hdl;
	PT_HANDLE *wid_filter_a1_qnt_hdl;
	PT_HANDLE *wid_filter_a0_qnt_hdl;

	/* Delay Specific */
	PT_HANDLE *dly_qnt_hdl;
};      

/****** Eq info *******/
struct knobp_eq_info_type {

	/* Graphic Eq handle */
	PT_HANDLE *graphicEq_hdl;

	int i_processing_on;
};

/* Trace info */
struct knobp_trace_info_type {
	int mode;
	int i_process_real_samples_done;
	int i_process_int_samples_done;
};

/* Spectrum info */
struct knobp_spectrum_info_type {
	PT_HANDLE *spectrum_hdl;
	int sample_sets_since_last_spectrum_save;
};


/* Shared memory */
/*
struct knobp_shared_memory_type {
	HINSTANCE hinst;
	struct knobSharedGlobalsType *sp_data;
};
*/

/* Universal UI usage settings */
struct knobp_universal_type {
   int last_called_nch;
   int last_called_bps;
   int last_called_srate;
   int last_called_valid_bits;

	/* For internal float buffer, MAX times 8 for 7.1 surround sound signals. */
	realtype f_samples[DAW_MAX_BUFFER_SIZE * 8];

	wchar_t wcp_knob_ui_path[PT_MAX_PATH_STRLEN]; /* C:\Program Files\KNOB\knob.exe" */

	/* Hash of previous buffer */
	int hash_queue_vals[KNOBP_UNIVERSAL_HASH_QUEUE_SIZE];
	int hash_queue_index;

	/* Allcaps version of fullpath to parent exe */
	wchar_t wcp_parent_exe_path_uppercase[PT_MAX_PATH_STRLEN];

	/* Flag needed for enumerating windows */
	int i_found_incompatable_website_for_processing;
};

/**************************
 *  Main KNOBP Handle Type *
 **************************/

struct knobpHdlType {
	
	/* Main product name */
	wchar_t wcp_product_name[PT_MAX_GENERIC_STRLEN];

	/* Fully initialized flag */
	int fully_initialized;

	/* Message info */
	wchar_t wcp_msg1[2048];
	char cp_msg1[2048];
   CSlout *slout1;  

	struct knobp_trace_info_type trace;

	/* Vendor specific info */
	int vendor_code;
   int subvendor_code;
	int i_freemium_version;
	int oem_build;
	int major_version; /* Major version number (ex. 7) */
	long l_host_buffer_delay_msecs;
	int processing_only;

	// Buffers for internal signal manipulations
	realtype r_samples[KNOBP_SAMPLE_BUFFER_SIZE];
	realtype r_samples_reordered[KNOBP_SAMPLE_BUFFER_SIZE];


	/* Qnt handles */
	struct midi_to_dsp_qnt_hdls midi_to_dsp;
	PT_HANDLE *real_to_midi_qnt_hdl;
	PT_HANDLE *midi_to_real_qnt_hdl;

	/* Spectrum info */
	struct knobp_spectrum_info_type spectrum;

	/* Signal info */
	realtype sampling_freq;
	realtype sampling_period; //Actual sampling frequency of processed signals
	realtype internal_sampling_freq; //Sampling frequency used internally for processing
	realtype internal_sampling_period; //Sampling period used internally for processing
	int internal_rate_ratio; //Integer ratio between actual sampling frequency and internal sampling frequency
   int bits_per_sample;
	int valid_bits;
	int unsupported_format_flag;

	/* These will have the same value except when using SurroundSyn functions */
   int num_channels_in;
   int num_channels_out;

	/* Processing Buffers */
   short int *si_input_samples;
   short int *si_output_samples;
	int numsamples;

	/* Communication handles to dsps */
	PT_HANDLE *com_hdl_front; 
	PT_HANDLE *com_hdl_rear; 
	PT_HANDLE *com_hdl_side; 
	PT_HANDLE *com_hdl_center; 
	PT_HANDLE *com_hdl_subwoofer; 

	/* Surround Synthesis handle (2 to 6/8 channel) */
	PT_HANDLE *SurroundSyn_hdl;

	/* Binaural Synthesis handle (6/8 to 2 channel) and binaural on/off flag */
	PT_HANDLE *BinauralSyn_hdl;
	int binaural_headphone_on_flag;

	int processing_override;

	/* Eq info */
	struct knobp_eq_info_type eq;

	/* Whether or not this is the first time run since installation */
	int first_time_run_flag;

	/* The function knobpCommunicateAllNonFixed() can be slowed down and utilize this count (see function for details) */
	int i_communicate_slowly_count;

	/* Total audio processed time in the session in milliseconds */
	unsigned long ul_total_msecs_audio_processed_time;

	/* Shared memory */
	PT_HANDLE *hp_sharedUtil;

	/* Settings when using the universal ui */
	struct knobp_universal_type universal;
};

/************************ 
 * Local Functions      *
 ************************/

/* knobpComm.cpp */
int knobp_CommunicateInit(PT_HANDLE *);
int knobp_ComLoadAndRun(PT_HANDLE *);
int knobp_CommunicateStereoInput(PT_HANDLE *);
int knobp_CommunicateBypassSettings(PT_HANDLE *);
int knobp_CommunicateFidelity(PT_HANDLE *);
int knobp_CommunicateAmbience(PT_HANDLE *);
int knobp_CommunicateDynamicBoost(PT_HANDLE *);
int knobp_CommunicateSpaciousness(PT_HANDLE *);
int knobp_CommunicateBassBoost(PT_HANDLE *);
int knobp_CommunicateVocalReduction(PT_HANDLE *);
int knobp_CommunicateFixedQnts_Aural(PT_HANDLE *);
int knobp_CommunicateFixedQnts_Lex(PT_HANDLE *);
int knobp_CommunicateFixedQnts_Opt(PT_HANDLE *);
int knobp_CommunicateFixedQnts_Wid(PT_HANDLE *);
int knobp_CommunicateFixedQnts_Delay(PT_HANDLE *);
int knobp_CommunicateFixedQnts_Play(PT_HANDLE *);
int knobp_AuralCommunicateTune(PT_HANDLE *);
int knobp_LexCommunicateSize(PT_HANDLE *);
int knobp_LexCommunicateRolloff(PT_HANDLE *);
int knobp_LexCommunicateDamping(PT_HANDLE *);
int knobp_LexCommunicateDepth(PT_HANDLE *);
int knobp_LexCommunicateRate(PT_HANDLE *);
int knobp_MaxCommunicateReleaseTime(PT_HANDLE *);
int knobp_WidCommunicateDispersion(PT_HANDLE *);
int knobp_WidCommunicateFreqThreshold(PT_HANDLE *);
int knobp_CommAmbienceBypass(PT_HANDLE *);
int knobp_CommunicateMusicMode(PT_HANDLE *);
int knobp_CommunicateAllFixed(PT_HANDLE *);

/* knobpEq.cpp */
int knobp_EqInit(PT_HANDLE *);

/* knobpGet.cpp */
int knobp_GetKnobValue_MIDI(PT_HANDLE *, int, int *);

/* knobpInit.cpp */
int knobp_InitFirstTimeRunFlag(PT_HANDLE *);

/* knobpProcess.cpp */
int knobp_CalcMsecsSinceLastBufferProcessed(PT_HANDLE *, int *);
int knobp_UpdateBufferLengthInfo(PT_HANDLE *, int, int *);
int knobp_StoreLongestBufferSize(PT_HANDLE *, int);

/* knobpProcessClear.cpp */
int knobp_ClearBuffersIfSongStart(PT_HANDLE *);

/* knobpQnt.cpp */
int knobp_InitAllQnts(PT_HANDLE *);
int knobp_InitStaticQnts(PT_HANDLE *);
int knobp_InitDynamicQnts(PT_HANDLE *);
int knobp_InitDynamicQnts_Aural(PT_HANDLE *);
int knobp_InitDynamicQnts_Lex(PT_HANDLE *);
int knobp_InitDynamicQnts_Opt(PT_HANDLE *);
int knobp_InitDynamicQnts_Wid(PT_HANDLE *);
int knobp_InitDynamicQnts_Play(PT_HANDLE *);
int knobp_InitDynamicQnts_Delay(PT_HANDLE *);

/* knobpQuit.cpp */
int knobp_FreeAll(PT_HANDLE *);

/* knobpRegistry.cpp */
int knobp_RecordLastUsedDate(PT_HANDLE *);
int knobp_RegistryGetTopSharedFolderPath(PT_HANDLE *, wchar_t *);
int knobp_RegistryGetTopVendorSpecificFolderPath(PT_HANDLE *, wchar_t *);
int knobp_RegistryGetKnobUniversalUiFullpath(PT_HANDLE *, wchar_t *);


/* knobpSession.cpp */
int knobp_SessionWriteIntegerValue(PT_HANDLE *, wchar_t *, int);
int knobp_SessionReadIntegerValue(PT_HANDLE *, wchar_t *, int, int *);
int knobp_SessionWriteRealValue(PT_HANDLE *, wchar_t *, realtype);
int knobp_SessionReadRealValue(PT_HANDLE *, wchar_t *, realtype, realtype *);
int knobp_CalcHowManyTimesRun(PT_HANDLE *, int *);
int knobp_CheckIfFirstTimeRun(PT_HANDLE *, int *);

/* knobpSet.cpp */
int knobp_SetKnobValue_MIDI(PT_HANDLE *, int, int, bool);
int knobpConvertIntToFaderRealValue(PT_HANDLE *, int, realtype *);
int knobp_KnobForWmpSetEnabledBasedOnBypass(PT_HANDLE *);

/* knobpSpectrum.cpp */
int knobp_SpectrumInit(PT_HANDLE *);
int knobp_SpectrumStoreCurrentValuesInSharedMemory(PT_HANDLE *, int);


/* knobpUniversal.cpp */
int knobp_UniversalCalcBufferHash(PT_HANDLE *, BYTE *, DWORD, int *);
int knobp_UniversalUpdateTotalTimeProcessed(PT_HANDLE *, int);
int knobp_UniversalIsBufferAllSilence(PT_HANDLE *, short int *, int, int, int, int *);



#endif //_U_KNOBP_H_
