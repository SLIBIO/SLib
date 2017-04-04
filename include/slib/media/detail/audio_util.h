/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_DETAIL_AUDIO_UTIL
#define CHECKHEADER_SLIB_MEDIA_DETAIL_AUDIO_UTIL

#include "../audio_util.h"

namespace slib
{

	SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, sl_int8& _out)
	{
		_out = _in;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, sl_uint8& _out)
	{
		_out = (sl_uint8)((sl_int32)_in + 128);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, sl_int16& _out)
	{
		_out = (sl_int16)(_in) << 8;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, sl_uint16& _out)
	{
		_out = (sl_uint16)((sl_int32)_in + 128) << 8;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int8 _in, float& _out)
	{
		_out = (float)(_in) / 128.0f;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, sl_int8& _out)
	{
		_out = (sl_int8)((sl_int32)_in - 128);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, sl_uint8& _out)
	{
		_out = _in;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, sl_int16& _out)
	{
		_out = (sl_int16)(((sl_int32)(_in) << 8) - 0x8000);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, sl_uint16& _out)
	{
		_out = (sl_uint16)(_in) << 8;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint8 _in, float& _out)
	{
		_out = (float)((sl_int8)((sl_int32)_in - 128)) / 128.0f;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, sl_int8& _out)
	{
		_out = (sl_int8)(_in >> 8);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, sl_uint8& _out)
	{
		_out = (sl_uint8)(((sl_uint16)((sl_int32)_in + 0x8000)) >> 8);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, sl_int16& _out)
	{
		_out = _in;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, sl_uint16& _out)
	{
		_out = (sl_uint16)((sl_int32)_in + 0x8000);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_int16 _in, float& _out)
	{
		_out = (float)(_in) / 32768.0f;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, sl_int8& _out)
	{
		_out = (sl_int8)((sl_int32)(_in >> 8) - 128);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, sl_uint8& _out)
	{
		_out = (sl_uint8)(_in >> 8);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, sl_int16& _out)
	{
		_out = (sl_int16)((sl_int32)_in - 0x8000);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, sl_uint16& _out)
	{
		_out = _in;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(sl_uint16 _in, float& _out)
	{
		_out = (float)((sl_int16)((sl_int32)_in - 0x8000)) / 32768.0f;
	}
	
	SLIB_INLINE void AudioUtil::convertSample(float _in, sl_int8& _out)
	{
		_out = (sl_int8)(Math::clamp0_255((sl_int32)(_in * 128.0f) + 128) - 128);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(float _in, sl_uint8& _out)
	{
		_out = (sl_uint8)(Math::clamp0_255((sl_int32)(_in * 128.0f) + 128));
	}
	
	SLIB_INLINE void AudioUtil::convertSample(float _in, sl_int16& _out)
	{
		_out = (sl_int16)(Math::clamp0_65535((sl_int32)(_in * 32768.0f)) - 0x8000);
	}
	
	SLIB_INLINE void AudioUtil::convertSample(float _in, sl_uint16& _out)
	{
		_out = (sl_int16)(Math::clamp0_65535((sl_int32)(_in * 32768.0f)));
	}
	
	SLIB_INLINE void AudioUtil::convertSample(float _in, float& _out)
	{
		_out = _in;
	}
	
	SLIB_INLINE void AudioUtil::mixSamples(sl_int8 in1, sl_int8 in2, sl_int8& _out)
	{
		_out = (sl_int8)(((sl_int32)in1 + (sl_int32)in2) >> 1);
	}
	
	SLIB_INLINE void AudioUtil::mixSamples(sl_uint8 in1, sl_uint8 in2, sl_uint8& _out)
	{
		_out = (sl_uint8)(((sl_uint32)in1 + (sl_uint32)in2) >> 1);
	}
	
	SLIB_INLINE void AudioUtil::mixSamples(sl_int16 in1, sl_int16 in2, sl_int16& _out)
	{
		_out = (sl_int16)(((sl_int32)in1 + (sl_int32)in2) >> 1);
	}
	
	SLIB_INLINE void AudioUtil::mixSamples(sl_uint16 in1, sl_uint16 in2, sl_uint16& _out)
	{
		_out = (sl_uint16)(((sl_int32)in1 + (sl_int32)in2) >> 1);
	}
	
	SLIB_INLINE void AudioUtil::mixSamples(float in1, float in2, float& _out)
	{
		_out = (in1 + in2) / 2;
	}

}

#endif
