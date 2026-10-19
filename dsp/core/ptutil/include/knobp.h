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
#ifndef _KNOBP_H_
#define _KNOBP_H_ 

#include "knobpDefs.h"
#include "slout.h"

/* knobpComm.cpp */
int knobpCommunicateAll(PT_HANDLE *);
int knobpCommunicateAllNonFixed(PT_HANDLE *, int);

/* knobpEq */
int knobpEqSetProcessingOn(PT_HANDLE *, int, int);
int knobpEqGetProcessingOn(PT_HANDLE *, int, int *);
int knobpEqSetEqType(PT_HANDLE *, int);
int knobpEqGetBandCenterFrequency(PT_HANDLE *, int, realtype *);
int knobpEqSetBandBoostCut(PT_HANDLE *, int, int, realtype);
int knobpEqGetBandBoostCut_FromProcessing(PT_HANDLE *, int, realtype *, wchar_t *);
int knobpEqGetBandBoostCut_FromRegistry(PT_HANDLE *, int, realtype *, wchar_t *);
int knobpEqGetGraphicEqHdl(PT_HANDLE *, PT_HANDLE **);

/* knobpGet */
int knobpGetKnobValue(PT_HANDLE *, int, float *);
int knobpGetButtonValue(PT_HANDLE *, int, int *);
int knobpGetSamplingFreq(PT_HANDLE *, realtype *);
int knobpGetProcessingOverride(PT_HANDLE *, int *);
int knobpGetFirstTimeRunFlag(PT_HANDLE *, int *);
int knobpGetTemporaryBypassAll(PT_HANDLE *, int *);
int knobpGetKnobTunedTrackPlaying(PT_HANDLE *, int *);
int knobpSetKnobTunedTrackPlaying(PT_HANDLE *, int);
int knobpGetTotalAudioProcessedTime(PT_HANDLE *, unsigned long *);

/* knobpInit */
int knobpInit(PT_HANDLE **, wchar_t *, int, int, int, int, int, long, int, int, int, CSlout *);

/* knobpProcess */
int knobpSetValidBits(PT_HANDLE *, int);
int knobpModifyRealtypeSamples(PT_HANDLE *, realtype *, int, int);
int knobpClearPreviousBufferedAudio(PT_HANDLE *);

/* knobpQuit */
int knobpQuit(PT_HANDLE **);

/* knobpRegistryStandard */
int knobpGetInstallationDate(PT_HANDLE *, long *, int *);
int knobpGetLastUsedDate(PT_HANDLE *, long *, int *);
int knobpGetInstallationDate_NoHandle(wchar_t *, int, long *, int *);

/* knobpSet */
int knobpSetKnobValue(PT_HANDLE *, int, float, bool);
int knobpSetButtonValue(PT_HANDLE *, int, int);
int knobpConvertFaderRealValueToInt(PT_HANDLE *, realtype, int *, int);
int knobpSetProcessingOverride(PT_HANDLE *, int);
int knobpSetTemporaryBypassAll(PT_HANDLE *, int);
int knobpSetTotalAudioProcessedTime(PT_HANDLE *, unsigned long);

/* knobpSharedMemory */
int knobpSharedMemorySetFlag(PT_HANDLE *, int, int);
int knobpSharedMemoryGetFlag(PT_HANDLE *, int, int *);
int knobpSharedMemoryGetCurrentSpectrumValues(PT_HANDLE *, realtype *, int);
int knobpSharedMemoryStoreCurrentSpectrumValues_FromArray(PT_HANDLE *, realtype *, int);

/* knobpSpectrum */
int knobpSpectrumSendClearValues(PT_HANDLE *);
int knobpSpectrumGetBandValues(PT_HANDLE *, realtype *, int);


/* knobpUniversal */
int knobpUniversalInit(PT_HANDLE **, long, int, CSlout *);
int knobpUniversalSetSignalFormat(PT_HANDLE *, int, int, int, int);
int knobpUniversalModifySamples(PT_HANDLE *, short int *, short int *, int, int);
int knobpUniversalCheckParentCompatibility(PT_HANDLE *, int, int *);

#endif //_KNOBP_H_