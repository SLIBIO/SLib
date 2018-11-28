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

#include "slib/media/audio_codec.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(AudioEncoder, Object)

	AudioEncoder::AudioEncoder()
	{
		m_nSamplesPerSecond = 0;
		m_nChannels = 0;
		m_bitrate = 0;
	}

	AudioEncoder::~AudioEncoder()
	{
	}

	sl_uint32 AudioEncoder::getSamplesCountPerSecond() const
	{
		return m_nSamplesPerSecond;
	}

	sl_uint32 AudioEncoder::getChannelsCount() const
	{
		return m_nChannels;
	}

	sl_uint32 AudioEncoder::getBitrate()
	{
		return m_bitrate;
	}

	void AudioEncoder::setBitrate(sl_uint32 bitrate)
	{
		m_bitrate = bitrate;
	}


	SLIB_DEFINE_OBJECT(AudioDecoder, Object)

	AudioDecoder::AudioDecoder()
	{
		m_nSamplesPerSecond = 0;
		m_nChannels = 0;
	}

	AudioDecoder::~AudioDecoder()
	{
	}

	sl_uint32 AudioDecoder::getSamplesCountPerSecond() const
	{
		return m_nSamplesPerSecond;
	}

	sl_uint32 AudioDecoder::getChannelsCount() const
	{
		return m_nChannels;
	}

}
