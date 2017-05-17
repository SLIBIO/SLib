/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_UTIL
#define CHECKHEADER_SLIB_MEDIA_AUDIO_UTIL

#include "definition.h"

#include "../core/math.h"

namespace slib
{
	class SLIB_EXPORT AudioUtil
	{
	public:
		// S8 => S8
		static void convertSample(sl_int8 _in, sl_int8& _out);
		
		static void convertSamples(sl_size count, const sl_int8* _in, sl_int8* _out);
		
		// S8 => U8
		static void convertSample(sl_int8 _in, sl_uint8& _out);
		
		static void convertSamples(sl_size count, const sl_int8* _in, sl_uint8* _out);
		
		// S8 => S16
		static void convertSample(sl_int8 _in, sl_int16& _out);
		
		static void convertSamples(sl_size count, const sl_int8* _in, sl_int16* _out);
		
		// S8 => U16
		static void convertSample(sl_int8 _in, sl_uint16& _out);
		
		static void convertSamples(sl_size count, const sl_int8* _in, sl_uint16* _out);
		
		// S8 => Float
		static void convertSample(sl_int8 _in, float& _out);
		
		static void convertSamples(sl_size count, const sl_int8* _in, float* _out);
		
		
		// U8 => S8
		static void convertSample(sl_uint8 _in, sl_int8& _out);
		
		static void convertSamples(sl_size count, const sl_uint8* _in, sl_int8* _out);
		
		// U8 => U8
		static void convertSample(sl_uint8 _in, sl_uint8& _out);
		
		static void convertSamples(sl_size count, const sl_uint8* _in, sl_uint8* _out);
		
		// U8 => S16
		static void convertSample(sl_uint8 _in, sl_int16& _out);
		
		static void convertSamples(sl_size count, const sl_uint8* _in, sl_int16* _out);
		
		// U8 => U16
		static void convertSample(sl_uint8 _in, sl_uint16& _out);
		
		static void convertSamples(sl_size count, const sl_uint8* _in, sl_uint16* _out);
		
		// U8 => Float
		static void convertSample(sl_uint8 _in, float& _out);
		
		static void convertSamples(sl_size count, const sl_uint8* _in, float* _out);
		
		
		// S16 => S8
		static void convertSample(sl_int16 _in, sl_int8& _out);
		
		static void convertSamples(sl_size count, const sl_int16* _in, sl_int8* _out);
		
		// S16 => U8
		static void convertSample(sl_int16 _in, sl_uint8& _out);
		
		static void convertSamples(sl_size count, const sl_int16* _in, sl_uint8* _out);
		
		// S16 => S16
		static void convertSample(sl_int16 _in, sl_int16& _out);
		
		static void convertSamples(sl_size count, const sl_int16* _in, sl_int16* _out);
		
		// S16 => U16
		static void convertSample(sl_int16 _in, sl_uint16& _out);
		
		static void convertSamples(sl_size count, const sl_int16* _in, sl_uint16* _out);
		
		// S16 => Float
		static void convertSample(sl_int16 _in, float& _out);
		
		static void convertSamples(sl_size count, const sl_int16* _in, float* _out);
		
		
		
		// U16 => S8
		static void convertSample(sl_uint16 _in, sl_int8& _out);
		
		static void convertSamples(sl_size count, const sl_uint16* _in, sl_int8* _out);
		
		// U16 => U8
		static void convertSample(sl_uint16 _in, sl_uint8& _out);
		
		static void convertSamples(sl_size count, const sl_uint16* _in, sl_uint8* _out);
		
		// U16 => S16
		static void convertSample(sl_uint16 _in, sl_int16& _out);
		
		static void convertSamples(sl_size count, const sl_uint16* _in, sl_int16* _out);
		
		// U16 => U16
		static void convertSample(sl_uint16 _in, sl_uint16& _out);
		
		static void convertSamples(sl_size count, const sl_uint16* _in, sl_uint16* _out);
		
		// U16 => Float
		static void convertSample(sl_uint16 _in, float& _out);
		
		static void convertSamples(sl_size count, const sl_uint16* _in, float* _out);
		
		
		// Float => S8
		static void convertSample(float _in, sl_int8& _out);
		
		static void convertSamples(sl_size count, const float* _in, sl_int8* _out);
		
		// Float => U8
		static void convertSample(float _in, sl_uint8& _out);
		
		static void convertSamples(sl_size count, const float* _in, sl_uint8* _out);
		
		// Float => S16
		static void convertSample(float _in, sl_int16& _out);
		
		static void convertSamples(sl_size count, const float* _in, sl_int16* _out);
		
		// Float => U16
		static void convertSample(float _in, sl_uint16& _out);
		
		static void convertSamples(sl_size count, const float* _in, sl_uint16* _out);
		
		// Float => Float
		static void convertSample(float _in, float& _out);
		
		static void convertSamples(sl_size count, const float* _in, float* _out);
		
		
		static void mixSamples(sl_int8 in1, sl_int8 in2, sl_int8& _out);
		
		static void mixSamples(sl_uint8 in1, sl_uint8 in2, sl_uint8& _out);
		
		static void mixSamples(sl_int16 in1, sl_int16 in2, sl_int16& _out);
		
		static void mixSamples(sl_uint16 in1, sl_uint16 in2, sl_uint16& _out);
		
		static void mixSamples(float in1, float in2, float& _out);
		
	};
	
}

#include "detail/audio_util.inc"

#endif
