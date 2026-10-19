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

/* knobpGet.cpp */

#include "codedefs.h"

#ifndef PT_PORTABLE_DSP
#include <windows.h>
#endif
#include <stdio.h>

#include "u_knobp.h" 

#include "knobp.h"
#ifdef PT_PORTABLE_DSP
#include "knobSharedUtil.h"
#endif
#include "KnobSdk.h"
#include "qnt.h"
#include "spectrum.h"

/*
 * FUNCTION: knobpGetKnobValue() 
 * DESCRIPTION:
 *  Passes back the value of the specified knob as a float.
 */
int knobpGetKnobValue(PT_HANDLE *hp_knobp, int i_knob_type, float *fp_value)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	realtype r_value;
	int i_midi_value;

	*fp_value = (float)0.0;

	if (knobp_GetKnobValue_MIDI(hp_knobp, i_knob_type, &i_midi_value) != OKAY)
		return(NOT_OKAY);

	/* Calulate the real value */
   if (qntIToRCalc(cast_handle->midi_to_real_qnt_hdl, i_midi_value, &r_value) != OKAY)
		return(NOT_OKAY);

	*fp_value = (float)r_value;

	return(OKAY);
}

/*
 * FUNCTION: knobp_GetKnobValue_MIDI() 
 * DESCRIPTION:
 *  Passes back the value of the specified knob in MIDI representation.
 */
int knobp_GetKnobValue_MIDI(PT_HANDLE *hp_knobp, int i_knob_type, int *ip_midi_value)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	wchar_t wcp_key_name[128];
	int default_value;

   if (ip_midi_value == NULL)
		return(NOT_OKAY);

	*ip_midi_value = 0;


	/***************************
	 * TAKE CARE OF KNOB CASE   *
	 ***************************/

   /* Hard code the vocal reduction value */
	if (i_knob_type == KNOB_UI_KNOB_VOCAL_REDUCTION)
	{
		*ip_midi_value = KNOBP_INIT_VOCAL_REDUCTION_MIDI_VAL;

		return(OKAY);
	}

   /* Determine the registry keyname and default value associated with the knob_type */
	if (i_knob_type == KNOB_UI_KNOB_FIDELITY)
	{
		default_value = KNOBP_INIT_FIDELITY_MIDI_VAL;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_VALUE_FIDELITY_WIDE);
	}
	else if (i_knob_type == KNOB_UI_KNOB_AMBIENCE)
	{
		default_value = KNOBP_INIT_AMBIENCE_MIDI_VAL;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_VALUE_AMBIENCE_WIDE);
	}
	else if (i_knob_type == KNOB_UI_KNOB_DYNAMIC_BOOST)
	{
		default_value = KNOBP_INIT_DYNAMIC_BOOST_MIDI_VAL;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_VALUE_DYNAMIC_BOOST_WIDE);
	}
	else if (i_knob_type == KNOB_UI_KNOB_SURROUND)
	{
		default_value = KNOBP_INIT_SURROUND_MIDI_VAL;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_VALUE_SURROUND_WIDE);
	}
	else if (i_knob_type == KNOB_UI_KNOB_BASS_BOOST)
	{
		default_value = KNOBP_INIT_BASS_BOOST_MIDI_VAL;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_VALUE_BASS_BOOST_WIDE);
	}
	else 
		return(OKAY);

	/* Get the midi value */
   if (knobp_SessionReadIntegerValue(hp_knobp, wcp_key_name, default_value, ip_midi_value) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

/*
 * FUNCTION: knobpGetButtonValue()
 * DESCRIPTION:
 *   Passes back the on/off value of the specified button.
 */
int knobpGetButtonValue(PT_HANDLE *hp_knobp, int i_button_type, int *ip_value)
{
	struct knobpHdlType *cast_handle;
	int default_bypass_value;
	int default_music_mode;
	int registry_value;
	wchar_t wcp_key_name[256];
	int use_opposite_of_registy_value;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	/* The vocal reduction values are special cases because they are always bypassed */
	if (i_button_type == KNOB_UI_BUTTON_VOCAL_REDUCTION_ON)
	{
		*ip_value = IS_FALSE;
		return(OKAY);
	}
	if (i_button_type == KNOB_UI_BUTTON_VOCAL_REDUCTION_MODE)
	{
		*ip_value = KNOB_UI_VOCAL_REDUCTION_MODE_1;
		return(OKAY);
	}
	/* The music mode is a special case because it can have several different values */
	if (i_button_type == KNOB_UI_BUTTON_MUSIC_MODE)
	{
		default_music_mode = KNOB_UI_MUSIC_MODE_MUSIC2;

		if (knobp_SessionReadIntegerValue(hp_knobp, KNOBP_REGISTRY_MODE_MUSIC_MODE_WIDE, default_music_mode, ip_value) != OKAY)
			return(NOT_OKAY);

		return(OKAY);
	}

   /* All of the remaining buttons are on/off buttons */
	if (i_button_type == KNOB_UI_BUTTON_BYPASS)
	{
		default_bypass_value = IS_FALSE;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_BYPASS_ALL_WIDE);
		use_opposite_of_registy_value = IS_FALSE;
	}
	else if (i_button_type == KNOB_UI_BUTTON_FIDELITY)
	{
		default_bypass_value = IS_FALSE;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_BYPASS_FIDELITY_WIDE);
		use_opposite_of_registy_value = IS_TRUE;
	}
	else if (i_button_type == KNOB_UI_BUTTON_AMBIENCE)
	{
		default_bypass_value = IS_FALSE;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_BYPASS_AMBIENCE_WIDE);
		use_opposite_of_registy_value = IS_TRUE;
	}
	else if (i_button_type == KNOB_UI_BUTTON_DYNAMIC_BOOST)
	{
		default_bypass_value = IS_FALSE;

		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_BYPASS_DYNAMIC_BOOST_WIDE);
		use_opposite_of_registy_value = IS_TRUE;
	}
	else if (i_button_type == KNOB_UI_BUTTON_SURROUND)
	{
		default_bypass_value = IS_FALSE;

		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_BYPASS_SURROUND_WIDE);
		use_opposite_of_registy_value = IS_TRUE;
	}
	else if (i_button_type == KNOB_UI_BUTTON_BASS_BOOST)
	{
		default_bypass_value = IS_FALSE;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_BYPASS_BASS_BOOST_WIDE);
		use_opposite_of_registy_value = IS_TRUE;
	}
	else if (i_button_type == KNOB_UI_BUTTON_HEADPHONE)
	{
		default_bypass_value = IS_TRUE;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_BYPASS_HEADPHONE_WIDE);
		use_opposite_of_registy_value = IS_TRUE;
	}
	else if (i_button_type == KNOB_UI_BUTTON_REMIX_BYPASS)
	{
		default_bypass_value = IS_FALSE;
		swprintf(wcp_key_name, L"%s", KNOBP_REGISTRY_REMIX_BYPASS_ALL_WIDE);
		use_opposite_of_registy_value = IS_FALSE;;
	}
	else
		return(NOT_OKAY);

	/* Read the value from the registry */
	if (knobp_SessionReadIntegerValue(hp_knobp, wcp_key_name, default_bypass_value, &registry_value) != OKAY)
		return(NOT_OKAY);

	if (use_opposite_of_registy_value)
		*ip_value = !registry_value;
	else
		*ip_value = registry_value;

	return(OKAY);
}

/*
 * FUNCTION: knobpGetSamplingFreq()
 * DESCRIPTION:
 *
 *  Passes back the current sampling frequency used for processing.
 *
 */
int knobpGetSamplingFreq(PT_HANDLE *hp_knobp, realtype *rp_sampling_freq)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

   *rp_sampling_freq = cast_handle->sampling_freq;

	return(OKAY);
}

/*
 * FUNCTION: knobpGetProcessingOverride() 
 * DESCRIPTION:
 *
 *  Gets the special KNOB processing override setting.  
 *  See knobpSetProcessingOverride() for description.
 *
 */
int knobpGetProcessingOverride(PT_HANDLE *hp_knobp, int *ip_knob_processing_override)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	*ip_knob_processing_override = cast_handle->processing_override;

	return(OKAY);
}

/*
 * FUNCTION: knobpGetFirstTimeRunFlag()
 * DESCRIPTION:
 *   Passes back a flag stating whether or not this is the first time the KNOB has been run since
 *   installation.
 */
int knobpGetFirstTimeRunFlag(PT_HANDLE *hp_knobp, int *ip_first_time_run_flag)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	*ip_first_time_run_flag = cast_handle->first_time_run_flag;

	return(OKAY);
}

/*
 * FUNCTION: knobpGetTemporaryBypassAll()
 * DESCRIPTION:
 *
 *  Gets whether or not to temporarily bypass all processing.
 */
int knobpGetTemporaryBypassAll(PT_HANDLE *hp_knobp, int *ip_temporary_bypass_all)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	int i_default_temporary_bypass_all;

	if (cast_handle == NULL)
		return(OKAY);

	i_default_temporary_bypass_all = IS_FALSE;

	if (knobp_SessionReadIntegerValue(hp_knobp, KNOBP_REGISTRY_TEMPORARY_BYPASS_ALL_WIDE, 
											   i_default_temporary_bypass_all, ip_temporary_bypass_all) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

/*
 * FUNCTION: knobpGetKnobTunedTrackPlaying()
 * DESCRIPTION:
 *
 *  Gets whether or not a KNOB printed track is currently playing (i.e. a song already processed by iKNOB).
 */
int knobpGetKnobTunedTrackPlaying(PT_HANDLE *hp_knobp, int *ip_knob_tuned_track_playing)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	int i_default_knob_tuned_track_playing;

	if (cast_handle == NULL)
		return(OKAY);

	i_default_knob_tuned_track_playing = IS_FALSE;

	if (knobp_SessionReadIntegerValue(hp_knobp, KNOBP_REGISTRY_KNOB_TUNED_TRACK_PLAYING_WIDE, 
												i_default_knob_tuned_track_playing, ip_knob_tuned_track_playing) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

/*
 * FUNCTION: knobpGetTotalAudioProcessedTime()
 * DESCRIPTION:
 *   Get the total audio processed time
 */
int knobpGetTotalAudioProcessedTime(PT_HANDLE *hp_knobp, unsigned long *ul_msec_audio_processed_time)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	*ul_msec_audio_processed_time = 0;

	if (cast_handle == NULL)
		return(OKAY);

	if (cast_handle->trace.mode)
		(cast_handle->slout1)->Message_Wide(FIRST_LINE, L"knobpGetTotalAudioProcessedTime: Entered");

	if (!(cast_handle->fully_initialized))
		return(NOT_OKAY);

	#ifdef PT_PORTABLE_DSP
	return knobSharedUtilGetTotalProcessedTime(cast_handle->hp_sharedUtil, ul_msec_audio_processed_time);
	#else
	*ul_msec_audio_processed_time = cast_handle->ul_total_msecs_audio_processed_time;
	#endif

	return(OKAY);
}
