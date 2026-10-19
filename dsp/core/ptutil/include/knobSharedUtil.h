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
#ifndef _KNOB_SHARED_UTIL_H_
#define _KNOB_SHARED_UTIL_H_
                                    
int PT_DECLSPEC knobSharedUtilInit(PT_HANDLE **, int, CSlout *);
int PT_DECLSPEC knobSharedUtilFreeUp(PT_HANDLE **);
int PT_DECLSPEC knobSharedUtilSetSpectrumValues(PT_HANDLE *, realtype *, int);
int PT_DECLSPEC knobSharedUtilGetSpectrumValues(PT_HANDLE *, realtype *, int);
int PT_DECLSPEC knobSharedUtilSetTotalProcessedTime(PT_HANDLE *, unsigned long);
int PT_DECLSPEC knobSharedUtilGetTotalProcessedTime(PT_HANDLE *, unsigned long *);

#endif // _KNOB_SHARED_UTIL_H_
