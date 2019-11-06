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

#include "slib/media/audio_player.h"

#include "slib/media/audio_format.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AudioPlayerInfo)

	AudioPlayerInfo::AudioPlayerInfo()
	{
	}

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AudioPlayerBufferParam)

	AudioPlayerBufferParam::AudioPlayerBufferParam()
	{
		streamType = AudioStreamType::Default;
		
		samplesPerSecond = 16000;
		channelsCount = 1;
		frameLengthInMilliseconds = 50;
		maxBufferLengthInMilliseconds = 0;
		
		flagAutoStart = sl_false;
	}


	SLIB_DEFINE_OBJECT(AudioPlayerBuffer, Object)

	AudioPlayerBuffer::AudioPlayerBuffer()
	{
		m_lenBufferMax = 0;
		m_lastSample = 0;
	}

	AudioPlayerBuffer::~AudioPlayerBuffer()
	{
	}
	
	void AudioPlayerBuffer::_init(const AudioPlayerBufferParam& param)
	{
		m_param = param;
		m_lenBufferMax = param.samplesPerSecond * param.maxBufferLengthInMilliseconds / 1000 * param.channelsCount;
	}
	
	const AudioPlayerBufferParam& AudioPlayerBuffer::getParam()
	{
		return m_param;
	}

	void AudioPlayerBuffer::write(const AudioData& audioIn)
	{
		AudioFormat format;
		sl_uint32 nChannels = m_param.channelsCount;
		if (nChannels == 1) {
			format = AudioFormat::Int16_Mono;
		} else {
			format = AudioFormat::Int16_Stereo;
		}
		sl_size nOriginal = m_buffer.getSize() >> 1;
		sl_size nSamples = audioIn.count;
		if (m_lenBufferMax) {
			if (nOriginal >= m_lenBufferMax) {
				return;
			}
			if (nOriginal + nSamples > m_lenBufferMax) {
				nSamples = m_lenBufferMax - nOriginal;
			}
		}
		sl_size countTotal = nChannels * nSamples;
		sl_size sizeTotal = countTotal << 1;
		if (audioIn.format == format && (((sl_size)(audioIn.data)) & 1) == 0) {
			if (audioIn.ref.isNotNull()) {
				MemoryData m;
				m.data = audioIn.data;
				m.size = sizeTotal;
				m.refer = audioIn.ref;
				m_buffer.add(m);
			} else {
				m_buffer.add(Memory::create(audioIn.data, sizeTotal));
			}
		} else {
			Memory mem = Memory::create(sizeTotal);
			if (mem.isNotNull()) {
				AudioData temp;
				temp.format = format;
				temp.data = mem.getData();
				temp.count = nSamples;
				temp.copySamplesFrom(audioIn, nSamples);
				m_buffer.add(mem);
			}
		}
	}
	
	void AudioPlayerBuffer::flush()
	{
		m_buffer.clear();
	}
	
	sl_size AudioPlayerBuffer::getSamplesCountInQueue()
	{
		return m_buffer.getSize() >> 1;
	}

	Array<sl_int16> AudioPlayerBuffer::_getProcessData(sl_uint32 count)
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

	void AudioPlayerBuffer::_processFrame(sl_int16* s, sl_uint32 count)
	{
		if (m_param.event.isNotNull()) {
			m_param.event->set();
		}
		m_param.onPlayAudio(this, count / m_param.channelsCount);
		if (!(m_buffer.pop(s, count << 1))) {
			for (sl_uint32 i = 0; i < count; i++) {
				s[i] = m_lastSample;
			}
		}
		m_lastSample = s[count - 1];
	}

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AudioPlayerParam)

	AudioPlayerParam::AudioPlayerParam()
	{
	}


	SLIB_DEFINE_OBJECT(AudioPlayer, Object)

	AudioPlayer::AudioPlayer()
	{
	}

	AudioPlayer::~AudioPlayer()
	{
	}

	Ref<AudioPlayer> AudioPlayer::create()
	{
		AudioPlayerParam param;
		return create(param);
	}

	Ref<AudioPlayerBuffer> AudioPlayerBuffer::create(const AudioPlayerParam& playerParam, const AudioPlayerBufferParam& bufferParam)
	{
		Ref<AudioPlayer> player = AudioPlayer::create(playerParam);
		if (player.isNotNull()) {
			return player->createBuffer(bufferParam);
		}
		return sl_null;
	}

	Ref<AudioPlayerBuffer> AudioPlayerBuffer::create(const AudioPlayerBufferParam& param)
	{
		return create(AudioPlayerParam(), param);
	}

}
