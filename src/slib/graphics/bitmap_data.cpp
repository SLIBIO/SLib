/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/graphics/bitmap_data.h"

#include "slib/graphics/yuv.h"

namespace slib
{

	ColorComponentBuffer::ColorComponentBuffer()
	{
		width = 0;
		height = 0;
		data = sl_null;
		sample_stride = 0;
		pitch = 0;
	}

	ColorComponentBuffer::~ColorComponentBuffer()
	{
	}

	BitmapData::BitmapData()
	{
		width = 0;
		height = 0;
		format = BitmapFormat::None;
		
		data = sl_null;
		pitch = 0;
		
		data1 = sl_null;
		pitch1 = 0;
		
		data2 = sl_null;
		pitch2 = 0;

		data3 = sl_null;
		pitch3 = 0;
	}

	BitmapData::BitmapData(const BitmapData& other) = default;

	BitmapData::BitmapData(sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride)
	{
		setFromColors(width, height, colors, stride);
	}

	BitmapData::~BitmapData()
	{
	}

	void*& BitmapData::planeData(sl_uint32 plane)
	{
		return *(void**)(((sl_uint8*)&data) + (((sl_uint8*)&data1)-((sl_uint8*)&data)) * plane);
	}

	void* const& BitmapData::planeData(sl_uint32 plane) const
	{
		return *(void**)(((sl_uint8*)&data) + (((sl_uint8*)&data1)-((sl_uint8*)&data)) * plane);
	}

	sl_int32& BitmapData::planePitch(sl_uint32 plane)
	{
		return *(sl_int32*)(((sl_uint8*)&pitch) + (((sl_uint8*)&pitch1)-((sl_uint8*)&pitch)) * plane);
	}

	sl_int32 const& BitmapData::planePitch(sl_uint32 plane) const
	{
		return *(sl_int32*)(((sl_uint8*)&pitch) + (((sl_uint8*)&pitch1)-((sl_uint8*)&pitch)) * plane);
	}

	sl_int32 BitmapData::calculatePitchAlign1(sl_uint32 width, sl_uint32 bitsPerSample)
	{
		return (width * bitsPerSample + 7) >> 3;
	}

	sl_int32 BitmapData::calculatePitchAlign2(sl_uint32 width, sl_uint32 bitsPerSample)
	{
		return (width * bitsPerSample + 15) >> 4 << 1;
	}

	sl_int32 BitmapData::calculatePitchAlign4(sl_uint32 width, sl_uint32 bitsPerSample)
	{
		return (width * bitsPerSample + 31) >> 5 << 2;
	}

	sl_int32 BitmapData::calculatePitchAlign8(sl_uint32 width, sl_uint32 bitsPerSample)
	{
		return (width * bitsPerSample + 63) >> 6 << 3;
	}

	sl_int32 BitmapData::calculatePitchAlign16(sl_uint32 width, sl_uint32 bitsPerSample)
	{
		return (width * bitsPerSample + 127) >> 7 << 4;
	}

	void BitmapData::fillDefaultValues()
	{
		if (format == BitmapFormat::None) {
			return;
		}
		if (BitmapFormats::isYUV_420(format)) {
			if (width & 1) {
				return;
			}
			if (height & 1) {
				return;
			}
			if (format == BitmapFormat::YUV_I420 || format == BitmapFormat::YUV_YV12) {
				sl_uint32 w2 = width >> 1;
				sl_uint32 h2 = height >> 1;
				if (pitch == 0) {
					pitch = calculatePitchAlign16(width, 8);
				}
				if (!data1) {
					data1 = (sl_uint8*)(data) + pitch * height;
				}
				if (pitch1 == 0) {
					/*
						ceil(m/2/16) = ceil(ceil(m/16)/2)
					*/
					pitch1 = calculatePitchAlign16(w2, 8);
				}
				if (!data2) {
					data2 = (sl_uint8*)(data1) + pitch1 * h2;
				}
				if (pitch2 == 0) {
					pitch2 = pitch1;
				}
			} else {
				if (pitch == 0) {
					pitch = width;
				}
				if (!data1) {
					data1 = (sl_uint8*)(data) + pitch * height;
				}
				if (pitch1 == 0) {
					pitch1 = width;
				}
			}		
		} else {
			sl_uint32 i;
			sl_uint32 n = BitmapFormats::getPlanesCount(format);
			for (i = 0; i < n; i++) {
				sl_int32& p = planePitch(i);
				if (p == 0) {
					p = calculatePitchAlign4(width, BitmapFormats::getBitsPerSample(format));
				}
			}
			for (i = 1; i < n; i++) {
				void*& p = planeData(i);
				if (!p) {
					p = (sl_uint8*)(planeData(i-1)) + planePitch(i-1) * height;
				}
			}
		}
	}

	sl_size BitmapData::getTotalSize() const
	{
		if (format == BitmapFormat::None) {
			return 0;
		}
		BitmapData bd(*this);
		bd.fillDefaultValues();
		if (BitmapFormats::isYUV_420(bd.format)) {
			if (bd.width & 1) {
				return 0;
			}
			if (bd.height & 1) {
				return 0;
			}
			sl_uint32 h2 = bd.height >> 1;
			if (bd.format == BitmapFormat::YUV_I420 || bd.format == BitmapFormat::YUV_YV12) {
				return bd.pitch * bd.height + bd.pitch1 * h2 + bd.pitch2 * h2;
			} else {
				return bd.pitch * bd.height + bd.pitch1 * h2;
			}
		}
		sl_size ret = 0;
		sl_uint32 n = BitmapFormats::getPlanesCount(bd.format);
		for (sl_uint32 i = 0; i < n; i++) {
			ret += (sl_size)(bd.planePitch(i)) * (sl_size)(bd.height);
		}
		return ret;
	}

	sl_uint32 BitmapData::getColorComponentBuffers(ColorComponentBuffer* buffers) const
	{
		BitmapData bd(*this);
		bd.fillDefaultValues();
		switch (bd.format) {
			case BitmapFormat::RGBA:
			case BitmapFormat::RGBA_PA:
			case BitmapFormat::YUVA:
			case BitmapFormat::YUVA_PA:
				if (buffers) {
					for (int i = 0; i < 4; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 4;
						buffers[i].pitch = bd.pitch;
						buffers[i].ref = bd.ref;
					}
					buffers[0].data = ((sl_uint8*)(bd.data));
					buffers[1].data = ((sl_uint8*)(bd.data)) + 1;
					buffers[2].data = ((sl_uint8*)(bd.data)) + 2;
					buffers[3].data = ((sl_uint8*)(bd.data)) + 3;
				}
				return 4;
			case BitmapFormat::BGRA:
			case BitmapFormat::BGRA_PA:
				if (buffers) {
					for (int i = 0; i < 4; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 4;
						buffers[i].pitch = bd.pitch;
						buffers[i].ref = bd.ref;
					}
					buffers[2].data = ((sl_uint8*)(bd.data));
					buffers[1].data = ((sl_uint8*)(bd.data)) + 1;
					buffers[0].data = ((sl_uint8*)(bd.data)) + 2;
					buffers[3].data = ((sl_uint8*)(bd.data)) + 3;
				}
				return 4;
			case BitmapFormat::ARGB:
			case BitmapFormat::ARGB_PA:
				if (buffers) {
					for (int i = 0; i < 4; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 4;
						buffers[i].pitch = bd.pitch;
						buffers[i].ref = bd.ref;
					}
					buffers[3].data = ((sl_uint8*)(bd.data));
					buffers[0].data = ((sl_uint8*)(bd.data)) + 1;
					buffers[1].data = ((sl_uint8*)(bd.data)) + 2;
					buffers[2].data = ((sl_uint8*)(bd.data)) + 3;
				}
				return 4;
			case BitmapFormat::ABGR:
			case BitmapFormat::ABGR_PA:
				if (buffers) {
					for (int i = 0; i < 4; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 4;
						buffers[i].pitch = bd.pitch;
						buffers[i].ref = bd.ref;
					}
					buffers[3].data = ((sl_uint8*)(bd.data));
					buffers[2].data = ((sl_uint8*)(bd.data)) + 1;
					buffers[1].data = ((sl_uint8*)(bd.data)) + 2;
					buffers[0].data = ((sl_uint8*)(bd.data)) + 3;
				}
				return 4;
			case BitmapFormat::RGB:
			case BitmapFormat::YUV444:
				if (buffers) {
					for (int i = 0; i < 3; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 3;
						buffers[i].pitch = bd.pitch;
						buffers[i].ref = bd.ref;
					}
					buffers[0].data = ((sl_uint8*)(bd.data));
					buffers[1].data = ((sl_uint8*)(bd.data)) + 1;
					buffers[2].data = ((sl_uint8*)(bd.data)) + 2;
				}
				return 3;
			case BitmapFormat::BGR:
				if (buffers) {
					for (int i = 0; i < 3; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 3;
						buffers[i].pitch = bd.pitch;
						buffers[i].ref = bd.ref;
					}
					buffers[2].data = ((sl_uint8*)(bd.data));
					buffers[1].data = ((sl_uint8*)(bd.data)) + 1;
					buffers[0].data = ((sl_uint8*)(bd.data)) + 2;
				}
				return 3;
			case BitmapFormat::RGB565BE:
			case BitmapFormat::RGB565LE:
			case BitmapFormat::BGR565BE:
			case BitmapFormat::BGR565LE:
				if (buffers) {
					for (int i = 0; i < 3; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 2;
						buffers[i].pitch = bd.pitch;
						buffers[i].ref = bd.ref;
					}
					buffers[0].data = ((sl_uint8*)(bd.data));
					buffers[1].data = ((sl_uint8*)(bd.data));
					buffers[2].data = ((sl_uint8*)(bd.data));
				}
				return 3;
			case BitmapFormat::GRAY8:
				if (buffers) {
					for (int i = 0; i < 3; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 1;
						buffers[i].pitch = bd.pitch;
						buffers[i].ref = bd.ref;
					}
					buffers[0].data = ((sl_uint8*)(bd.data));
					buffers[1].data = ((sl_uint8*)(bd.data));
					buffers[2].data = ((sl_uint8*)(bd.data));
				}
				return 3;
			case BitmapFormat::RGBA_PLANAR:
			case BitmapFormat::RGBA_PLANAR_PA:
			case BitmapFormat::YUVA_PLANAR:
			case BitmapFormat::YUVA_PLANAR_PA:
				if (buffers) {
					for (int i = 0; i < 4; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 1;
					}
					buffers[0].data = bd.data;
					buffers[0].pitch = bd.pitch;
					buffers[0].ref = bd.ref;
					buffers[1].data = bd.data1;
					buffers[1].pitch = bd.pitch1;
					buffers[1].ref = bd.ref1;
					buffers[2].data = bd.data2;
					buffers[2].pitch = bd.pitch2;
					buffers[2].ref = bd.ref2;
					buffers[3].data = bd.data3;
					buffers[3].pitch = bd.pitch3;
					buffers[3].ref = bd.ref3;
				}
				return 4;
			case BitmapFormat::RGB_PLANAR:
			case BitmapFormat::YUV444_PLANAR:
				if (buffers) {
					for (int i = 0; i < 3; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
						buffers[i].sample_stride = 1;
					}
					buffers[0].data = bd.data;
					buffers[0].pitch = bd.pitch;
					buffers[0].ref = bd.ref;
					buffers[1].data = bd.data1;
					buffers[1].pitch = bd.pitch1;
					buffers[1].ref = bd.ref1;
					buffers[2].data = bd.data2;
					buffers[2].pitch = bd.pitch2;
					buffers[2].ref = bd.ref2;
				}
				return 3;
			case BitmapFormat::YUV_I420:
				if (bd.width & 1) {
					return 0;
				}
				if (bd.height & 1) {
					return 0;
				}
				buffers[0].width = bd.width;
				buffers[0].height = bd.height;
				buffers[0].sample_stride = 1;
				buffers[0].data = bd.data;
				buffers[0].pitch = bd.pitch;
				buffers[0].ref = bd.ref;
				buffers[1].width = bd.width >> 1;
				buffers[1].height = bd.height >> 1;
				buffers[1].sample_stride = 1;
				buffers[1].data = bd.data1;
				buffers[1].pitch = bd.pitch1;
				buffers[1].ref = bd.ref1;
				buffers[2].width = bd.width >> 1;
				buffers[2].height = bd.height >> 1;
				buffers[2].sample_stride = 1;
				buffers[2].data = bd.data2;
				buffers[2].pitch = bd.pitch2;
				buffers[2].ref = bd.ref2;
				return 3;
			case BitmapFormat::YUV_YV12:
				if (bd.width & 1) {
					return 0;
				}
				if (bd.height & 1) {
					return 0;
				}
				buffers[0].width = bd.width;
				buffers[0].height = bd.height;
				buffers[0].sample_stride = 1;
				buffers[0].data = bd.data;
				buffers[0].pitch = bd.pitch;
				buffers[0].ref = bd.ref;
				buffers[1].width = bd.width >> 1;
				buffers[1].height = bd.height >> 1;
				buffers[1].sample_stride = 1;
				buffers[1].data = bd.data2;
				buffers[1].pitch = bd.pitch2;
				buffers[1].ref = bd.ref2;
				buffers[2].width = bd.width >> 1;
				buffers[2].height = bd.height >> 1;
				buffers[2].sample_stride = 1;
				buffers[2].data = bd.data1;
				buffers[2].pitch = bd.pitch1;
				buffers[2].ref = bd.ref1;
				return 3;
			case BitmapFormat::YUV_NV21:
				if (bd.width & 1) {
					return 0;
				}
				if (bd.height & 1) {
					return 0;
				}
				buffers[0].width = bd.width;
				buffers[0].height = bd.height;
				buffers[0].sample_stride = 1;
				buffers[0].data = bd.data;
				buffers[0].pitch = bd.pitch;
				buffers[0].ref = bd.ref;
				buffers[1].width = bd.width >> 1;
				buffers[1].height = bd.height >> 1;
				buffers[1].sample_stride = 2;
				buffers[1].data = ((sl_uint8*)(bd.data1)) + 1;
				buffers[1].pitch = bd.pitch1;
				buffers[1].ref = bd.ref1;
				buffers[2].width = bd.width >> 1;
				buffers[2].height = bd.height >> 1;
				buffers[2].sample_stride = 2;
				buffers[2].data = ((sl_uint8*)(bd.data1));
				buffers[2].pitch = bd.pitch1;
				buffers[2].ref = bd.ref1;
				return 3;
			case BitmapFormat::YUV_NV12:
				if (bd.width & 1) {
					return 0;
				}
				if (bd.height & 1) {
					return 0;
				}
				buffers[0].width = bd.width;
				buffers[0].height = bd.height;
				buffers[0].sample_stride = 1;
				buffers[0].data = bd.data;
				buffers[0].pitch = bd.pitch;
				buffers[0].ref = bd.ref;
				buffers[1].width = bd.width >> 1;
				buffers[1].height = bd.height >> 1;
				buffers[1].sample_stride = 2;
				buffers[1].data = ((sl_uint8*)(bd.data1));
				buffers[1].pitch = bd.pitch1;
				buffers[1].ref = bd.ref1;
				buffers[2].width = bd.width >> 1;
				buffers[2].height = bd.height >> 1;
				buffers[2].sample_stride = 2;
				buffers[2].data = ((sl_uint8*)(bd.data1)) + 1;
				buffers[2].pitch = bd.pitch1;
				buffers[2].ref = bd.ref1;
				return 3;
			default:
				break;
		}
		return 0;
	}

	class RGBA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			r = p[0];
			g = p[1];
			b = p[2];
			a = p[3];
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			p[0] = r;
			p[1] = g;
			p[2] = b;
			p[3] = a;
			p0 += 4;
		}
	};

	class BGRA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			b = p[0];
			g = p[1];
			r = p[2];
			a = p[3];
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			p[0] = b;
			p[1] = g;
			p[2] = r;
			p[3] = a;
			p0 += 4;
		}
	};

	class ARGB_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			a = p[0];
			r = p[1];
			g = p[2];
			b = p[3];
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			p[0] = a;
			p[1] = r;
			p[2] = g;
			p[3] = b;
			p0 += 4;
		}
	};

	class ABGR_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			a = p[0];
			b = p[1];
			g = p[2];
			r = p[3];
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			p[0] = a;
			p[1] = b;
			p[2] = g;
			p[3] = r;
			p0 += 4;
		}
	};


	class RGBA_PA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			Color c;
			c.r = p[0];
			c.g = p[1];
			c.b = p[2];
			c.a = p[3];
			c.convertPAtoNPA();
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
			c.convertNPAtoPA();
			p[0] = c.r;
			p[1] = c.g;
			p[2] = c.b;
			p[3] = c.a;
			p0 += 4;
		}
	};

	class BGRA_PA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			Color c;
			c.b = p[0];
			c.g = p[1];
			c.r = p[2];
			c.a = p[3];
			c.convertPAtoNPA();
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
			c.convertNPAtoPA();
			p[0] = c.b;
			p[1] = c.g;
			p[2] = c.r;
			p[3] = c.a;
			p0 += 4;
		}
	};

	class ARGB_PA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			Color c;
			c.a = p[0];
			c.r = p[1];
			c.g = p[2];
			c.b = p[3];
			c.convertPAtoNPA();
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
			c.convertNPAtoPA();
			p[0] = c.a;
			p[1] = c.r;
			p[2] = c.g;
			p[3] = c.b;
			p0 += 4;
		}
	};

	class ABGR_PA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			Color c;
			c.a = p[0];
			c.b = p[1];
			c.g = p[2];
			c.r = p[3];
			c.convertPAtoNPA();
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
			c.convertNPAtoPA();
			p[0] = c.a;
			p[1] = c.b;
			p[2] = c.g;
			p[3] = c.r;
			p0 += 4;
		}
	};

	class RGB_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			r = p[0];
			g = p[1];
			b = p[2];
			a = 255;
			p0 += 3;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			p[0] = r;
			p[1] = g;
			p[2] = b;
			p0 += 3;
		}
	};

	class BGR_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			b = p[0];
			g = p[1];
			r = p[2];
			a = 255;
			p0 += 3;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			p[0] = b;
			p[1] = g;
			p[2] = r;
			p0 += 3;
		}
	};

	class RGB565BE_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			sl_uint32 s = p[0];
			s = (s << 8) | p[1];
			r = (sl_uint8)((s & 0xF800) >> 8);
			g = (sl_uint8)((s & 0x07E0) >> 3);
			b = (sl_uint8)((s & 0x001F) << 3);
			a = 255;
			p0 += 2;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			sl_uint32 s = r >> 3;
			s = (s << 5) | (g >> 2);
			s = (s << 6) | (b >> 3);
			p[0] = (sl_uint8)(s >> 8);
			p[1] = (sl_uint8)(s);
			p0 += 2;
		}
	};

	class RGB565LE_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			sl_uint16 s = p[1];
			s = (s << 8) | p[0];
			r = (sl_uint8)((s & 0xF800) >> 8);
			g = (sl_uint8)((s & 0x07E0) >> 3);
			b = (sl_uint8)((s & 0x001F) << 3);
			a = 255;
			p0 += 2;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			sl_uint32 s = r >> 3;
			s = (s << 5) | (g >> 2);
			s = (s << 6) | (b >> 3);
			p[1] = (sl_uint8)(s >> 8);
			p[0] = (sl_uint8)(s);
			p0 += 2;
		}
	};

	class BGR565BE_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			sl_uint32 s = p[0];
			s = (s << 8) | p[1];
			b = (sl_uint8)((s & 0xF800) >> 8);
			g = (sl_uint8)((s & 0x07E0) >> 3);
			r = (sl_uint8)((s & 0x001F) << 3);
			a = 255;
			p0 += 2;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			sl_uint32 s = b >> 3;
			s = (s << 5) | (g >> 2);
			s = (s << 6) | (r >> 3);
			p[0] = (sl_uint8)(s >> 8);
			p[1] = (sl_uint8)(s);
			p0 += 2;
		}
	};

	class BGR565LE_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			sl_uint16 s = p[1];
			s = (s << 8) | p[0];
			b = (sl_uint8)((s & 0xF800) >> 8);
			g = (sl_uint8)((s & 0x07E0) >> 3);
			r = (sl_uint8)((s & 0x001F) << 3);
			a = 255;
			p0 += 2;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			sl_uint32 s = b >> 3;
			s = (s << 5) | (g >> 2);
			s = (s << 6) | (r >> 3);
			p[1] = (sl_uint8)(s >> 8);
			p[0] = (sl_uint8)(s);
			p0 += 2;
		}
	};

	class GRAY8_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			sl_uint8 v = p[0];
			r = v;
			g = v;
			b = v;
			a = 255;
			p0++;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			sl_uint32 v = r;
			v += g;
			v += b;
			p[0] = (sl_uint8)(v / 3);
			p0++;
		}
	};

	class RGBA_PLANAR_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			r = p0[0];
			g = p1[0];
			b = p2[0];
			a = p3[0];
			p0++;
			p1++;
			p2++;
			p3++;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			p0[0] = r;
			p1[0] = g;
			p2[0] = b;
			p3[0] = a;
			p0++;
			p1++;
			p2++;
			p3++;
		}
	};

	class RGBA_PLANAR_PA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			Color c;
			c.r = p0[0];
			c.g = p1[0];
			c.b = p2[0];
			c.a = p3[0];
			c.convertPAtoNPA();
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			p0++;
			p1++;
			p2++;
			p3++;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
			c.convertNPAtoPA();
			p0[0] = c.r;
			p1[0] = c.g;
			p2[0] = c.b;
			p3[0] = c.a;
			p0++;
			p1++;
			p2++;
			p3++;
		}
	};

	class RGB_PLANAR_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			r = p0[0];
			g = p1[0];
			b = p2[0];
			a = 255;
			p0++;
			p1++;
			p2++;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			p0[0] = r;
			p1[0] = g;
			p2[0] = b;
			p0++;
			p1++;
			p2++;
		}
	};

	class YUVA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			YUV::convertYUVToRGB(p[0], p[1], p[2], r, g, b);
			a = p[3];
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			YUV::convertRGBToYUV(r, g, b, p[0], p[1], p[2]);
			p[3] = a;
			p0 += 4;
		}
	};

	class YUVA_PLANAR_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			YUV::convertYUVToRGB(p0[0], p1[0], p2[0], r, g, b);
			a = p3[0];
			p0++;
			p1++;
			p2++;
			p3++;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			YUV::convertRGBToYUV(r, g, b, p0[0], p1[0], p2[0]);
			p3[0] = a;
			p0++;
			p1++;
			p2++;
			p3++;
		}
	};

	class YUVA_PA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			Color c;
			YUV::convertYUVToRGB(p[0], p[1], p[2], c.r, c.g, c.b);
			c.a = p3[0];
			c.convertPAtoNPA();
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			p0 += 4;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
			c.convertNPAtoPA();
			YUV::convertRGBToYUV(c.r, c.g, c.b, p[0], p[1], p[2]);
			p[3] = c.a;
			p0 += 4;
		}
	};

	class YUVA_PLANAR_PA_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			Color c;
			YUV::convertYUVToRGB(p0[0], p1[0], p2[0], c.r, c.g, c.b);
			c.a = p3[0];
			c.convertPAtoNPA();
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			p0++;
			p1++;
			p2++;
			p3++;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			Color c;
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = a;
			c.convertNPAtoPA();
			YUV::convertRGBToYUV(c.r, c.g, c.b, p0[0], p1[0], p2[0]);
			p3[0] = c.a;
			p0++;
			p1++;
			p2++;
			p3++;
		}
	};

	class YUV444_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			sl_uint8* p = p0;
			YUV::convertYUVToRGB(p[0], p[1], p[2], r, g, b);
			a = 255;
			p0 += 3;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			sl_uint8* p = p0;
			YUV::convertRGBToYUV(r, g, b, p[0], p[1], p[2]);
			p0 += 3;
		}
	};

	class YUV444_PLANAR_PROC
	{
	public:
		SLIB_INLINE static void readSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
		{
			YUV::convertYUVToRGB(p0[0], p1[0], p2[0], r, g, b);
			a = 255;
			p0++;
			p1++;
			p2++;
		}
		
		SLIB_INLINE static void writeSample(sl_uint8*& p0, sl_uint8*& p1, sl_uint8*& p2, sl_uint8*& p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
		{
			YUV::convertRGBToYUV(r, g, b, p0[0], p1[0], p2[0]);
			p0++;
			p1++;
			p2++;
		}
	};

	template<class SourceProc, class TargetProc>
	void _BitmapData_copyPixels_Normal_Step2(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, sl_uint8** dst_planes, sl_int32* dst_pitches)
	{
		sl_uint8* sr0 = (sl_uint8*)(src_planes[0]);
		sl_uint8* sr1 = (sl_uint8*)(src_planes[1]);
		sl_uint8* sr2 = (sl_uint8*)(src_planes[2]);
		sl_uint8* sr3 = (sl_uint8*)(src_planes[3]);
		sl_uint8* dr0 = (sl_uint8*)(dst_planes[0]);
		sl_uint8* dr1 = (sl_uint8*)(dst_planes[1]);
		sl_uint8* dr2 = (sl_uint8*)(dst_planes[2]);
		sl_uint8* dr3 = (sl_uint8*)(dst_planes[3]);
		sl_uint8 c0, c1, c2, c3;
		for (sl_uint32 i = 0; i < height; i++) {
			sl_uint8* ss0 = sr0;
			sl_uint8* ss1 = sr1;
			sl_uint8* ss2 = sr2;
			sl_uint8* ss3 = sr3;
			sl_uint8* ds0 = dr0;
			sl_uint8* ds1 = dr1;
			sl_uint8* ds2 = dr2;
			sl_uint8* ds3 = dr3;
			for (sl_uint32 j = 0; j < width; j++) {
				SourceProc::readSample(ss0, ss1, ss2, ss3, c0, c1, c2, c3);
				TargetProc::writeSample(ds0, ds1, ds2, ds3, c0, c1, c2, c3);
			}
			sr0 += src_pitches[0];
			sr1 += src_pitches[1];
			sr2 += src_pitches[2];
			sr3 += src_pitches[3];
			dr0 += dst_pitches[0];
			dr1 += dst_pitches[1];
			dr2 += dst_pitches[2];
			dr3 += dst_pitches[3];
		}
	}

	template<class SourceProc>
	void _BitmapData_copyPixels_Normal_Step1(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches)
	{
		switch (dst_format) {
			case BitmapFormat::RGBA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, RGBA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, RGBA_PA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGRA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, BGRA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGRA_PA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, BGRA_PA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ARGB:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, ARGB_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ARGB_PA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, ARGB_PA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ABGR:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, ABGR_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ABGR_PA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, ABGR_PA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, RGB_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, BGR_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB565BE:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, RGB565BE_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB565LE:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, RGB565LE_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR565BE:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, BGR565BE_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR565LE:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, BGR565LE_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::GRAY8:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, GRAY8_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PLANAR:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, RGBA_PLANAR_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PLANAR_PA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, RGBA_PLANAR_PA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB_PLANAR:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, RGB_PLANAR_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, YUVA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, YUVA_PA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PLANAR:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, YUVA_PLANAR_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PLANAR_PA:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, YUVA_PLANAR_PA_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUV444:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, YUV444_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUV444_PLANAR:
				_BitmapData_copyPixels_Normal_Step2<SourceProc, YUV444_PLANAR_PROC>(width, height, src_planes, src_pitches, dst_planes, dst_pitches);
				break;
			default:
				break;
		}
	}

	void _BitmapData_copyPixels_Normal(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8** src_planes, sl_int32* src_pitches, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches)
	{
		switch (src_format) {
			case BitmapFormat::RGBA:
				_BitmapData_copyPixels_Normal_Step1<RGBA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PA:
				_BitmapData_copyPixels_Normal_Step1<RGBA_PA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGRA:
				_BitmapData_copyPixels_Normal_Step1<BGRA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGRA_PA:
				_BitmapData_copyPixels_Normal_Step1<BGRA_PA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ARGB:
				_BitmapData_copyPixels_Normal_Step1<ARGB_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ARGB_PA:
				_BitmapData_copyPixels_Normal_Step1<ARGB_PA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ABGR:
				_BitmapData_copyPixels_Normal_Step1<ABGR_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ABGR_PA:
				_BitmapData_copyPixels_Normal_Step1<ABGR_PA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB:
				_BitmapData_copyPixels_Normal_Step1<RGB_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR:
				_BitmapData_copyPixels_Normal_Step1<BGR_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB565BE:
				_BitmapData_copyPixels_Normal_Step1<RGB565BE_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB565LE:
				_BitmapData_copyPixels_Normal_Step1<RGB565LE_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR565BE:
				_BitmapData_copyPixels_Normal_Step1<BGR565BE_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR565LE:
				_BitmapData_copyPixels_Normal_Step1<BGR565LE_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::GRAY8:
				_BitmapData_copyPixels_Normal_Step1<GRAY8_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PLANAR:
				_BitmapData_copyPixels_Normal_Step1<RGBA_PLANAR_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PLANAR_PA:
				_BitmapData_copyPixels_Normal_Step1<RGBA_PLANAR_PA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB_PLANAR:
				_BitmapData_copyPixels_Normal_Step1<RGB_PLANAR_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA:
				_BitmapData_copyPixels_Normal_Step1<YUVA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PA:
				_BitmapData_copyPixels_Normal_Step1<YUVA_PA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PLANAR:
				_BitmapData_copyPixels_Normal_Step1<YUVA_PLANAR_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PLANAR_PA:
				_BitmapData_copyPixels_Normal_Step1<YUVA_PLANAR_PA_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUV444:
				_BitmapData_copyPixels_Normal_Step1<YUV444_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUV444_PLANAR:
				_BitmapData_copyPixels_Normal_Step1<YUV444_PLANAR_PROC>(width, height, src_planes, src_pitches, dst_format, dst_planes, dst_pitches);
				break;
			default:
				break;
		}
	}

	template<class TargetProc>
	void _BitmapData_copyPixels_YUV420ToYUV_Step1(sl_uint32 width, sl_uint32 height, BitmapData& src, sl_uint8** dst_planes, sl_int32* dst_pitches)
	{
		ColorComponentBuffer src_cb[3];
		if (src.getColorComponentBuffers(src_cb) != 3) {
			return;
		}
		sl_uint32 W2 = width >> 1;
		sl_uint32 H2 = height >> 1;
		sl_uint8* sry = (sl_uint8*)(src_cb[0].data);
		sl_uint8* sru = (sl_uint8*)(src_cb[1].data);
		sl_uint8* srv = (sl_uint8*)(src_cb[2].data);
		sl_uint8* dr0 = (sl_uint8*)(dst_planes[0]);
		sl_uint8* dr1 = (sl_uint8*)(dst_planes[1]);
		sl_uint8* dr2 = (sl_uint8*)(dst_planes[2]);
		sl_uint8* dr3 = (sl_uint8*)(dst_planes[3]);
		sl_int32 strideUV = src_cb[1].sample_stride;
		for (sl_uint32 i = 0; i < H2; i++) {
			sl_uint8* ssy_u = sry;
			sl_uint8* ssy_d = sry + src_cb[0].pitch;
			sl_uint8* ssu = sru;
			sl_uint8* ssv = srv;
			sl_uint8* ds0_u = dr0;
			sl_uint8* ds1_u = dr1;
			sl_uint8* ds2_u = dr2;
			sl_uint8* ds3_u = dr3;
			sl_uint8* ds0_d = dr0 + dst_pitches[0];
			sl_uint8* ds1_d = dr1 + dst_pitches[1];
			sl_uint8* ds2_d = dr2 + dst_pitches[2];
			sl_uint8* ds3_d = dr3 + dst_pitches[3];
			for (sl_uint32 j = 0; j < W2; j++) {
				TargetProc::writeSample(ds0_u, ds1_u, ds2_u, ds3_u, *ssy_u, *ssu, *ssv, 255);
				ssy_u++;
				TargetProc::writeSample(ds0_u, ds1_u, ds2_u, ds3_u, *ssy_u, *ssu, *ssv, 255);
				ssy_u++;
				TargetProc::writeSample(ds0_d, ds1_d, ds2_d, ds3_d, *ssy_d, *ssu, *ssv, 255);
				ssy_d++;
				TargetProc::writeSample(ds0_d, ds1_d, ds2_d, ds3_d, *ssy_d, *ssu, *ssv, 255);
				ssy_d++;
				ssu += strideUV;
				ssv += strideUV;
			}
			sry += src_cb[0].pitch + src_cb[0].pitch;
			sru += src_cb[1].pitch;
			srv += src_cb[2].pitch;
			dr0 += dst_pitches[0] + dst_pitches[0];
			dr1 += dst_pitches[1] + dst_pitches[1];
			dr2 += dst_pitches[2] + dst_pitches[2];
			dr3 += dst_pitches[3] + dst_pitches[3];
		}
	}

	void _BitmapData_copyPixels_YUV420ToYUV(sl_uint32 width, sl_uint32 height, BitmapData& src, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches)
	{
		switch (dst_format) {
			case BitmapFormat::YUVA:
				_BitmapData_copyPixels_YUV420ToYUV_Step1<RGBA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PA:
				_BitmapData_copyPixels_YUV420ToYUV_Step1<RGBA_PA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PLANAR:
				_BitmapData_copyPixels_YUV420ToYUV_Step1<RGBA_PLANAR_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUVA_PLANAR_PA:
				_BitmapData_copyPixels_YUV420ToYUV_Step1<RGBA_PLANAR_PA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUV444:
				_BitmapData_copyPixels_YUV420ToYUV_Step1<RGB_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::YUV444_PLANAR:
				_BitmapData_copyPixels_YUV420ToYUV_Step1<RGB_PLANAR_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			default:
				break;
		}
	}

	template<class TargetProc>
	void _BitmapData_copyPixels_YUV420ToOther_Step1(sl_uint32 width, sl_uint32 height, BitmapData& src, sl_uint8** dst_planes, sl_int32* dst_pitches)
	{
		ColorComponentBuffer src_cb[3];
		if (src.getColorComponentBuffers(src_cb) != 3) {
			return;
		}
		sl_uint32 W2 = width >> 1;
		sl_uint32 H2 = height >> 1;
		sl_uint8* sry = (sl_uint8*)(src_cb[0].data);
		sl_uint8* sru = (sl_uint8*)(src_cb[1].data);
		sl_uint8* srv = (sl_uint8*)(src_cb[2].data);
		sl_uint8* dr0 = (sl_uint8*)(dst_planes[0]);
		sl_uint8* dr1 = (sl_uint8*)(dst_planes[1]);
		sl_uint8* dr2 = (sl_uint8*)(dst_planes[2]);
		sl_uint8* dr3 = (sl_uint8*)(dst_planes[3]);
		sl_int32 strideUV = src_cb[1].sample_stride;
		sl_uint8 r, g, b;
		for (sl_uint32 i = 0; i < H2; i++) {
			sl_uint8* ssy_u = sry;
			sl_uint8* ssy_d = sry + src_cb[0].pitch;
			sl_uint8* ssu = sru;
			sl_uint8* ssv = srv;
			sl_uint8* ds0_u = dr0;
			sl_uint8* ds1_u = dr1;
			sl_uint8* ds2_u = dr2;
			sl_uint8* ds3_u = dr3;
			sl_uint8* ds0_d = dr0 + dst_pitches[0];
			sl_uint8* ds1_d = dr1 + dst_pitches[1];
			sl_uint8* ds2_d = dr2 + dst_pitches[2];
			sl_uint8* ds3_d = dr3 + dst_pitches[3];
			for (sl_uint32 j = 0; j < W2; j++) {
				YUV::convertYUVToRGB(*ssy_u, *ssu, *ssv, r, g, b);
				TargetProc::writeSample(ds0_u, ds1_u, ds2_u, ds3_u, r, g, b, 255);
				ssy_u++;
				YUV::convertYUVToRGB(*ssy_u, *ssu, *ssv, r, g, b);
				TargetProc::writeSample(ds0_u, ds1_u, ds2_u, ds3_u, r, g, b, 255);
				ssy_u++;
				YUV::convertYUVToRGB(*ssy_d, *ssu, *ssv, r, g, b);
				TargetProc::writeSample(ds0_d, ds1_d, ds2_d, ds3_d, r, g, b, 255);
				ssy_d++;
				YUV::convertYUVToRGB(*ssy_d, *ssu, *ssv, r, g, b);
				TargetProc::writeSample(ds0_d, ds1_d, ds2_d, ds3_d, r, g, b, 255);
				ssy_d++;
				ssu += strideUV;
				ssv += strideUV;
			}
			sry += src_cb[0].pitch + src_cb[0].pitch;
			sru += src_cb[1].pitch;
			srv += src_cb[2].pitch;
			dr0 += dst_pitches[0] + dst_pitches[0];
			dr1 += dst_pitches[1] + dst_pitches[1];
			dr2 += dst_pitches[2] + dst_pitches[2];
			dr3 += dst_pitches[3] + dst_pitches[3];
		}
	}

	void _BitmapData_copyPixels_YUV420ToOther(sl_uint32 width, sl_uint32 height, BitmapData& src, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches)
	{
		switch (dst_format) {
			case BitmapFormat::RGBA:
				_BitmapData_copyPixels_YUV420ToOther_Step1<RGBA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PA:
				_BitmapData_copyPixels_YUV420ToOther_Step1<RGBA_PA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGRA:
				_BitmapData_copyPixels_YUV420ToOther_Step1<BGRA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGRA_PA:
				_BitmapData_copyPixels_YUV420ToOther_Step1<BGRA_PA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ARGB:
				_BitmapData_copyPixels_YUV420ToOther_Step1<ARGB_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ARGB_PA:
				_BitmapData_copyPixels_YUV420ToOther_Step1<ARGB_PA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ABGR:
				_BitmapData_copyPixels_YUV420ToOther_Step1<ABGR_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::ABGR_PA:
				_BitmapData_copyPixels_YUV420ToOther_Step1<ABGR_PA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB:
				_BitmapData_copyPixels_YUV420ToOther_Step1<RGB_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR:
				_BitmapData_copyPixels_YUV420ToOther_Step1<BGR_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB565BE:
				_BitmapData_copyPixels_YUV420ToOther_Step1<RGB565BE_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB565LE:
				_BitmapData_copyPixels_YUV420ToOther_Step1<RGB565LE_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR565BE:
				_BitmapData_copyPixels_YUV420ToOther_Step1<BGR565BE_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::BGR565LE:
				_BitmapData_copyPixels_YUV420ToOther_Step1<BGR565LE_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::GRAY8:
				_BitmapData_copyPixels_YUV420ToOther_Step1<GRAY8_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PLANAR:
				_BitmapData_copyPixels_YUV420ToOther_Step1<RGBA_PLANAR_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGBA_PLANAR_PA:
				_BitmapData_copyPixels_YUV420ToOther_Step1<RGBA_PLANAR_PA_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			case BitmapFormat::RGB_PLANAR:
				_BitmapData_copyPixels_YUV420ToOther_Step1<RGB_PLANAR_PROC>(width, height, src, dst_planes, dst_pitches);
				break;
			default:
				break;
		}
	}


	template<class SourceProc>
	void _BitmapData_copyPixels_YUVToYUV420_Step1(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, BitmapData& dst)
	{
		ColorComponentBuffer dst_cb[3];
		if (dst.getColorComponentBuffers(dst_cb) != 3) {
			return;
		}
		sl_uint32 W2 = width >> 1;
		sl_uint32 H2 = height >> 1;
		sl_uint8* sr0 = (sl_uint8*)(src_planes[0]);
		sl_uint8* sr1 = (sl_uint8*)(src_planes[1]);
		sl_uint8* sr2 = (sl_uint8*)(src_planes[2]);
		sl_uint8* sr3 = (sl_uint8*)(src_planes[3]);
		sl_uint8* dry = (sl_uint8*)(dst_cb[0].data);
		sl_uint8* dru = (sl_uint8*)(dst_cb[1].data);
		sl_uint8* drv = (sl_uint8*)(dst_cb[2].data);
		sl_int32 strideUV = dst_cb[1].sample_stride;
		sl_uint8 U, V, A;
		sl_uint32 TU, TV;
		for (sl_uint32 i = 0; i < H2; i++) {
			sl_uint8* ss0_u = sr0;
			sl_uint8* ss1_u = sr1;
			sl_uint8* ss2_u = sr2;
			sl_uint8* ss3_u = sr3;
			sl_uint8* ss0_d = sr0 + src_pitches[0];
			sl_uint8* ss1_d = sr1 + src_pitches[1];
			sl_uint8* ss2_d = sr2 + src_pitches[2];
			sl_uint8* ss3_d = sr3 + src_pitches[3];
			sl_uint8* dsy_u = dry;
			sl_uint8* dsy_d = dry + dst_cb[0].pitch;
			sl_uint8* dsu = dru;
			sl_uint8* dsv = drv;
			for (sl_uint32 j = 0; j < W2; j++) {
				SourceProc::readSample(ss0_u, ss1_u, ss2_u, ss3_u, *dsy_u, U, V, A);
				dsy_u++;
				TU = U;
				TV = V;
				SourceProc::readSample(ss0_u, ss1_u, ss2_u, ss3_u, *dsy_u, U, V, A);
				dsy_u++;
				TU += U;
				TV += V;
				SourceProc::readSample(ss0_d, ss1_d, ss2_d, ss3_d, *dsy_d, U, V, A);
				dsy_d++;
				TU += U;
				TV += V;
				SourceProc::readSample(ss0_d, ss1_d, ss2_d, ss3_d, *dsy_d, U, V, A);
				dsy_d++;
				TU += U;
				TV += V;
				*dsu = TU >> 2;
				*dsv = TV >> 2;
				dsu += strideUV;
				dsv += strideUV;
			}
			sr0 += src_pitches[0] + src_pitches[0];
			sr1 += src_pitches[1] + src_pitches[1];
			sr2 += src_pitches[2] + src_pitches[2];
			sr3 += src_pitches[3] + src_pitches[3];
			dry += dst_cb[0].pitch + dst_cb[0].pitch;
			dru += dst_cb[1].pitch;
			drv += dst_cb[2].pitch;
		}
	}

	void _BitmapData_copyPixels_YUVToYUV420(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8** src_planes, sl_int32* src_pitches, BitmapData& dst)
	{
		switch (src_format) {
			case BitmapFormat::YUVA:
				_BitmapData_copyPixels_YUVToYUV420_Step1<RGBA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::YUVA_PA:
				_BitmapData_copyPixels_YUVToYUV420_Step1<RGBA_PA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::YUVA_PLANAR:
				_BitmapData_copyPixels_YUVToYUV420_Step1<RGBA_PLANAR_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::YUVA_PLANAR_PA:
				_BitmapData_copyPixels_YUVToYUV420_Step1<RGBA_PLANAR_PA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::YUV444:
				_BitmapData_copyPixels_YUVToYUV420_Step1<RGB_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::YUV444_PLANAR:
				_BitmapData_copyPixels_YUVToYUV420_Step1<RGB_PLANAR_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			default:
				break;
		}
	}

	template<class SourceProc>
	void _BitmapData_copyPixels_OtherToYUV420_Step1(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, BitmapData& dst)
	{
		ColorComponentBuffer dst_cb[3];
		if (dst.getColorComponentBuffers(dst_cb) != 3) {
			return;
		}
		sl_uint32 W2 = width >> 1;
		sl_uint32 H2 = height >> 1;
		sl_uint8* sr0 = (sl_uint8*)(src_planes[0]);
		sl_uint8* sr1 = (sl_uint8*)(src_planes[1]);
		sl_uint8* sr2 = (sl_uint8*)(src_planes[2]);
		sl_uint8* sr3 = (sl_uint8*)(src_planes[3]);
		sl_uint8* dry = (sl_uint8*)(dst_cb[0].data);
		sl_uint8* dru = (sl_uint8*)(dst_cb[1].data);
		sl_uint8* drv = (sl_uint8*)(dst_cb[2].data);
		sl_int32 strideUV = dst_cb[1].sample_stride;
		sl_uint8 R, G, B, A;
		sl_uint8 U, V;
		sl_uint32 TU, TV;
		for (sl_uint32 i = 0; i < H2; i++) {
			sl_uint8* ss0_u = sr0;
			sl_uint8* ss1_u = sr1;
			sl_uint8* ss2_u = sr2;
			sl_uint8* ss3_u = sr3;
			sl_uint8* ss0_d = sr0 + src_pitches[0];
			sl_uint8* ss1_d = sr1 + src_pitches[1];
			sl_uint8* ss2_d = sr2 + src_pitches[2];
			sl_uint8* ss3_d = sr3 + src_pitches[3];
			sl_uint8* dsy_u = dry;
			sl_uint8* dsy_d = dry + dst_cb[0].pitch;
			sl_uint8* dsu = dru;
			sl_uint8* dsv = drv;
			for (sl_uint32 j = 0; j < W2; j++) {
				SourceProc::readSample(ss0_u, ss1_u, ss2_u, ss3_u, R, G, B, A);
				YUV::convertRGBToYUV(R, G, B, *dsy_u, U, V);
				dsy_u++;
				TU = U;
				TV = V;
				SourceProc::readSample(ss0_u, ss1_u, ss2_u, ss3_u, R, G, B, A);
				YUV::convertRGBToYUV(R, G, B, *dsy_u, U, V);
				dsy_u++;
				TU += U;
				TV += V;
				SourceProc::readSample(ss0_d, ss1_d, ss2_d, ss3_d, R, G, B, A);
				YUV::convertRGBToYUV(R, G, B, *dsy_d, U, V);
				dsy_d++;
				TU += U;
				TV += V;
				SourceProc::readSample(ss0_d, ss1_d, ss2_d, ss3_d, R, G, B, A);
				YUV::convertRGBToYUV(R, G, B, *dsy_d, U, V);
				dsy_d++;
				TU += U;
				TV += V;
				*dsu = TU >> 2;
				*dsv = TV >> 2;
				dsu += strideUV;
				dsv += strideUV;
			}
			sr0 += src_pitches[0] + src_pitches[0];
			sr1 += src_pitches[1] + src_pitches[1];
			sr2 += src_pitches[2] + src_pitches[2];
			sr3 += src_pitches[3] + src_pitches[3];
			dry += dst_cb[0].pitch + dst_cb[0].pitch;
			dru += dst_cb[1].pitch;
			drv += dst_cb[2].pitch;
		}
	}

	void _BitmapData_copyPixels_OtherToYUV420(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8** src_planes, sl_int32* src_pitches, BitmapData& dst)
	{
		switch (src_format) {
			case BitmapFormat::RGBA:
				_BitmapData_copyPixels_OtherToYUV420_Step1<RGBA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::RGBA_PA:
				_BitmapData_copyPixels_OtherToYUV420_Step1<RGBA_PA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::BGRA:
				_BitmapData_copyPixels_OtherToYUV420_Step1<BGRA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::BGRA_PA:
				_BitmapData_copyPixels_OtherToYUV420_Step1<BGRA_PA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::ARGB:
				_BitmapData_copyPixels_OtherToYUV420_Step1<ARGB_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::ARGB_PA:
				_BitmapData_copyPixels_OtherToYUV420_Step1<ARGB_PA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::ABGR:
				_BitmapData_copyPixels_OtherToYUV420_Step1<ABGR_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::ABGR_PA:
				_BitmapData_copyPixels_OtherToYUV420_Step1<ABGR_PA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::RGB:
				_BitmapData_copyPixels_OtherToYUV420_Step1<RGB_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::BGR:
				_BitmapData_copyPixels_OtherToYUV420_Step1<BGR_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::RGB565BE:
				_BitmapData_copyPixels_OtherToYUV420_Step1<RGB565BE_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::RGB565LE:
				_BitmapData_copyPixels_OtherToYUV420_Step1<RGB565LE_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::BGR565BE:
				_BitmapData_copyPixels_OtherToYUV420_Step1<BGR565BE_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::BGR565LE:
				_BitmapData_copyPixels_OtherToYUV420_Step1<BGR565LE_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::GRAY8:
				_BitmapData_copyPixels_OtherToYUV420_Step1<GRAY8_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::RGBA_PLANAR:
				_BitmapData_copyPixels_OtherToYUV420_Step1<RGBA_PLANAR_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::RGBA_PLANAR_PA:
				_BitmapData_copyPixels_OtherToYUV420_Step1<RGBA_PLANAR_PA_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			case BitmapFormat::RGB_PLANAR:
				_BitmapData_copyPixels_OtherToYUV420_Step1<RGB_PLANAR_PROC>(width, height, src_planes, src_pitches, dst);
				break;
			default:
				break;
		}
	}

	void BitmapData::copyPixelsFrom(const BitmapData& _other) const
	{
		BitmapData dst(*this);
		BitmapData src(_other);
		if (BitmapFormats::isYUV_420(src.format)) {
			if (src.width & 1) {
				return;
			}
			if (src.height & 1) {
				return;
			}
		}
		if (BitmapFormats::isYUV_420(dst.format)) {
			if (dst.width & 1) {
				return;
			}
			if (dst.height & 1) {
				return;
			}
		}
		sl_uint32 width = SLIB_MIN(src.width, dst.width);
		sl_uint32 height = SLIB_MIN(src.height, dst.height);
		if (BitmapFormats::isYUV_420(src.format) || BitmapFormats::isYUV_420(dst.format)) {
			width = width & 0xFFFFFFFE;
			height = height & 0xFFFFFFFE;
		}
		if (width == 0 || height == 0) {
			return;
		}
		
		src.fillDefaultValues();
		dst.fillDefaultValues();

		sl_uint8* src_planes[4];
		sl_int32 src_pitches[4];
		sl_uint8* dst_planes[4];
		sl_int32 dst_pitches[4];
		
		for (sl_uint32 i = 0; i < 4; i++) {
			src_planes[i] = (sl_uint8*)(src.planeData(i));
			src_pitches[i] = src.planePitch(i);
			dst_planes[i] = (sl_uint8*)(dst.planeData(i));
			dst_pitches[i] = dst.planePitch(i);
		}

		if (src.format == dst.format) {
			sl_uint32 n = BitmapFormats::getPlanesCount(src.format);
			sl_uint32 i = 0;
			for (; i < n; i++) {
				if (src_planes[i] != dst_planes[i] || src_pitches[i] == dst_pitches[i]) {
					break;
				}
			}
			if (i == n) {
				return;
			}
		}

		if (BitmapFormats::isYUV_420(src.format)) {
			
			if (BitmapFormats::isYUV_420(dst.format)) {
				// yuv420 -> yuv420
				ColorComponentBuffer src_cb[3];
				ColorComponentBuffer dst_cb[3];
				if (src.getColorComponentBuffers(src_cb) != 3) {
					return;
				}
				if (dst.getColorComponentBuffers(dst_cb) != 3) {
					return;
				}
				for (sl_uint32 iPlane = 0; iPlane < 3; iPlane++) {
					sl_uint32 w, h;
					if (iPlane == 0) {
						w = width;
						h = height;
					} else {
						w = width >> 1;
						h = height >> 1;
					}
					sl_int32 src_stride = src_cb[iPlane].sample_stride;
					sl_int32 dst_stride = dst_cb[iPlane].sample_stride;
					sl_uint8* sr = (sl_uint8*)(src_cb[iPlane].data);
					sl_uint8* dr = (sl_uint8*)(dst_cb[iPlane].data);
					for (sl_uint32 i = 0; i < h; i++) {
						sl_uint8* ss = sr;
						sl_uint8* ds = dr;
						for (sl_uint32 j = 0; j < w; j++) {
							*ds = *ss;
							ss+=src_stride;
							ds+=dst_stride;
						}
						sr += src_cb[iPlane].pitch;
						dr += dst_cb[iPlane].pitch;
					}
				}
				
			} else {
				if (BitmapFormats::getColorSpace(dst.format) == ColorSpace::YUV) {
					// yuv420 -> YUV normal
					_BitmapData_copyPixels_YUV420ToYUV(width, height, src, dst.format, dst_planes, dst_pitches);
				} else {
					// yuv420 -> other normal
					_BitmapData_copyPixels_YUV420ToOther(width, height, src, dst.format, dst_planes, dst_pitches);
				}
			}
		} else {
			if (BitmapFormats::isYUV_420(dst.format)) {
				if (BitmapFormats::getColorSpace(src.format) == ColorSpace::YUV) {
					// YUV normal -> yuv420
					_BitmapData_copyPixels_YUVToYUV420(width, height, src.format, src_planes, src_pitches, dst);
				} else {
					// other normal -> yuv420
					_BitmapData_copyPixels_OtherToYUV420(width, height, src.format, src_planes, src_pitches, dst);
				}
			} else {
				// normal -> normal
				if (BitmapFormats::isPrecomputedAlpha(src.format) && BitmapFormats::isPrecomputedAlpha(dst.format)) {
					src.format = BitmapFormats::getNonPrecomputedAlphaFormat(src.format);
					dst.format = BitmapFormats::getNonPrecomputedAlphaFormat(dst.format);
				}
				if (BitmapFormats::getColorSpace(src.format) == ColorSpace::YUV && BitmapFormats::getColorSpace(dst.format) == ColorSpace::YUV) {
					src.format = BitmapFormats::getCompatibleRGBFormat(src.format);
					dst.format = BitmapFormats::getCompatibleRGBFormat(dst.format);
				}
				if (src.format == dst.format) {
					sl_uint32 row_size = (BitmapFormats::getBitsPerSample(src.format) * width) >> 3;
					sl_uint32 nPlanes = BitmapFormats::getPlanesCount(src.format);
					for (sl_uint32 iPlane = 0; iPlane < nPlanes; iPlane++) {
						sl_uint8* sr = (sl_uint8*)(src_planes[iPlane]);
						sl_uint8* dr = (sl_uint8*)(dst_planes[iPlane]);
						for (sl_uint32 i = 0; i < height; i++) {
							sl_uint8* ss = sr;
							sl_uint8* ds = dr;
							for (sl_uint32 j = 0; j < row_size; j++) {
								*ds = *ss;
								ss++;
								ds++;
							}
							sr += src_pitches[iPlane];
							dr += dst_pitches[iPlane];
						}
					}
				} else {
					_BitmapData_copyPixels_Normal(width, height, src.format, src_planes, src_pitches, dst.format, dst_planes, dst_pitches);
				}
			}
		}
	}

	void BitmapData::setFromColors(sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride)
	{
		this->width = width;
		this->height = height;
		this->format = BitmapFormat::RGBA;
		
		this->data = (void*)colors;
		this->pitch = stride * 4;
		if (this->pitch == 0) {
			this->pitch = width * 4;
		}
		
		this->data1 = sl_null;
		this->pitch1 = 0;
		
		this->data2 = sl_null;
		this->pitch2 = 0;
		
		this->data3 = sl_null;
		this->pitch3 = 0;
	}

	BitmapData& BitmapData::operator=(const BitmapData& other) = default;

}
