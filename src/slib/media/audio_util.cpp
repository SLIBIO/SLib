/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/media/audio_util.h"

#define AUDIO_UTIL_DEF_CONVERT_SAMPLES(TYPE_IN, TYPE_OUT) \
	void AudioUtil::convertSamples(sl_size count, const TYPE_IN* in, TYPE_OUT* out) \
	{ \
		for (sl_size i = 0; i < count; i++) { \
			convertSample(in[i], out[i]); \
		} \
	}

namespace slib
{

	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int8, sl_int8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int8, sl_uint8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int8, sl_int16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int8, sl_uint16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int8, float)

	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint8, sl_int8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint8, sl_uint8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint8, sl_int16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint8, sl_uint16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint8, float)

	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int16, sl_int8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int16, sl_uint8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int16, sl_int16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int16, sl_uint16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_int16, float)

	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint16, sl_int8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint16, sl_uint8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint16, sl_int16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint16, sl_uint16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(sl_uint16, float)

	AUDIO_UTIL_DEF_CONVERT_SAMPLES(float, sl_int8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(float, sl_uint8)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(float, sl_int16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(float, sl_uint16)
	AUDIO_UTIL_DEF_CONVERT_SAMPLES(float, float)

}
