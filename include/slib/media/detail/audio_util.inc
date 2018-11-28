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
