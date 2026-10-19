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

/* knobpSet.cpp */

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
//#include "knobForWmpDefs.h"
#include "reg.h"
#include "midi.h"

/*
 * FUNCTION: knobpSetKnobValue() 
 * DESCRIPTION:
 *   Sets the passed knob to the specified value (in float representation).
 */
int knobpSetKnobValue(PT_HANDLE *hp_knobp, int i_knob_type, float f_value, bool b_from_eq_change)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	int midi_val;

   if (!(cast_handle->fully_initialized))
		return(NOT_OKAY);

	/* Make sure it is in the proper range */
	if ((f_value < KNOB_UI_MIN_VALUE) || (f_value > KNOB_UI_MAX_VALUE))
		return(NOT_OKAY);
 
	/* Convert the passed real value to the proper MIDI value */
	if (qntRToICalc(cast_handle->real_to_midi_qnt_hdl, (realtype)f_value, &midi_val) != OKAY)
		return(NOT_OKAY);

	/* Set the midi representation of the value */
	if (knobp_SetKnobValue_MIDI(hp_knobp, i_knob_type, midi_val, b_from_eq_change) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

/*
 * FUNCTION: knobp_SetKnobValue_MIDI() 
 * DESCRIPTION:
 *   Sets the passed knob to the specified value (in MIDI representation).
 */
int knobp_SetKnobValue_MIDI(PT_HANDLE *hp_knobp, int i_knob_type, int i_midi_value, bool b_from_eq_change)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	realtype r_eq_setting_normalized;
	realtype r_eq_setting_db;

   if (!(cast_handle->fully_initialized))
		return(NOT_OKAY);

	/* Make sure it is in the proper range */
	if ((i_midi_value < MIDI_MIN_VALUE) || (i_midi_value > MIDI_MAX_VALUE))
		return(NOT_OKAY);

	/* Take care of the specific knobs */
   if (i_knob_type == KNOB_UI_KNOB_FIDELITY)
	{
		if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_VALUE_FIDELITY_WIDE, i_midi_value) != OKAY)
			return(NOT_OKAY);
      if (knobp_CommunicateFidelity(hp_knobp) != OKAY)
         return(NOT_OKAY);
	}
	else if (i_knob_type == KNOB_UI_KNOB_AMBIENCE)
	{   
		if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_VALUE_AMBIENCE_WIDE, i_midi_value) != OKAY)
			return(NOT_OKAY);
      if (knobp_CommunicateAmbience(hp_knobp) != OKAY)
         return(NOT_OKAY);
	}
	else if (i_knob_type == KNOB_UI_KNOB_DYNAMIC_BOOST)
	{
		if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_VALUE_DYNAMIC_BOOST_WIDE, i_midi_value) != OKAY)
			return(NOT_OKAY);
      if (knobp_CommunicateDynamicBoost(hp_knobp) != OKAY)
         return(NOT_OKAY);
	}
	else if (i_knob_type == KNOB_UI_KNOB_SURROUND)
	{
		if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_VALUE_SURROUND_WIDE, i_midi_value) != OKAY)
			return(NOT_OKAY);
      if (knobp_CommunicateSpaciousness(hp_knobp) != OKAY)
         return(NOT_OKAY);
	}
	else if (i_knob_type == KNOB_UI_KNOB_BASS_BOOST)
	{
		if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_VALUE_BASS_BOOST_WIDE, i_midi_value) != OKAY)
			return(NOT_OKAY);
      if (knobp_CommunicateBassBoost(hp_knobp) != OKAY)
         return(NOT_OKAY);

		/* 
		 * Move the EQ setting of band 1 to match the new HyperBass setting. 
		 * NOTE: Make sure not to do this if this call came as a result of a change to EQ band1 trying to move the
		 *       the HyperBass slider to match the EQ.  Otherwise, we will end up in an endless loop.
		 */


	}
	
	return(OKAY);
}

/*
 * FUNCTION: knobpSetButtonValue()
 * DESCRIPTION:
 *   Sets the passed button to the specified value on/off value.
 */
int knobpSetButtonValue(PT_HANDLE *hp_knobp, int i_button_type, int i_value)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

   if (!(cast_handle->fully_initialized))
		return(NOT_OKAY);

	/* Take care of the non-bypass type buttons */
	if (i_button_type == KNOB_UI_BUTTON_MUSIC_MODE)
	{
		if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_MODE_MUSIC_MODE_WIDE, i_value) != OKAY)
			return(NOT_OKAY);
      if (knobp_CommunicateMusicMode(hp_knobp) != OKAY)
         return(NOT_OKAY);
	}
	else if (i_button_type == KNOB_UI_BUTTON_BYPASS)
	{
      if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_BYPASS_ALL_WIDE, i_value) != OKAY)
	      return(NOT_OKAY);
		if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
			return(NOT_OKAY);
	}
	else if (i_button_type == KNOB_UI_BUTTON_FIDELITY)
	{
	   if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_BYPASS_FIDELITY_WIDE, !(i_value)) != OKAY)
	      return(NOT_OKAY);
		if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
			return(NOT_OKAY);
	}
	else if (i_button_type == KNOB_UI_BUTTON_AMBIENCE)
	{   
	   if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_BYPASS_AMBIENCE_WIDE, !(i_value)) != OKAY)
	      return(NOT_OKAY);
		if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
			return(NOT_OKAY);
	}
	else if (i_button_type == KNOB_UI_BUTTON_DYNAMIC_BOOST)
	{
	   if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_BYPASS_DYNAMIC_BOOST_WIDE, !(i_value)) != OKAY)
	      return(NOT_OKAY);
		if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
			return(NOT_OKAY);
	}
	else if (i_button_type == KNOB_UI_BUTTON_SURROUND)
	{
	   if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_BYPASS_SURROUND_WIDE, !(i_value)) != OKAY)
	      return(NOT_OKAY);
		if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
			return(NOT_OKAY);
	}
	else if (i_button_type == KNOB_UI_BUTTON_BASS_BOOST)
	{
	   if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_BYPASS_BASS_BOOST_WIDE, !(i_value)) != OKAY)
	      return(NOT_OKAY);
		if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
			return(NOT_OKAY);
	}
	else if (i_button_type == KNOB_UI_BUTTON_HEADPHONE)
	{
      if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_BYPASS_HEADPHONE_WIDE, !(i_value)) != OKAY)
	      return(NOT_OKAY);
		if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
			return(NOT_OKAY);
	}
	else if (i_button_type == KNOB_UI_BUTTON_REMIX_BYPASS)
	{
      if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_REMIX_BYPASS_ALL_WIDE, i_value) != OKAY)
	      return(NOT_OKAY);
		if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
			return(NOT_OKAY);
	}

	return(OKAY);
}

/*
 * FUNCTION: knobpConvertFaderRealValueToInt()
 * DESCRIPTION:
 *
 * Takes the passed fader real value and converts it to the proper number to display.
 *
 */
int knobpConvertFaderRealValueToInt(PT_HANDLE *hp_knobp, realtype r_value, int *ip_value, int i_quantized)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	if (i_quantized)
	{
	   if (r_value < (realtype)0.05)
	      *ip_value = 0;
	   else if (r_value < (realtype)0.15)
	      *ip_value = 1;
	   else if (r_value < (realtype)0.25)
	      *ip_value = 2;
	   else if (r_value < (realtype)0.35)
	      *ip_value = 3;
	   else if (r_value < (realtype)0.45)
	      *ip_value = 4;
	   else if (r_value < (realtype)0.55)
	      *ip_value = 5;
	   else if (r_value < (realtype)0.65)
	      *ip_value = 6;
	   else if (r_value < (realtype)0.75)
	      *ip_value = 7;
	   else if (r_value < (realtype)0.85)
	      *ip_value = 8;
	   else if (r_value < (realtype)0.95)
	      *ip_value = 9;
	   else 
		   *ip_value = 10;
	}
	else
	{
	   if (r_value < (realtype)0.1)
	      *ip_value = 0;
	   else if (r_value < (realtype)0.2)
	      *ip_value = 1;
	   else if (r_value < (realtype)0.3)
	      *ip_value = 2;
	   else if (r_value < (realtype)0.4)
	      *ip_value = 3;
	   else if (r_value < (realtype)0.5)
	      *ip_value = 4;
	   else if (r_value < (realtype)0.6)
	      *ip_value = 5;
	   else if (r_value < (realtype)0.7)
	      *ip_value = 6;
	   else if (r_value < (realtype)0.8)
	      *ip_value = 7;
	   else if (r_value < (realtype)0.9)
	      *ip_value = 8;
	   else if (r_value < (realtype)1.0)
	      *ip_value = 9;
	   else 
		   *ip_value = 10;
	}

   return(OKAY);
}

/*
 * FUNCTION: knobpConvertIntToFaderRealValue()
 * DESCRIPTION:
 *
 * Takes the passed fader int value (0 - 10) and passes back the corresponding real value
 * of the fader (0.0 - 1.0).
 *
 */
int knobpConvertIntToFaderRealValue(PT_HANDLE *hp_knobp, int i_value, realtype *rp_value)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

   if (i_value == 0)
	   *rp_value = (realtype)0.0;
	else if (i_value == 1)
		*rp_value = (realtype)0.1;
	else if (i_value == 2)
		*rp_value = (realtype)0.2;
	else if (i_value == 3)
		*rp_value = (realtype)0.3;
	else if (i_value == 4)
		*rp_value = (realtype)0.4;
	else if (i_value == 5)
		*rp_value = (realtype)0.5;
	else if (i_value == 6)
		*rp_value = (realtype)0.6;
	else if (i_value == 7)
		*rp_value = (realtype)0.7;
	else if (i_value == 8)
		*rp_value = (realtype)0.8;
	else if (i_value == 9)
		*rp_value = (realtype)0.9;
	else 
		*rp_value = (realtype)1.0;

   return(OKAY);
}


/*
 * FUNCTION: knobpSetProcessingOverride() 
 * DESCRIPTION:
 *
 *  Sets the special KNOB processing override setting.  When Remix is running we may want
 *  override the KNOB processing components based on certain conditions.  For example a copy Remix
 *  may want to completely override KNOB processing if it detects that a KNOB plugin is also concurrently
 *  running.
 *
 */
int knobpSetProcessingOverride(PT_HANDLE *hp_knobp, int i_knob_processing_override)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);
	
	if (cast_handle->trace.mode)
	   (cast_handle->slout1)->Message_Wide(FIRST_LINE, L"knobpSetProcessingOverride: Entered");

	/* If the override setting has not changed, there is nothing to do */
   if (i_knob_processing_override == cast_handle->processing_override)
		return(OKAY);

	cast_handle->processing_override = i_knob_processing_override;

   if (knobp_CommunicateBypassSettings(hp_knobp) != OKAY)
		return(NOT_OKAY);

	if (cast_handle->trace.mode)
	   (cast_handle->slout1)->Message_Wide(FIRST_LINE, L"knobpSetProcessingOverride: Done");

	return(OKAY);
}

/*
 * FUNCTION: knobpSetTemporaryBypassAll()
 * DESCRIPTION:
 *
 *  Sets whether or not to temporarily bypass all processing.
 */
int knobpSetTemporaryBypassAll(PT_HANDLE *hp_knobp, int i_temporary_bypass_all)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

   if (!(cast_handle->fully_initialized))
		return(NOT_OKAY);

	/* THIS MIGHT FAIL FOR INTERNET EXPLORER */
	if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_TEMPORARY_BYPASS_ALL_WIDE, i_temporary_bypass_all) != OKAY)
		return(OKAY);

	return(OKAY);
}

/*
 * FUNCTION: knobpSetKnobTunedTrackPlaying()
 * DESCRIPTION:
 *
 *  Sets whether or not to temporarily bypass all processing because the current track
 *  has already been KNOB Tuned.
 */
int knobpSetKnobTunedTrackPlaying(PT_HANDLE *hp_knobp, int i_knob_tuned_track_playing)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	if (cast_handle->trace.mode)
	   (cast_handle->slout1)->Message_Wide(FIRST_LINE, L"knobpSetKnobTunedTrackPlaying: Entered");

   if (!(cast_handle->fully_initialized))
		return(NOT_OKAY);

	if (cast_handle->trace.mode)
	{
		swprintf(cast_handle->wcp_msg1, L"knobpSetKnobTunedTrackPlaying: i_knob_tuned_track_playing = %d", i_knob_tuned_track_playing);
	   (cast_handle->slout1)->Message_Wide(FIRST_LINE, cast_handle->wcp_msg1);
   }

	if (knobp_SessionWriteIntegerValue(hp_knobp, KNOBP_REGISTRY_KNOB_TUNED_TRACK_PLAYING_WIDE, i_knob_tuned_track_playing) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

/*
 * FUNCTION: knobpSetTotalAudioProcessedTime()
 * DESCRIPTION:
 *   Set the total audio processed time to the given value
 */
int knobpSetTotalAudioProcessedTime(PT_HANDLE *hp_knobp, unsigned long ul_msec_audio_processed_time)
{
	struct knobpHdlType *cast_handle;

	cast_handle = (struct knobpHdlType *)(hp_knobp);

	if (cast_handle == NULL)
		return(OKAY);

	if (cast_handle->trace.mode)
		(cast_handle->slout1)->Message_Wide(FIRST_LINE, L"knobpSetTotalAudioProcessedTime: Entered");

	if (!(cast_handle->fully_initialized))
		return(NOT_OKAY);

	cast_handle->ul_total_msecs_audio_processed_time = ul_msec_audio_processed_time;
#ifdef PT_PORTABLE_DSP
	return knobSharedUtilSetTotalProcessedTime(cast_handle->hp_sharedUtil, ul_msec_audio_processed_time);
#endif

	return(OKAY);
}
