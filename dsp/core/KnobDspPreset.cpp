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
#include "vals.h"
#include "qnt.h"
#include "ptutil/knobp/u_knobp.h"

/* Vals indexes */
#define KNOBG_VALS_FIDELITY_INDEX                 0
#define KNOBG_VALS_SURROUND_INDEX                 1
#define KNOBG_VALS_AMBIENCE_INDEX                 3
#define KNOBG_VALS_DYNAMIC_BOOST_INDEX            4
#define KNOBG_VALS_BASS_BOOST_INDEX               5

/* App Depend vals info */
#define KNOBG_VALS_NUM_APP_DEPEND_INTS            7
#define KNOBG_VALS_APP_DEPEND_FIDELITY_INDEX      0
#define KNOBG_VALS_APP_DEPEND_SURROUND_INDEX      1
#define KNOBG_VALS_APP_DEPEND_AMBIENCE_INDEX      2
#define KNOBG_VALS_APP_DEPEND_DYNAMIC_BOOST_INDEX 3
#define KNOBG_VALS_APP_DEPEND_BASS_BOOST_INDEX    4
#define KNOBG_VALS_APP_DEPEND_HEADPHONE_INDEX     5
#define KNOBG_VALS_APP_DEPEND_MUSIC_MODE_INDEX    6

/*
* The different modes for music processing
*/
#define KNOB_UI_MUSIC_MODE_MUSIC1      1
#define KNOB_UI_MUSIC_MODE_MUSIC2      2
#define KNOB_UI_MUSIC_MODE_SPEECH      3

/*
 * Version of the vals files to be written.
 *
 *  PRE KNOB 12: VALS VERSION = 7.0
 *  KNOB 12: VALS VERSION = 9.0
 */
#define KNOBG_VALS_FILE_VERSION        9.0

/*
* Modeled after knobg_LoadPreset() in knobgPreset.cpp.
*/
int KnobDspPrivate::loadPreset(std::wstring preset_file_full_path)
{
	//	int exists;
	PT_HANDLE *new_vals_hdl;
	//	int legal_free_preset;
	//	int is_factory_preset;

	/* Read the file */
	if (valsRead(&preset_file_full_path[0], IS_FALSE, NULL, &new_vals_hdl) != OKAY)
		return(NOT_OKAY);

	/* Store the vals settings into the global state info */
	if (getStateInfoFromVals(new_vals_hdl, true) != OKAY)
		return(NOT_OKAY);

	/* Store the preset num in the registry */
	// SHOULD LET THE APP REMEMBER CURRENT PRESET SELECTION
	//if (writeRegistrySessionLongValue((long)i_prelst_index, KNOBG_REGISTRY_CURRENT_PRESET_NUM_WIDE) != OKAY)
	//return(NOT_OKAY);

	/* Store the new clean preset settigns */
	//if (knobg_PresetStoreCleanSettings(hp_knobg) != OKAY)
	//return(NOT_OKAY);

	/* Check if the preset is dirty (It should not be dirty at this point) */
	//if (knobg_PresetCalcAndSetDirtyFlag(hp_knobg) != OKAY)
	//return(NOT_OKAY);

	/*setEffectValue(KnobDsp::Effect::Fidelity, fidelity_.value*10.0f);
	setEffectValue(KnobDsp::Effect::Ambience, ambience_.value*10.0f);
	setEffectValue(KnobDsp::Effect::Surround, surround_.value*10.0f);
	setEffectValue(KnobDsp::Effect::DynamicBoost, dynamic_boost_.value*10.0f);
	setEffectValue(KnobDsp::Effect::Bass, bass_boost_.value*10.0f);*/

	/* Free the vals handle */
	if (valsFreeUp(&new_vals_hdl) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

int KnobDspPrivate::savePreset(std::wstring preset_name, std::wstring preset_file_full_path)
{
	PT_HANDLE *vals_hdl;

	if (createValsFromStateInfo(&preset_name[0], &vals_hdl) != OKAY)
		return(NOT_OKAY);

	preset_name.append(L".fac");
	if (valsSave(vals_hdl, &preset_file_full_path[0], &preset_name[0]) != OKAY)
		return(NOT_OKAY);

	/* Free the vals handle */
	if (valsFreeUp(&vals_hdl) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

int KnobDspPrivate::exportPreset(std::wstring preset_source_file_full_path, std::wstring preset_name, std::wstring preset_export_path)
{
	PT_HANDLE* vals_hdl;

	if (valsRead(&preset_source_file_full_path[0], IS_FALSE, NULL, &vals_hdl) != OKAY)
		return(NOT_OKAY);

	if (getStateInfoFromVals(vals_hdl, true) != OKAY)
		return(NOT_OKAY);

	preset_name.append(L".fac");
	if (valsSave(vals_hdl, &preset_export_path[0], &preset_name[0]) != OKAY)
		return(NOT_OKAY);

	if (valsFreeUp(&vals_hdl) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

/*
* Modeled after knobg_GetStateInfoFromVals() in knobgPreset.cpp.
*/
int KnobDspPrivate::getStateInfoFromVals(PT_HANDLE *hp_vals, bool b_include_eq)
{
	int i_value;
	int button_on;
	realtype vals_file_version;
	//	int update_from_registry_active_original_setting;

	if (hp_vals == NULL)
		return(NOT_OKAY);

	/* Temporarily make sure the update from registry timer is off. */
	update_from_registry_ = false;

	/* Get the vals file version */
	if (valsGetFileVersion(hp_vals, &vals_file_version) != OKAY)
		return(NOT_OKAY);

	/* Set the fidelity information */
	if (valsGetAppDependentInt(hp_vals, KNOBG_VALS_APP_DEPEND_FIDELITY_INDEX, &(button_on)) != OKAY)
		return(NOT_OKAY);

	fidelity_.bypass = !(button_on);

	if (valsGetMainParamValue(hp_vals, KNOBG_VALS_FIDELITY_INDEX, &i_value) != OKAY)
		return(NOT_OKAY);

	if (qntIToRCalc(midi_to_rval_qnt_handle_, i_value, &(fidelity_.value)) != OKAY)
		return(NOT_OKAY);

	/* Set the ambience information */
	if (valsGetAppDependentInt(hp_vals, KNOBG_VALS_APP_DEPEND_AMBIENCE_INDEX, &(button_on)) != OKAY)
		return(NOT_OKAY);

	ambience_.bypass = !(button_on);

	if (valsGetMainParamValue(hp_vals, KNOBG_VALS_AMBIENCE_INDEX, &i_value) != OKAY)
		return(NOT_OKAY);

	if (qntIToRCalc(midi_to_rval_qnt_handle_, i_value, &(ambience_.value)) != OKAY)
		return(NOT_OKAY);

	/* Set the surround information */
	if (valsGetAppDependentInt(hp_vals, KNOBG_VALS_APP_DEPEND_SURROUND_INDEX, &(button_on)) != OKAY)
		return(NOT_OKAY);

	surround_.bypass = !(button_on);

	if (valsGetMainParamValue(hp_vals, KNOBG_VALS_SURROUND_INDEX, &i_value) != OKAY)
		return(NOT_OKAY);

	if (qntIToRCalc(midi_to_rval_qnt_handle_, i_value, &(surround_.value)) != OKAY)
		return(NOT_OKAY);

	/* Set the dynamic_boost information */
	if (valsGetAppDependentInt(hp_vals, KNOBG_VALS_APP_DEPEND_DYNAMIC_BOOST_INDEX, &(button_on)) != OKAY)
		return(NOT_OKAY);

	dynamic_boost_.bypass = !(button_on);

	if (valsGetMainParamValue(hp_vals, KNOBG_VALS_DYNAMIC_BOOST_INDEX, &i_value) != OKAY)
		return(NOT_OKAY);

	if (qntIToRCalc(midi_to_rval_qnt_handle_, i_value, &(dynamic_boost_.value)) != OKAY)
		return(NOT_OKAY);

	/* Set the bass_boost information */
	if (vals_file_version < 3.0)
	{
		button_on = IS_TRUE;
		i_value = 0;
	}
	else
	{
		if (valsGetAppDependentInt(hp_vals, KNOBG_VALS_APP_DEPEND_BASS_BOOST_INDEX, &(button_on)) != OKAY)
			return(NOT_OKAY);

		bass_boost_.bypass = !(button_on);

		if (valsGetMainParamValue(hp_vals, KNOBG_VALS_BASS_BOOST_INDEX, &i_value) != OKAY)
			return(NOT_OKAY);

		if (qntIToRCalc(midi_to_rval_qnt_handle_, i_value, &(bass_boost_.value)) != OKAY)
			return(NOT_OKAY);
	}

	/* Set the headphone information */
	if (vals_file_version < 4.0)
	{
		headphone_on_ = IS_FALSE;
	}
	else
	{
		if (valsGetAppDependentInt(hp_vals, KNOBG_VALS_APP_DEPEND_HEADPHONE_INDEX, &(headphone_on_)) != OKAY)
			return(NOT_OKAY);
	}

	/*
	* Set the music information
	*
	* NOTE: As of KNOB Version 13, we only allow music mode 2.
	*
	*/
	music_mode_ = KNOB_UI_MUSIC_MODE_MUSIC2;

	/* Communicate the changes to the DSP processing module */
	/*
	if (knobg_CommunicateAll(hp_knobg) != OKAY)
	{
	cast_handle->timers.update_from_registry.active = update_from_registry_active_original_setting;
	return(NOT_OKAY);
	}
	*/
	if (b_include_eq)
	{
		/* Get the EQ settings from the preset */
		if (getGraphicEqInfoFromVals(hp_vals) != OKAY)
		{
			update_from_registry_ = true;
			return(NOT_OKAY);
		}
	}

	/* Turn the update from registry back on if it was originally on */
	update_from_registry_ = true;

	return(OKAY);
}

int KnobDspPrivate::createValsFromStateInfo(wchar_t *preset_name, PT_HANDLE **hpp_vals)
{
	int midi_value;
	PT_HANDLE *hp_graphicEq;
	int i_eq_on;

	if (hpp_vals == NULL)
		return(NOT_OKAY);

	/* Create the vals handle */
	if (valsInit(hpp_vals, slout1_, 1, KNOBG_VALS_FILE_VERSION, IS_FALSE) != OKAY)
		return(NOT_OKAY);

	if (valsInitAppDependentInfo(*hpp_vals, KNOBG_VALS_NUM_APP_DEPEND_INTS, 0, 0) != OKAY)
		return(NOT_OKAY);

	if (valsSetComment(*hpp_vals, preset_name) != OKAY)
		return(NOT_OKAY);

	/* Get the eq handle */
	if (knobpEqGetGraphicEqHdl(knobp_handle_, &hp_graphicEq) != OKAY)
		return(NOT_OKAY);
	if (knobpEqGetProcessingOn(knobp_handle_, KNOBP_STORAGE_TYPE_REGISTRY, &i_eq_on) != OKAY)
		return(NOT_OKAY);
	if (valsSetGraphicEq(*hpp_vals, hp_graphicEq, i_eq_on) != OKAY)
		return(NOT_OKAY);

	/* Set the fidelity information */
	if (valsSetAppDependentInt(*hpp_vals, KNOBG_VALS_APP_DEPEND_FIDELITY_INDEX,
		!(fidelity_.bypass)) != OKAY)
		return(NOT_OKAY);

	if (qntRToICalc(rval_to_midi_qnt_handle_, fidelity_.value, &midi_value) != OKAY)
		return(NOT_OKAY);

	if (valsSetMainParamValue(*hpp_vals, KNOBG_VALS_FIDELITY_INDEX, midi_value) != OKAY)
		return(NOT_OKAY);

	/* Set the ambience information */
	if (valsSetAppDependentInt(*hpp_vals, KNOBG_VALS_APP_DEPEND_AMBIENCE_INDEX,
		!(ambience_.bypass)) != OKAY)
		return(NOT_OKAY);

	if (qntRToICalc(rval_to_midi_qnt_handle_, ambience_.value, &midi_value) != OKAY)
		return(NOT_OKAY);

	if (valsSetMainParamValue(*hpp_vals, KNOBG_VALS_AMBIENCE_INDEX, midi_value) != OKAY)
		return(NOT_OKAY);

	/* Set the surround information */
	if (valsSetAppDependentInt(*hpp_vals, KNOBG_VALS_APP_DEPEND_SURROUND_INDEX,
		!(surround_.bypass)) != OKAY)
		return(NOT_OKAY);

	if (qntRToICalc(rval_to_midi_qnt_handle_, surround_.value, &midi_value) != OKAY)
		return(NOT_OKAY);

	if (valsSetMainParamValue(*hpp_vals, KNOBG_VALS_SURROUND_INDEX, midi_value) != OKAY)
		return(NOT_OKAY);

	/* Set the dynamic_boost information */
	if (valsSetAppDependentInt(*hpp_vals, KNOBG_VALS_APP_DEPEND_DYNAMIC_BOOST_INDEX,
		!(dynamic_boost_.bypass)) != OKAY)
		return(NOT_OKAY);

	if (qntRToICalc(rval_to_midi_qnt_handle_, dynamic_boost_.value, &midi_value) != OKAY)
		return(NOT_OKAY);

	if (valsSetMainParamValue(*hpp_vals, KNOBG_VALS_DYNAMIC_BOOST_INDEX, midi_value) != OKAY)
		return(NOT_OKAY);

	/* Set the bass_boost information */
	if (valsSetAppDependentInt(*hpp_vals, KNOBG_VALS_APP_DEPEND_BASS_BOOST_INDEX,
		!(bass_boost_.bypass)) != OKAY)
		return(NOT_OKAY);

	if (qntRToICalc(rval_to_midi_qnt_handle_, bass_boost_.value, &midi_value) != OKAY)
		return(NOT_OKAY);

	if (valsSetMainParamValue(*hpp_vals, KNOBG_VALS_BASS_BOOST_INDEX, midi_value) != OKAY)
		return(NOT_OKAY);

	/* Set the headphone information */
	if (valsSetAppDependentInt(*hpp_vals, KNOBG_VALS_APP_DEPEND_HEADPHONE_INDEX,
		headphone_on_) != OKAY)
		return(NOT_OKAY);

	/* Set the music mode */
	if (valsSetAppDependentInt(*hpp_vals, KNOBG_VALS_APP_DEPEND_MUSIC_MODE_INDEX,
		music_mode_) != OKAY)
		return(NOT_OKAY);

	return(OKAY);
}

KnobPreset KnobDspPrivate::getPresetInfo(std::wstring preset_file_full_path)
{
	KnobPreset knob_preset;
	PT_HANDLE *vals_hdl;
	wchar_t *wcp_comment;

	knob_preset.full_path = preset_file_full_path;

	// Read the file
	if (valsRead(&preset_file_full_path[0], IS_FALSE, NULL, &vals_hdl) != OKAY)
	{
	}

	if (vals_hdl == NULL)
	{
	}

	/* Get the comment */
	if (valsGetComment(vals_hdl, &wcp_comment) != OKAY)
	{
	}
	if (wcp_comment != NULL)
	{
		knob_preset.name = wcp_comment;
		//swprintf(wcp_line_str, L"%s", wcp_comment);
	}

	// Free up the vals handle 
	if (valsFreeUp(&vals_hdl) != OKAY)
	{
	}

	return knob_preset;

}

/*
int knobg_DlgPresetInit(PT_HANDLE *hp_knobg, HWND hDlg)
{
int num_presets;
int index;
int preset_exists;
wchar_t wcp_fullpath[PT_MAX_PATH_STRLEN];
wchar_t *wcp_comment;
PT_HANDLE *vals_hdl;
wchar_t wcp_line_str[PT_MAX_GENERIC_STRLEN];
int i_current_preset_listbox_num;
int i_current_prelst_index;
wchar_t wcp_preset_name[PT_MAX_GENERIC_STRLEN];

// Store that the dialog is up
cast_handle->dialog_info.up_general = (cast_handle->dialog_info.up_general) | KNOBG_DIALOG_GENERAL_PRESET;
cast_handle->dialog_info.hwndDlgPreset = hDlg;

// Store the current settings in the special system cache so they can be restored on cancel
if (knobg_PresetGetCurrentPresetName(hp_knobg, wcp_preset_name) != OKAY)
return(NOT_OKAY);
if (knobg_CreateValsFromStateInfo(hp_knobg, wcp_preset_name, &(cast_handle->dialog_info.preset_dlg_info.hp_vals_pre_select)) != OKAY)
return(NOT_OKAY);

num_presets = 0;

cast_handle->dialog_info.preset_dlg_info.i_initial_prelst_index = -1;

// Build the list of preset
for (index = 0; index < PRELST_MAX_USER_PRESET_INDEX; index++)
{
// Check if this preset exists
if (prelstGetExists(cast_handle->preset_info.prelst_hdl, index, &preset_exists) != OKAY)
return(NOT_OKAY);

if (preset_exists)
{
// Construct the fullpath to the file
if (prelstConstructFullpath(cast_handle->preset_info.prelst_hdl, index, wcp_fullpath) != OKAY)
return(NOT_OKAY);

// Read the file
if (valsRead(wcp_fullpath, cast_handle->trace.mode, cast_handle->slout1, &vals_hdl) != OKAY)
return(NOT_OKAY);

if (vals_hdl == NULL)
return(NOT_OKAY);

// Get the comment
if (valsGetComment(vals_hdl, &wcp_comment) != OKAY)
return(NOT_OKAY);
if (wcp_comment == NULL)
return(NOT_OKAY);

// Construct the line
//	      swprintf(wcp_line_str, L"(%d) %s", index + 1, wcp_comment);
swprintf(wcp_line_str, L"%s", wcp_comment);

SendDlgItemMessage(hDlg, IDC_PRESET_LIST, LB_ADDSTRING,
0, (LPARAM)(LPSTR)wcp_line_str);
num_presets++;

// Free up the vals handle
if (valsFreeUp(&vals_hdl) != OKAY)
return(NOT_OKAY);
}
}

// Read what the current preset number is and use that as the default
if (knobg_PresetGetCurrentPrelstIndex(hp_knobg, &i_current_prelst_index) != OKAY)
return(NOT_OKAY);

// Make sure the specified current preset exists
if (prelstGetExists(cast_handle->preset_info.prelst_hdl, i_current_prelst_index, &preset_exists) != OKAY)
return(NOT_OKAY);

if (preset_exists)
{
if (prelstCalcRealToListNum(cast_handle->preset_info.prelst_hdl, i_current_prelst_index,
&i_current_preset_listbox_num) != OKAY)
return(NOT_OKAY);

cast_handle->dialog_info.preset_dlg_info.i_initial_prelst_index = i_current_prelst_index;
}
else
{
i_current_preset_listbox_num = -1; // No selection
}

// Set the default selection
SendDlgItemMessage(hDlg, IDC_PRESET_LIST, LB_SETCURSEL, i_current_preset_listbox_num, 0);

// Based on the current selection enable or disable the delete button
if (knobg_DlgPresetSetDeleteEnabling(hp_knobg, hDlg) != OKAY)
return(NOT_OKAY);

return(OKAY);
}
*/

/*
int KnobDsp::initPresets(std::wstring facotryPresetDirectoryFullPath, std::wstring userPresetDirectoryFullPath, std::wstring systemPresetDirectoryFullPath)
{
struct knobgHdlType *cast_handle;

cast_handle = (struct knobgHdlType *)(hp_knobg);

if (cast_handle == NULL)
return(OKAY);

int i_current_prelst_index;
int preset_exists;
int i_success;


// Create the path to the special system preset dir and create the folder if it does not exist
if (fileCreateDirectoryAndParents_Wide(&systemPresetDirectoryFullPath[0], &i_success, NULL) != OKAY)
return(NOT_OKAY);

// Initialize the preset list
if (prelstCreate(&preset_handle_,
NULL,
&facotryPresetDirectoryFullPath[0],
&userPresetDirectoryFullPath[0],
KNOBG_MIN_USER_PRESET_INDEX,
PRELST_MAX_USER_PRESET_INDEX) != OKAY)
return(NOT_OKAY);

// Read what the current preset number
if (knobg_PresetGetCurrentPrelstIndex(hp_knobg, &i_current_prelst_index) != OKAY)
return(NOT_OKAY);

// Make sure the specified current preset exists
if (prelstGetExists(cast_handle->preset_info.prelst_hdl, i_current_prelst_index, &preset_exists) != OKAY)
return(OKAY);

// If it does not exist , set the current preset to be the default one
if (!preset_exists)
{
if (knobg_WriteRegistrySessionLongValue(hp_knobg, (long)KNOBG_DEFAULT_PRESET_INDEX, KNOBG_REGISTRY_CURRENT_PRESET_NUM_WIDE) != OKAY)
return(NOT_OKAY);
}

// Store the virgin settings of the current preset to be the "clean" cache
if (knobg_PresetStoreCleanSettings(hp_knobg) != OKAY)
return(NOT_OKAY);

return(OKAY);
}
*/