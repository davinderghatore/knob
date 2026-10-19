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
#include "KnobDsp.h"
#include "u_KnobDsp.h"

KnobDsp::KnobDsp()
{
	data_ = new KnobDspPrivate();
}


KnobDsp::~KnobDsp()
{
	delete data_;
}

void KnobDsp::powerOn(bool on)
{
	data_->powerOn(on);
}

bool KnobDsp::isPowerOn()
{
	return data_->isPowerOn();
}

float KnobDsp::getEffectValue(Effect effect)
{
	return data_->getEffectValue(effect);
}

void KnobDsp::setEffectValue(Effect effect, float value)
{
	data_->setEffectValue(effect, value);
}

int KnobDsp::loadPreset(std::wstring preset_file_full_path)
{
	return data_->loadPreset(preset_file_full_path);
}

int KnobDsp::savePreset(std::wstring preset_name, std::wstring preset_file_full_path)
{
	return data_->savePreset(preset_name, preset_file_full_path);
}

int KnobDsp::exportPreset(std::wstring preset_source_file_full_path, std::wstring preset_name, std::wstring preset_export_path)
{
	return data_->exportPreset(preset_source_file_full_path, preset_name, preset_export_path);
}

void KnobDsp::eqOn(bool on)
{
	data_->eqOn(on);
}

int KnobDsp::getNumEqBands()
{
	return data_->getNumEqBands();
}

KnobPreset KnobDsp::getPresetInfo(std::wstring preset_file_full_path)
{
	return data_->getPresetInfo(preset_file_full_path);
}

unsigned long KnobDsp::getTotalAudioProcessedTime()
{
	return data_->getTotalAudioProcessedTime();
}

void KnobDsp::resetTotalAudioProcessedTime()
{
	data_->resetTotalAudioProcessedTime();
}

int KnobDsp::setSignalFormat(int i_bps, int i_nch, int i_srate, int i_valid_bits)
{
	if (data_->being_destroyed_)
	{
		return OKAY;
	} else
	{
		return data_->setSignalFormat(i_bps, i_nch, i_srate, i_valid_bits);
	}
}

int KnobDsp::processAudio(short int *si_input_samples, short int *si_output_samples, int i_num_sample_sets, int i_check_for_duplicate_buffers)
{
	if (data_->being_destroyed_)
	{
		return OKAY;
	}
	else
	{
		return data_->processAudio(si_input_samples, si_output_samples, i_num_sample_sets, i_check_for_duplicate_buffers);
	}
}

float KnobDsp::getBalance()
{
	return data_->getBalance();
}

void KnobDsp::setBalance(float gain_db)
{
	data_->setBalance(gain_db);
}

float KnobDsp::getNormalization()
{
	return data_->getNormalization();
}

void KnobDsp::setNormalization(float gain_db)
{
	data_->setNormalization(gain_db);
}

float KnobDsp::getVolumeLeveling()
{
	return data_->getVolumeLeveling();
}

void KnobDsp::setVolumeLeveling(float gain_db)
{
	data_->setVolumeLeveling(gain_db);
}

float KnobDsp::getMasterGain()
{
	return data_->getMasterGain();
}

void KnobDsp::setMasterGain(float gain_db)
{
	data_->setMasterGain(gain_db);
}

float KnobDsp::getFilterQ()
{
	return data_->getFilterQ();
}

void KnobDsp::setFilterQ(float q_multiplier)
{
	data_->setFilterQ(q_multiplier);
}

void KnobDsp::setNumBands(int num_bands)
{
	data_->setNumBands(num_bands);
}

float KnobDsp::getEqBandFrequency(int band_num)
{
	return data_->getEqBandFrequency(band_num);
}

void KnobDsp::setEqBandFrequency(int band_num, float freq)
{
    data_->setEqBandFrequency(band_num, freq);
}

void KnobDsp::getEqBandFrequencyRange(int band_num, float* min_freq, float* max_freq)
{
    data_->getEqBandFrequencyRange(band_num, min_freq, max_freq);
}

float KnobDsp::getEqBandBoostCut(int band_num)
{
	return data_->getEqBandBoostCut(band_num);
}

void KnobDsp::setEqBandBoostCut(int band_num, float boost)
{
	data_->setEqBandBoostCut(band_num, boost);
}

void KnobDsp::getSpectrumBandValues(float* rp_band_values, int i_array_size)
{
    data_->getSpectrumBandValues(rp_band_values, i_array_size);
}

