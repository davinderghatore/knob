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
#include "codedefs.h"
#include "pt_defs.h"
#include "u_KnobDsp.h"
#include "knobp.h"
#include "KnobSdk.h"
#include "vals.h"
#include "GraphicEq.h"
#include "reg.h"
#include "mth.h"
#include "ptutil/knobp/u_knobp.h"

int KNOBP_GRAPHIC_EQ_NUM_BANDS = 31;

/*
* FUNCTION: eqUpdateFromRegistry()
* DESCRIPTION:
*
* 2020-04-29: Taken from knobg_UpdateFromRegistryEQ() in knobgEQ.cpp
*
*  Checks if the value EQ stored in the registry has changed from what we
*  currently think it is.  If it has changed, then update the display and DSP memory values accordingly.
*
* This function only gets called in the sound card case, not in the XP dll based case that uses the winmm or dsound dll's.
* It implements the knobp calls required to update the EQ params in the DSP module.
* In the XP case these calls are implemented directly in knobpCommunicateAllNonFixed and the calls in that function must be
* kept up to date with any change in calls in this function.
*/
int KnobDspPrivate::eqUpdateFromRegistry(int *ip_eq_changed)
{
	int i_band_num;
	realtype r_boost_cut_from_processing;
	realtype r_boost_cut_from_registry;
	wchar_t wcp_boost_cut_from_processing[PT_MAX_GENERIC_STRLEN];
	wchar_t wcp_boost_cut_from_registry[PT_MAX_GENERIC_STRLEN];
	int i_eq_on_state_from_memory;
	int i_eq_on_state_from_registry;

	*ip_eq_changed = IS_FALSE;

	/* Check if the on/off state has changed */
	if (knobpEqGetProcessingOn(knobp_handle_, KNOBP_STORAGE_TYPE_MEMORY, &i_eq_on_state_from_memory) != OKAY)
		return(NOT_OKAY);
	if (knobpEqGetProcessingOn(knobp_handle_, KNOBP_STORAGE_TYPE_REGISTRY, &i_eq_on_state_from_registry) != OKAY)
		return(NOT_OKAY);
	if (i_eq_on_state_from_memory != i_eq_on_state_from_registry)
	{
		*ip_eq_changed = IS_TRUE;

		/* Store the new setting in memory */
		if (knobpEqSetProcessingOn(knobp_handle_, KNOBP_STORAGE_TYPE_MEMORY, i_eq_on_state_from_registry) != OKAY)
			return(NOT_OKAY);

		/* If eq has been turned off, then turn Hyperbass off */
		if (i_eq_on_state_from_registry == IS_FALSE)
		{
			if (knobpSetButtonValue(knobp_handle_, KNOB_UI_BUTTON_BASS_BOOST, IS_FALSE) != OKAY)
				return(NOT_OKAY);
		}
	}

	for (i_band_num = 1; i_band_num <= KNOBP_GRAPHIC_EQ_NUM_BANDS; i_band_num++)
	{
		/* Get the currently utilized eq setting */
		if (knobpEqGetBandBoostCut_FromProcessing(knobp_handle_, i_band_num, &r_boost_cut_from_processing, wcp_boost_cut_from_processing) != OKAY)
			return(NOT_OKAY);

		/* Get the eq setting from the registry */
		if (knobpEqGetBandBoostCut_FromRegistry(knobp_handle_, i_band_num, &r_boost_cut_from_registry, wcp_boost_cut_from_registry) != OKAY)
			return(NOT_OKAY);

		/*
		* Check if setting has changed.
		* NOTE: Since we are comparing real values we must compare their string equivalents as they would be stored in the registry.
		*/
		if (wcscmp(wcp_boost_cut_from_processing, wcp_boost_cut_from_registry))
		{
			*ip_eq_changed = IS_TRUE;

			/* Set the new value */
			if (knobpEqSetBandBoostCut(knobp_handle_, KNOBP_STORAGE_TYPE_MEMORY, i_band_num, r_boost_cut_from_registry) != OKAY)
				return(NOT_OKAY);

		}
	}

	return(OKAY);
}

int KnobDspPrivate::resetEQ()
{
	int i_band_num;

	update_from_registry_ = true;

	for (i_band_num = 1; i_band_num <= KNOBP_GRAPHIC_EQ_NUM_BANDS; i_band_num++)
	{
		if (knobpEqSetBandBoostCut(knobp_handle_, KNOBP_STORAGE_TYPE_ALL, i_band_num, (realtype)0.0) != OKAY)
			return(NOT_OKAY);
	}

	return(OKAY);
}

/*
* Modeled after knobg_GetGraphicEqInfoFromVals() in knobgPreset.cpp.
*/
int KnobDspPrivate::getGraphicEqInfoFromVals(PT_HANDLE *hp_vals)
{
	PT_HANDLE *hp_graphicEq;
	realtype r_boost_cut;
    realtype r_freq;
	int i_band_num;
	int i_eq_on;
	float f_bass_boost_value;

	int graphic_eq_num_bands = KNOBP_GRAPHIC_EQ_NUM_BANDS;

	if (valsGetGraphicEq(hp_vals, &hp_graphicEq, &i_eq_on) != OKAY)
		return(NOT_OKAY);

	/*
	* If it is an old preset which does not have any eq data eq to be on but then set all the bands to be flat 
	*/
	if (hp_graphicEq == NULL)
	{
		i_eq_on = IS_TRUE;

		if (knobpEqSetProcessingOn(knobp_handle_, KNOBP_STORAGE_TYPE_REGISTRY, i_eq_on) != OKAY)
			return(NOT_OKAY);

		for (i_band_num = 1; i_band_num <= graphic_eq_num_bands; i_band_num++)
		{
			r_boost_cut = (realtype)0.0;

			if (knobpEqSetBandBoostCut(knobp_handle_, KNOBP_STORAGE_TYPE_ALL, i_band_num, r_boost_cut) != OKAY)
				return(NOT_OKAY);
		}
	}
	else
	{
		if (knobpEqSetProcessingOn(knobp_handle_, KNOBP_STORAGE_TYPE_REGISTRY, i_eq_on) != OKAY)
			return(NOT_OKAY);

		PT_HANDLE* graphic_eq_handle;

		knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);

		// ====================================================================================
		//  CORRECTION for variable number of bands (Since version Theremino 2.0)
		//  - Interpolation/selection of gain only if nBands not equal to graphic_eq_num_bands.
		//  - Band frequencies from the preset are applied only when the band count matches;
		//    interpolating frequency across differing band counts previously produced
		//    incorrect/overlapping ranges, so a band-count mismatch keeps the live fixed
		//    (ISO or generic log-spaced) frequency table and only interpolates gain.
		// ====================================================================================
		int nBands;
		GraphicEqGetNumBands(hp_graphicEq, &nBands);
		// ---------------------------------------------------------- Dimension arrays more than max bands number (31)
		realtype r_boost_cut_original[35];
		realtype r_boost_cut_interpolated[35];
		// ---------------------------------------------------------- Read values into the original array
		for (int i_band_num = 1; i_band_num <= nBands; i_band_num++)
		{
			if (GraphicEqGetBandBoostCut(hp_graphicEq, i_band_num, &r_boost_cut_original[i_band_num]) != OKAY)
				return(NOT_OKAY);
		}

		// ---------------------------------------------------------- Interpolate values if the number of bands is different
		if (nBands != graphic_eq_num_bands)
		{
			if (nBands < graphic_eq_num_bands)
			{
				// -------------------------------------------------- Increase number of bands (linear interpolation)
				for (int i = 1; i <= graphic_eq_num_bands; i++)
				{
					realtype source_index = 1.0 + (realtype)(i - 1) * (nBands - 1.0) / (graphic_eq_num_bands - 1.0);
					int lower_index = (int)source_index;
					int upper_index = lower_index + 1;
					realtype fraction = source_index - lower_index;

					if (lower_index >= 1 && upper_index <= nBands)
					{
						r_boost_cut_interpolated[i] = r_boost_cut_original[lower_index] + (r_boost_cut_original[upper_index] - r_boost_cut_original[lower_index]) * fraction;
					}
					else if (lower_index == nBands)
					{
						r_boost_cut_interpolated[i] = r_boost_cut_original[lower_index];
					}
				}
			}
			else
			{
				// -------------------------------------------------- Decrease number of bands (equidistant selection)
				for (int i = 1; i <= graphic_eq_num_bands; i++)
				{
					int source_index = 1 + (int)((i - 1.0) * (nBands - 1.0) / (graphic_eq_num_bands - 1.0) + 0.5);
					r_boost_cut_interpolated[i] = r_boost_cut_original[source_index];
				}
			}

			// ------------------------------------------------------ Use interpolated values
			for (int i_band_num = 1; i_band_num <= graphic_eq_num_bands; i_band_num++)
			{
				if (knobpEqSetBandBoostCut(knobp_handle_, KNOBP_STORAGE_TYPE_ALL, i_band_num, r_boost_cut_interpolated[i_band_num]) != OKAY)
					return(NOT_OKAY);
			}
		}
		else
		{
			// ------------------------------------------------------ Use original values if the number of bands is the same
			for (int i_band_num = 1; i_band_num <= graphic_eq_num_bands; i_band_num++)
			{
				if (knobpEqSetBandBoostCut(knobp_handle_, KNOBP_STORAGE_TYPE_ALL, i_band_num, r_boost_cut_original[i_band_num]) != OKAY)
					return(NOT_OKAY);

				if (GraphicEqGetBandCenterFrequency(hp_graphicEq, i_band_num, &r_freq) != OKAY)
					return(NOT_OKAY);

				if (GraphicEqSetBandFreq(graphic_eq_handle, i_band_num, r_freq) != OKAY)
					return(NOT_OKAY);
			}
		}
	}

	return(OKAY);

}

/*
* Modeled after knobpEqSetProcessingOn() in knobpEq.cpp.
*/
int KnobDspPrivate::eqSetProcessingOn(int i_storage_type, int i_on)
{
	wchar_t wcp_full_key_path[PT_MAX_PATH_STRLEN];
	wchar_t wcp_key_value[KNOBP_REGISTRY_BUFFER_LENGTH];


	/* Store setting in memory (if requested) */
	if ((i_storage_type == KNOBP_STORAGE_TYPE_MEMORY) ||
		(i_storage_type == KNOBP_STORAGE_TYPE_ALL))
	{
		eq_processing_on_ = i_on;
	}

	/* Store setting in registry (if requested) */
	if ((i_storage_type == KNOBP_STORAGE_TYPE_REGISTRY) ||
		(i_storage_type == KNOBP_STORAGE_TYPE_ALL))
	{
		swprintf(wcp_full_key_path, L"%s\\%s\\%d\\%d\\%s\\%s\\%s",
			KNOBP_REGISTRY_TOP_WIDE,
			product_specific_.wcp_registry_product_name,
			product_specific_.major_version,
			vendor_specific_.vendor_code,
			KNOBP_REGISTRY_LASTUSED_WIDE,
			KNOBP_REGISTRY_EQ_FOLDER_NAME_WIDE,
			KNOBP_REGISTRY_EQ_ON_WIDE);

		swprintf(wcp_key_value, L"%d", i_on);

		if (regCreateKey_Wide(REG_CURRENT_USER, wcp_full_key_path, wcp_key_value) != OKAY)
			return(NOT_OKAY);
	}

	return(OKAY);
}

/*
* Modeled after knobpEqGetProcessingOn() in knobpEq.cpp.
*/
int KnobDspPrivate::eqGetProcessingOn(int i_storage_type, int *ip_on)
{
	wchar_t wcp_full_key_path[PT_MAX_PATH_STRLEN];
	wchar_t wcp_key_value[KNOBP_REGISTRY_BUFFER_LENGTH];
	int key_exists;
	int use_default;
	int is_long;

	*ip_on = IS_TRUE;

	if (i_storage_type == KNOBP_STORAGE_TYPE_MEMORY)
	{
		*ip_on = eq_processing_on_;
		return(OKAY);
	}

	swprintf(wcp_full_key_path, L"%s\\%s\\%d\\%d\\%s\\%s\\%s",
		KNOBP_REGISTRY_TOP_WIDE,
		product_specific_.wcp_registry_product_name,
		product_specific_.major_version,
		vendor_specific_.vendor_code,
		KNOBP_REGISTRY_LASTUSED_WIDE,
		KNOBP_REGISTRY_EQ_FOLDER_NAME_WIDE,
		KNOBP_REGISTRY_EQ_ON_WIDE);

	if (regReadKey_Wide(REG_CURRENT_USER, wcp_full_key_path, &key_exists, wcp_key_value,
		(unsigned long)KNOBP_REGISTRY_BUFFER_LENGTH) != OKAY)
		return(NOT_OKAY);

	use_default = IS_TRUE;

	if (key_exists)
	{
		if (mthIsLong_Wide(wcp_key_value, &is_long) != OKAY)
			return(NOT_OKAY);
		if (is_long)
			use_default = IS_FALSE;
	}

	if (!use_default)
		*ip_on = _wtoi(wcp_key_value);

	return(OKAY);
}

void KnobDspPrivate::eqOn(bool on)
{
	update_from_registry_ = true;

	if (on)
	{
		eqSetProcessingOn(KNOBP_STORAGE_TYPE_ALL, IS_TRUE);
	}
	else
	{
		eqSetProcessingOn(KNOBP_STORAGE_TYPE_ALL, IS_FALSE);
	}
}

int KnobDspPrivate::getNumEqBands()
{
	int num_bands = 0;
    PT_HANDLE* graphic_eq_handle;

    knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);

	GraphicEqGetNumBands(graphic_eq_handle, &num_bands);

	return num_bands;
}

float KnobDspPrivate::getBalance()
{
	float balance_db;

	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqGetBalance(graphic_eq_handle, &balance_db);

	return balance_db;
}

void KnobDspPrivate::setBalance(float gain_db)
{
	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqSetBalance(graphic_eq_handle, gain_db);
}

float KnobDspPrivate::getNormalization()
{
	float gain_db;

	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqGetNormalization(graphic_eq_handle, &gain_db);

	return gain_db;
}

void KnobDspPrivate::setNormalization(float gain_db)
{
	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqSetNormalization(graphic_eq_handle, gain_db);
}

float KnobDspPrivate::getVolumeLeveling()
{
	float gain_db;

	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqGetVolumeLeveling(graphic_eq_handle, &gain_db);

	return gain_db;
}

void KnobDspPrivate::setVolumeLeveling(float gain_db)
{
	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqSetVolumeLeveling(graphic_eq_handle, gain_db);
}

float KnobDspPrivate::getMasterGain()
{
	float gain_db;

	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqGetMasterGain(graphic_eq_handle, &gain_db);

	return gain_db;
}

void KnobDspPrivate::setMasterGain(float gain_db)
{
	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqSetMasterGain(graphic_eq_handle, gain_db);
}

float KnobDspPrivate::getFilterQ()
{
	float q_multiplier;

	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqGetFilterQ(graphic_eq_handle, &q_multiplier);

	return q_multiplier;
}

void KnobDspPrivate::setFilterQ(float q_multiplier)
{
	PT_HANDLE* graphic_eq_handle;
	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqSetFilterQ(graphic_eq_handle, q_multiplier);
}
void KnobDspPrivate::setNumBands(int num_bands)
{
	PT_HANDLE* graphic_eq_handle;

	update_from_registry_ = true;

	knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);
	GraphicEqSetNumBands(graphic_eq_handle, num_bands);
}
float KnobDspPrivate::getEqBandFrequency(int band_num)
{
	float band_freq = 0.0;

    knobpEqGetBandCenterFrequency(knobp_handle_, band_num + 1, &band_freq);

	return band_freq;
}

void KnobDspPrivate::setEqBandFrequency(int band_num, float freq)
{
    PT_HANDLE* graphic_eq_handle;

	update_from_registry_ = true;

    knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);

    GraphicEqSetBandFreq(graphic_eq_handle, band_num + 1, freq);
}

void KnobDspPrivate::getEqBandFrequencyRange(int band_num, float* min_freq, float* max_freq)
{
    PT_HANDLE* graphic_eq_handle;

    *min_freq = 0;
    *max_freq = 0;

    knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);

    GraphicEqGetBandFrequencyRange(graphic_eq_handle, band_num + 1, min_freq, max_freq);
}

float KnobDspPrivate::getEqBandBoostCut(int band_num)
{
    PT_HANDLE* graphic_eq_handle;

	float band_boost_cut = 0.0;

    knobpEqGetGraphicEqHdl(knobp_handle_, &graphic_eq_handle);

	GraphicEqGetBandBoostCut(graphic_eq_handle, band_num + 1, &band_boost_cut);

	return band_boost_cut;
}

void KnobDspPrivate::setEqBandBoostCut(int band_num, float boost)
{
	update_from_registry_ = true;

	knobpEqSetBandBoostCut(knobp_handle_, KNOBP_STORAGE_TYPE_ALL, band_num + 1, boost);
}
