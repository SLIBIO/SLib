/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
