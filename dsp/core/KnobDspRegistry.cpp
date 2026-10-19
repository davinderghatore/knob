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
#include "u_KnobDsp.h"
#include "reg.h"

#define PT_MAX_PATH_STRLEN             1024
#define KNOBG_REGISTRY_BUFFER_LENGTH             PT_MAX_PATH_STRLEN
#define KNOBG_REGISTRY_TOP_WIDE                        L"SOFTWARE"
#define KNOBG_REGISTRY_TOP_ANSI                        "SOFTWARE"
#define KNOBG_REGISTRY_LASTUSED_WIDE                    L"LASTUSED_KNOBG"

/*
* Modeled after knobg_WriteRegistrySessionLongValue() in knobgSession.cpp.
*/
int KnobDspPrivate::writeRegistrySessionLongValue(long l_value, wchar_t *wcp_key_name)
{
	wchar_t wcp_key_value[KNOBG_REGISTRY_BUFFER_LENGTH];
	wchar_t wcp_full_key_path[PT_MAX_PATH_STRLEN];

	if (vendor_specific_.vendor_code != 0)
	{
		swprintf(wcp_full_key_path, PT_MAX_PATH_STRLEN, L"%s\\%s\\%d\\%d\\%s\\%s",
			KNOBG_REGISTRY_TOP_WIDE,
			product_specific_.wcp_registry_product_name,
			product_specific_.major_version,
			vendor_specific_.vendor_code,
			KNOBG_REGISTRY_LASTUSED_WIDE,
			wcp_key_name);

		swprintf(wcp_key_value, KNOBG_REGISTRY_BUFFER_LENGTH, L"%ld", l_value);

		if (regCreateKey_Wide(REG_CURRENT_USER, wcp_full_key_path, wcp_key_value) != OKAY)
			return(NOT_OKAY);
	}

	return(OKAY);
}