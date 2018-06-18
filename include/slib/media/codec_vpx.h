/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_CODEC_VPX
#define CHECKHEADER_SLIB_MEDIA_CODEC_VPX

#include "definition.h"

#include "video_codec.h"

namespace slib
{
	enum class VpxBitrateMode
	{
		Variable,
		Constant,
		ConstrainedQuality,
		ConstantQuality
	};
	
	enum class VpxCodecName
	{
		VP8,
		VP9
	};
	
	class SLIB_EXPORT VpxEncoderParam
	{
	public:
		VpxCodecName codec;
		sl_uint32 width;
		sl_uint32 height;
		sl_uint32 framesPerSecond;
		sl_uint32 keyFrameInterval;
		sl_uint32 bitrate;
		sl_uint32 threadsCount;
		sl_uint32 cpuUsage;
		VpxBitrateMode bitrateMode;
		
	public:
		VpxEncoderParam();
		
		~VpxEncoderParam();
		
	};
	
	class SLIB_EXPORT VpxDecoderParam
	{
	public:
		VpxCodecName codec;
		sl_uint32 width;
		sl_uint32 height;
		
	public:
		VpxDecoderParam();
		
		~VpxDecoderParam();
		
	};
	
	class SLIB_EXPORT VpxEncoder : public VideoEncoder
	{
	public:
		VpxEncoder();
		
		~VpxEncoder();
		
	public:
		static Ref<VpxEncoder> create(const VpxEncoderParam& param);
		
	};
	
	class SLIB_EXPORT VpxDecoder : public VideoDecoder
	{
	public:
		VpxDecoder();
		
		~VpxDecoder();
		
	public:
		static Ref<VpxDecoder> create(const VpxDecoderParam& param);
		
	};	
}

#endif
