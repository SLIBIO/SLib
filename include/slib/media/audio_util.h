/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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
