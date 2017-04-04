/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/media/audio_player.h"

#include "slib/media/audio_format.h"
#include "slib/core/safe_static.h"

namespace slib
{

	AudioPlayerInfo::AudioPlayerInfo()
	{
	}

	AudioPlayerInfo::~AudioPlayerInfo()
	{
	}


	IAudioPlayerBufferListener::IAudioPlayerBufferListener()
	{
	}

	IAudioPlayerBufferListener::~IAudioPlayerBufferListener()
	{
	}


	AudioPlayerBufferParam::AudioPlayerBufferParam()
	{
		samplesPerSecond = 16000;
		channelsCount = 1;
		frameLengthInMilliseconds = 50;
		bufferLengthInMilliseconds = 1000;
		
		flagAutoStart = sl_true;
	}

	AudioPlayerBufferParam::~AudioPlayerBufferParam()
	{
	}


	SLIB_DEFINE_OBJECT(AudioPlayerBuffer, Object)

	AudioPlayerBuffer::AudioPlayerBuffer()
	{
		m_lastSample = 0;
		m_nChannels = 1;
	}

	AudioPlayerBuffer::~AudioPlayerBuffer()
	{
	}
	
	void AudioPlayerBuffer::_init(const AudioPlayerBufferParam& param)
	{
		m_queue.setQueueSize(param.samplesPerSecond * param.bufferLengthInMilliseconds / 1000 * param.channelsCount);
		m_nChannels = param.channelsCount;
		m_listener = param.listener;
		m_onRequireAudioData = param.onRequireAudioData;
		m_event = param.event;
	}

	void AudioPlayerBuffer::write(const AudioData& audioIn)
	{
		AudioFormat format;
		sl_uint32 nChannels = m_nChannels;
		if (nChannels == 1) {
			format = AudioFormat::Int16_Mono;
		} else {
			format = AudioFormat::Int16_Stereo;
		}
		if (audioIn.format == format && (((sl_size)(audioIn.data)) & 1) == 0) {
			m_queue.add((sl_int16*)(audioIn.data), nChannels * audioIn.count);
		} else {
			sl_int16 samples[2048];
			AudioData temp;
			temp.format = format;
			temp.data = samples;
			temp.count = 1024;
			sl_size n = audioIn.count;
			while (n > 0) {
				sl_size m = n;
				if (m > 1024) {
					m = 1024;
				}
				n -= m;
				temp.copySamplesFrom(audioIn, m);
				m_queue.add(samples, nChannels*m);
			}
		}
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
		if (m_event.isNotNull()) {
			m_event->set();
		}
		m_onRequireAudioData(this, count / m_nChannels);
		PtrLocker<IAudioPlayerBufferListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onRequireAudioData(this, count / m_nChannels);
		}
		if (!(m_queue.get(s, count))) {
			for (sl_uint32 i = 0; i < count; i++) {
				s[i] = m_lastSample;
			}
		}
		m_lastSample = s[count - 1];
	}


	AudioPlayerParam::AudioPlayerParam()
	{
	}

	AudioPlayerParam::~AudioPlayerParam()
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

}
