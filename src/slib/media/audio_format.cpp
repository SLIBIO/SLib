/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/media/audio_format.h"

namespace slib
{

	AudioSampleType AudioFormats::getSampleType(AudioFormat format)
	{
		return (AudioSampleType)((sl_uint32)(format) & 255);
	}

	sl_uint32 AudioFormats::getBitsPerSample(AudioFormat format)
	{
		return ((sl_uint32)(format) >> 8) & 255;
	}

	sl_uint32 AudioFormats::getBytesPerSample(AudioFormat format)
	{
		return ((sl_uint32)(format) >> 11) & 31;
	}

	sl_uint32 AudioFormats::getChannelsCount(AudioFormat format)
	{
		return ((sl_uint32)(format) >> 16) & 255;
	}

	sl_bool AudioFormats::isNonInterleaved(AudioFormat format)
	{
		return ((sl_uint32)(format) >> 24) & 1;
	}

	sl_bool AudioFormats::isFloat(AudioFormat format)
	{
		AudioSampleType type = getSampleType(format);
		return type == AudioSampleType::Float || type == AudioSampleType::FloatLE || type == AudioSampleType::FloatBE;
	}

}
