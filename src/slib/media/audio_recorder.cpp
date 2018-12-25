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

#include "slib/media/audio_recorder.h"

#include "slib/media/audio_format.h"

namespace slib
{

	AudioRecorderInfo::AudioRecorderInfo()
	{
	}

	AudioRecorderInfo::~AudioRecorderInfo()
	{
	}

	
	AudioRecorderParam::AudioRecorderParam()
	{
		samplesPerSecond = 16000;
		channelsCount = 1;
		frameLengthInMilliseconds = 50;
		bufferLengthInMilliseconds = 1000;
		flagAutoStart = sl_true;
	}

	AudioRecorderParam::~AudioRecorderParam()
	{
	}


	SLIB_DEFINE_OBJECT(AudioRecorder, Object)

	AudioRecorder::AudioRecorder()
	{
		m_nChannels = 1;
	}

	AudioRecorder::~AudioRecorder()
	{
	}

	sl_bool AudioRecorder::read(const AudioData& audioOut)
	{
		AudioFormat format;
		sl_uint32 nChannels = m_nChannels;
		if (nChannels == 1) {
			format = AudioFormat::Int16_Mono;
		} else {
			format = AudioFormat::Int16_Stereo;
		}
		if (audioOut.format == format && (((sl_size)(audioOut.data)) & 1) == 0) {
			return m_queue.pop((sl_int16*)(audioOut.data), nChannels * audioOut.count);
		} else {
			sl_int16 samples[2048];
			AudioData temp;
			temp.format = format;
			temp.data = samples;
			temp.count = 1024;
			sl_size n = audioOut.count;
			ObjectLocker lock(&m_queue);
			if (n > m_queue.getCount()) {
				while (n > 0) {
					sl_size m = n;
					if (m > 1024) {
						m = 1024;
					}
					n -= m;
					m_queue.pop(samples, nChannels*m);
					audioOut.copySamplesFrom(temp, m);
				}
				return sl_true;
			}
			return sl_false;
		}
	}

	void AudioRecorder::_init(const AudioRecorderParam& param)
	{
		m_queue.setQueueSize(param.samplesPerSecond * param.bufferLengthInMilliseconds / 1000 * param.channelsCount);
		m_nChannels = param.channelsCount;
		m_onRecordAudio = param.onRecordAudio;
		m_event = param.event;
	}

	Array<sl_int16> AudioRecorder::_getProcessData(sl_uint32 count)
	{
		Array<sl_int16> data = m_processData;
		if (data.getCount() >= count) {
			return data;
		} else {
			data = Array<sl_int16>::create(count);
			m_processData = data;
			return data;
		}
	}

	void AudioRecorder::_processFrame(sl_int16* s, sl_uint32 count)
	{
		if (m_onRecordAudio.isNotNull()) {
			AudioData audio;
			AudioFormat format;
			sl_uint32 nChannels = m_nChannels;
			if (nChannels == 1) {
				format = AudioFormat::Int16_Mono;
			} else {
				format = AudioFormat::Int16_Stereo;
			}
			audio.format = format;
			audio.count = count / nChannels;
			audio.data = s;
			m_onRecordAudio(this, audio);
		}
		m_queue.push(s, count);
		if (m_event.isNotNull()) {
			m_event->set();
		}
	}

}
