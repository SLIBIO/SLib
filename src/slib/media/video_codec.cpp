/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/media/video_codec.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(VideoEncoder, Object)

	VideoEncoder::VideoEncoder()
	{
		m_nWidth = 0;
		m_nHeight = 0;
		m_nKeyFrameInterval = 0;
		m_bitrate = 0;
	}

	VideoEncoder::~VideoEncoder()
	{
	}

	sl_uint32 VideoEncoder::getBitrate()
	{
		return m_bitrate;
	}

	void VideoEncoder::setBitrate(sl_uint32 bitrate)
	{
		m_bitrate = bitrate;
	}


	SLIB_DEFINE_OBJECT(VideoDecoder, Object)

	VideoDecoder::VideoDecoder()
	{
		m_nWidth = 0;
		m_nHeight = 0;
	}

	VideoDecoder::~VideoDecoder()
	{
	}

}

