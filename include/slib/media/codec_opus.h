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

#ifndef CHECKHEADER_SLIB_MEDIA_CODEC_OPUS
#define CHECKHEADER_SLIB_MEDIA_CODEC_OPUS

#include "definition.h"

#include "audio_codec.h"

/*
	Opus audio codec (RFC 6716)
 ------------------------------------
 
 one frame is (2.5, 5, 10, 20, 40 or 60 ms) of audio data
 
*/

namespace slib
{
	enum class OpusEncoderType
	{
		Voice = 0,
		Music = 1
	};
	
	class SLIB_EXPORT OpusEncoderParam
	{
	public:
		sl_uint32 samplesPerSecond;
		sl_uint32 channelsCount;
		sl_uint32 bitsPerSecond;
		
		OpusEncoderType type;
		
	public:
		OpusEncoderParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OpusEncoderParam)
		
	};
	
	class SLIB_EXPORT OpusEncoder : public AudioEncoder
	{
	public:
		OpusEncoder();
		
		~OpusEncoder();
		
	public:
		static Ref<OpusEncoder> create(const OpusEncoderParam& param);
		
	public:
		static sl_bool isValidSamplingRate(sl_uint32 nSamplesPerSecond);
		
	};
	
	class SLIB_EXPORT OpusDecoderParam
	{
	public:
		sl_uint32 samplesPerSecond;
		sl_uint32 channelsCount;
		
	public:
		OpusDecoderParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(OpusDecoderParam)
		
	};
	
	class SLIB_EXPORT OpusDecoder : public AudioDecoder
	{
	public:
		OpusDecoder();
		
		~OpusDecoder();
		
	public:
		static Ref<OpusDecoder> create(const OpusDecoderParam& param);
		
	public:
		static sl_bool isValidSamplingRate(sl_uint32 nSamplesPerSecond);
		
	};	
}

#endif
