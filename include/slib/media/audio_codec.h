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

#ifndef CHECKHEADER_SLIB_MEDIA_AUDIO_CODEC
#define CHECKHEADER_SLIB_MEDIA_AUDIO_CODEC

#include "definition.h"

#include "audio_data.h"

#include "../core/object.h"
#include "../core/memory.h"

namespace slib
{
	class SLIB_EXPORT AudioEncoder : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		AudioEncoder();
		
		~AudioEncoder();
		
	public:
		virtual Memory encode(const AudioData& input) = 0;
		
	public:
		sl_uint32 getSamplesCountPerSecond() const;
		
		sl_uint32 getChannelsCount() const;
		
		sl_uint32 getBitrate();

		virtual void setBitrate(sl_uint32 bitrate);
		
	protected:
		sl_uint32 m_nSamplesPerSecond;
		sl_uint32 m_nChannels;
		sl_uint32 m_bitrate;
		
	};
	
	class SLIB_EXPORT AudioDecoder : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		AudioDecoder();
		
		~AudioDecoder();
		
	public:
		// returns the count of samples (per channel) actually decoded
		virtual sl_uint32 decode(const void* input, sl_uint32 sizeInput, const AudioData& output) = 0;
		
	public:
		sl_uint32 getSamplesCountPerSecond() const;
		
		sl_uint32 getChannelsCount() const;
		
	protected:
		sl_uint32 m_nSamplesPerSecond;
		sl_uint32 m_nChannels;
		
	};
}

#endif
