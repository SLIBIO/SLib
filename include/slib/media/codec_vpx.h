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
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(VpxEncoderParam)
		
	};
	
	class SLIB_EXPORT VpxDecoderParam
	{
	public:
		VpxCodecName codec;
		sl_uint32 width;
		sl_uint32 height;
		
	public:
		VpxDecoderParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(VpxDecoderParam)
		
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
