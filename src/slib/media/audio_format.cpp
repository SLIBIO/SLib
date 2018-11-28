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
