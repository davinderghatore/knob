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

/* knobpSpectrum.cpp */

#include "codedefs.h"

#ifndef PT_PORTABLE_DSP
#include <windows.h>
#endif
#include <stdio.h>
#ifndef PT_PORTABLE_DSP
#include <share.h>
#endif

#include "u_knobp.h" 

#include "knobp.h"
#include "spectrum.h"
#include "KnobSdk.h"
#include "reg.h"
#include "File.h"
#include "knobSharedUtil.h"

/*
 * FUNCTION: knobp_SpectrumInit() 
 * DESCRIPTION:
 *
 *  Initialize the spectrum handle and the associated data.
 *
 */
int knobp_SpectrumInit(PT_HANDLE *hp_knobp)
{
    struct knobpHdlType *cast_handle;

    cast_handle = (struct knobpHdlType *)(hp_knobp);

    if (cast_handle == NULL)
        return(OKAY);

    realtype r_host_buffer_delay_secs;
    realtype r_spectrum_refresh_rate_secs;

    /* Initialize the spectrum handle */
    r_host_buffer_delay_secs = (realtype)cast_handle->l_host_buffer_delay_msecs * (realtype)0.001;
    r_spectrum_refresh_rate_secs = (realtype)KNOBP_SPECTRUM_REFRESH_RATE_MSECS * (realtype)0.001;

     if (spectrumNew(&(cast_handle->spectrum.spectrum_hdl), 
                                KNOBP_SPECTRUM_NUM_BANDS, 
                                r_host_buffer_delay_secs, 
                                r_spectrum_refresh_rate_secs,
                                cast_handle->slout1,
                               cast_handle->trace.mode) != OKAY)
        return(NOT_OKAY);

    cast_handle->spectrum.sample_sets_since_last_spectrum_save = 0;

    if (knobpSpectrumSendClearValues((PT_HANDLE *)cast_handle) != OKAY)
        return(NOT_OKAY);

    return(OKAY);
}

/*
 * FUNCTION: knobpSpectrumSendClearValues() 
 * DESCRIPTION:
 *
 *  Sends all zero spectrum values to the GUI.
 *
 */
int knobpSpectrumSendClearValues(PT_HANDLE *hp_knobp)
{
    struct knobpHdlType *cast_handle;

    cast_handle = (struct knobpHdlType *)(hp_knobp);

    if (cast_handle == NULL)
        return(OKAY);

    /* Clear the spectrum file */
    if (knobp_SpectrumStoreCurrentValuesInSharedMemory(hp_knobp, IS_TRUE) != OKAY)
        return(NOT_OKAY);

    return(OKAY);
}

/*
 * FUNCTION: knobpSpectrumGetBandValues()
 * DESCRIPTION:
 *
 *  Get the array with the previously calculated band values
 *
 */
int knobpSpectrumGetBandValues(PT_HANDLE* hp_knobp, realtype* rp_band_values, int i_array_size)
{
    struct knobpHdlType *cast_handle;

    cast_handle = (struct knobpHdlType *)(hp_knobp);

    if (cast_handle == NULL)
        return(OKAY);

    if (cast_handle->spectrum.spectrum_hdl == NULL)
        return(OKAY);

    #ifdef PT_PORTABLE_DSP
    return knobSharedUtilGetSpectrumValues(cast_handle->hp_sharedUtil, rp_band_values, i_array_size);
    #endif

    if (spectrumGetBandValues(cast_handle->spectrum.spectrum_hdl, rp_band_values, i_array_size) != OKAY)
        return(NOT_OKAY);

    return(OKAY);
}

/*
 * FUNCTION: knobp_SpectrumStoreCurrentValuesInSharedMemory() 
 * DESCRIPTION:
 *
 *  Stores the current spectrum values in the shared memory so they can be sent to the UI.
 *
 */
int knobp_SpectrumStoreCurrentValuesInSharedMemory(PT_HANDLE *hp_knobp, int i_clear_values)
{
    struct knobpHdlType *cast_handle;

    cast_handle = (struct knobpHdlType *)(hp_knobp);

    if (cast_handle == NULL)
        return(OKAY);

   realtype rp_band_values[KNOBP_SPECTRUM_NUM_BANDS];
    int index;

    if (cast_handle->spectrum.spectrum_hdl == NULL)
        return(OKAY);
    
    if (cast_handle->hp_sharedUtil == NULL)
        return(OKAY);

    if (i_clear_values)
    {
        for (index = 0; index < KNOBP_SPECTRUM_NUM_BANDS; index++)
        {
            rp_band_values[index] = (realtype)0.0;
        }
    }
    else
    {
        if (spectrumGetBandValues(cast_handle->spectrum.spectrum_hdl, rp_band_values, KNOBP_SPECTRUM_NUM_BANDS) != OKAY)
            return(NOT_OKAY);
    }

    /* Store the values in shared memory */
    if (knobSharedUtilSetSpectrumValues(cast_handle->hp_sharedUtil, rp_band_values, KNOBP_SPECTRUM_NUM_BANDS) != OKAY)
        return(NOT_OKAY);

    return(OKAY);
}
