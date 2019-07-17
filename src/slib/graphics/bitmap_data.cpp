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

#include "slib/graphics/bitmap_data.h"

#include "slib/graphics/yuv.h"
#include "slib/core/compile_optimize.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ColorComponentBuffer)
	
	ColorComponentBuffer::ColorComponentBuffer()
	{
		width = 0;
		height = 0;
		data = sl_null;
		pitch = 0;
		sampleStride = 0;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(BitmapData)

	BitmapData::BitmapData()
	{
		width = 0;
		height = 0;
		format = BitmapFormat::None;
		
		data = sl_null;
		pitch = 0;
		sampleStride = 0;
		
		data1 = sl_null;
		pitch1 = 0;
		sampleStride1 = 0;

		data2 = sl_null;
		pitch2 = 0;
		sampleStride2 = 0;

		data3 = sl_null;
		pitch3 = 0;
		sampleStride3 = 0;
	}

	BitmapData::BitmapData(sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride)
	{
		setFromColors(width, height, colors, stride);
	}

	void*& BitmapData::planeData(sl_uint32 plane)
	{
		switch (plane) {
			case 1:
				return data1;
			case 2:
				return data2;
			case 3:
				return data3;
			default:
				return data;
		}
	}

	void* BitmapData::planeData(sl_uint32 plane) const
	{
		switch (plane) {
			case 1:
				return data1;
			case 2:
				return data2;
			case 3:
				return data3;
			default:
				return data;
		}
	}

	sl_int32& BitmapData::planePitch(sl_uint32 plane)
	{
		switch (plane) {
			case 1:
				return pitch1;
			case 2:
				return pitch2;
			case 3:
				return pitch3;
			default:
				return pitch;
		}
	}

	sl_int32 BitmapData::planePitch(sl_uint32 plane) const
	{
		switch (plane) {
			case 1:
				return pitch1;
			case 2:
				return pitch2;
			case 3:
				return pitch3;
			default:
				return pitch;
		}
	}

	sl_int32& BitmapData::planeSampleStride(sl_uint32 plane)
	{
		switch (plane) {
			case 1:
				return sampleStride1;
			case 2:
				return sampleStride2;
			case 3:
				return sampleStride3;
			default:
				return sampleStride;
		}
	}
	
	sl_int32 BitmapData::planeSampleStride(sl_uint32 plane) const
	{
		switch (plane) {
			case 1:
				return sampleStride1;
			case 2:
				return sampleStride2;
			case 3:
				return sampleStride3;
			default:
				return sampleStride;
		}
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
				if (sampleStride == 0) {
					sampleStride = 1;
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
				if (sampleStride1 == 0) {
					sampleStride1 = 1;
				}
				if (!data2) {
					data2 = (sl_uint8*)(data1) + pitch1 * h2;
				}
				if (pitch2 == 0) {
					pitch2 = pitch1;
				}
				if (sampleStride2 == 0) {
					sampleStride2 = 1;
				}
			} else {
				if (pitch == 0) {
					pitch = width;
				}
				if (sampleStride == 0) {
					sampleStride = 1;
				}
				if (!data1) {
					data1 = (sl_uint8*)(data) + pitch * height;
				}
				if (pitch1 == 0) {
					pitch1 = width;
				}
				if (sampleStride1 == 0) {
					sampleStride1 = 2;
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
				sl_int32& s = planeSampleStride(i);
				if (s == 0) {
					s = BitmapFormats::getBytesPerSample(format);
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
						buffers[i].sampleStride = bd.sampleStride;
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
						buffers[i].sampleStride = bd.sampleStride;
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
						buffers[i].sampleStride = bd.sampleStride;
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
						buffers[i].sampleStride = bd.sampleStride;
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
						buffers[i].sampleStride = bd.sampleStride;
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
						buffers[i].sampleStride = bd.sampleStride;
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
						buffers[i].sampleStride = bd.sampleStride;
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
						buffers[i].sampleStride = bd.sampleStride;
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
					}
					buffers[0].data = bd.data;
					buffers[0].pitch = bd.pitch;
					buffers[0].sampleStride = bd.sampleStride;
					buffers[0].ref = bd.ref;
					buffers[1].data = bd.data1;
					buffers[1].pitch = bd.pitch1;
					buffers[1].sampleStride = bd.sampleStride1;
					buffers[1].ref = bd.ref1;
					buffers[2].data = bd.data2;
					buffers[2].pitch = bd.pitch2;
					buffers[2].sampleStride = bd.sampleStride2;
					buffers[2].ref = bd.ref2;
					buffers[3].data = bd.data3;
					buffers[3].pitch = bd.pitch3;
					buffers[3].sampleStride = bd.sampleStride3;
					buffers[3].ref = bd.ref3;
				}
				return 4;
			case BitmapFormat::RGB_PLANAR:
			case BitmapFormat::YUV444_PLANAR:
				if (buffers) {
					for (int i = 0; i < 3; i++) {
						buffers[i].width = bd.width;
						buffers[i].height = bd.height;
					}
					buffers[0].data = bd.data;
					buffers[0].pitch = bd.pitch;
					buffers[0].sampleStride = bd.sampleStride;
					buffers[0].ref = bd.ref;
					buffers[1].data = bd.data1;
					buffers[1].pitch = bd.pitch1;
					buffers[1].sampleStride = bd.sampleStride1;
					buffers[1].ref = bd.ref1;
					buffers[2].data = bd.data2;
					buffers[2].pitch = bd.pitch2;
					buffers[2].sampleStride = bd.sampleStride2;
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
				buffers[0].sampleStride = bd.sampleStride;
				buffers[0].data = bd.data;
				buffers[0].pitch = bd.pitch;
				buffers[0].ref = bd.ref;
				buffers[1].width = bd.width >> 1;
				buffers[1].height = bd.height >> 1;
				buffers[1].sampleStride = bd.sampleStride1;
				buffers[1].data = bd.data1;
				buffers[1].pitch = bd.pitch1;
				buffers[1].ref = bd.ref1;
				buffers[2].width = bd.width >> 1;
				buffers[2].height = bd.height >> 1;
				buffers[2].sampleStride = bd.sampleStride2;
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
				buffers[0].sampleStride = bd.sampleStride;
				buffers[0].data = bd.data;
				buffers[0].pitch = bd.pitch;
				buffers[0].ref = bd.ref;
				buffers[1].width = bd.width >> 1;
				buffers[1].height = bd.height >> 1;
				buffers[1].sampleStride = bd.sampleStride2;
				buffers[1].data = bd.data2;
				buffers[1].pitch = bd.pitch2;
				buffers[1].ref = bd.ref2;
				buffers[2].width = bd.width >> 1;
				buffers[2].height = bd.height >> 1;
				buffers[2].sampleStride = bd.sampleStride1;
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
				buffers[0].sampleStride = bd.sampleStride;
				buffers[0].data = bd.data;
				buffers[0].pitch = bd.pitch;
				buffers[0].ref = bd.ref;
				buffers[1].width = bd.width >> 1;
				buffers[1].height = bd.height >> 1;
				buffers[1].sampleStride = bd.sampleStride1;
				buffers[1].data = ((sl_uint8*)(bd.data1)) + 1;
				buffers[1].pitch = bd.pitch1;
				buffers[1].ref = bd.ref1;
				buffers[2].width = bd.width >> 1;
				buffers[2].height = bd.height >> 1;
				buffers[2].sampleStride = bd.sampleStride1;
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
				buffers[0].sampleStride = bd.sampleStride;
				buffers[0].data = bd.data;
				buffers[0].pitch = bd.pitch;
				buffers[0].ref = bd.ref;
				buffers[1].width = bd.width >> 1;
				buffers[1].height = bd.height >> 1;
				buffers[1].sampleStride = bd.sampleStride1;
				buffers[1].data = ((sl_uint8*)(bd.data1));
				buffers[1].pitch = bd.pitch1;
				buffers[1].ref = bd.ref1;
				buffers[2].width = bd.width >> 1;
				buffers[2].height = bd.height >> 1;
				buffers[2].sampleStride = bd.sampleStride1;
				buffers[2].data = ((sl_uint8*)(bd.data1)) + 1;
				buffers[2].pitch = bd.pitch1;
				buffers[2].ref = bd.ref1;
				return 3;
			default:
				break;
		}
		return 0;
	}
	
	namespace priv
	{
		namespace bitmap_data
		{
			
#define COLOR_PROC_DEFINE_PARAMS_PLANES1 sl_uint8*& p0, sl_int32 stride0, sl_uint8*& p1, sl_int32 stride1, sl_uint8*& p2, sl_int32 stride2, sl_uint8*& p3, sl_int32 stride3
#define COLOR_READ_PROC_DEFINE_PARAMS1 COLOR_PROC_DEFINE_PARAMS_PLANES, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a
#define COLOR_WRITE_PROC_DEFINE_PARAMS1 COLOR_PROC_DEFINE_PARAMS_PLANES, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a

			class RGBA_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					r = p[0];
					g = p[1];
					b = p[2];
					a = p[3];
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					p[0] = r;
					p[1] = g;
					p[2] = b;
					p[3] = a;
				}
				
			};

			class BGRA_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					b = p[0];
					g = p[1];
					r = p[2];
					a = p[3];
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					p[0] = b;
					p[1] = g;
					p[2] = r;
					p[3] = a;
				}
			};

			class ARGB_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					a = p[0];
					r = p[1];
					g = p[2];
					b = p[3];
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					p[0] = a;
					p[1] = r;
					p[2] = g;
					p[3] = b;
				}
			};

			class ABGR_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					a = p[0];
					b = p[1];
					g = p[2];
					r = p[3];
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					p[0] = a;
					p[1] = b;
					p[2] = g;
					p[3] = r;
				}
			};


			class RGBA_PA_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
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
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
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
				}
			};

			class BGRA_PA_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
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
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
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
				}
			};

			class ARGB_PA_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
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
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
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
				}
			};

			class ABGR_PA_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
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
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
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
				}
			};

			class RGB_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 3;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					r = p[0];
					g = p[1];
					b = p[2];
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					p[0] = r;
					p[1] = g;
					p[2] = b;
				}
			};

			class BGR_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 3;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					b = p[0];
					g = p[1];
					r = p[2];
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					p[0] = b;
					p[1] = g;
					p[2] = r;
				}
			};

			class RGB565BE_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 2;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					sl_uint32 s = p[0];
					s = (s << 8) | p[1];
					r = (sl_uint8)((s & 0xF800) >> 8);
					g = (sl_uint8)((s & 0x07E0) >> 3);
					b = (sl_uint8)((s & 0x001F) << 3);
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					sl_uint32 s = r >> 3;
					s = (s << 5) | (g >> 2);
					s = (s << 6) | (b >> 3);
					p[0] = (sl_uint8)(s >> 8);
					p[1] = (sl_uint8)(s);
				}
			};

			class RGB565LE_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 2;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					sl_uint16 s = p[1];
					s = (s << 8) | p[0];
					r = (sl_uint8)((s & 0xF800) >> 8);
					g = (sl_uint8)((s & 0x07E0) >> 3);
					b = (sl_uint8)((s & 0x001F) << 3);
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					sl_uint32 s = r >> 3;
					s = (s << 5) | (g >> 2);
					s = (s << 6) | (b >> 3);
					p[1] = (sl_uint8)(s >> 8);
					p[0] = (sl_uint8)(s);
				}
			};

			class BGR565BE_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 2;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					sl_uint32 s = p[0];
					s = (s << 8) | p[1];
					b = (sl_uint8)((s & 0xF800) >> 8);
					g = (sl_uint8)((s & 0x07E0) >> 3);
					r = (sl_uint8)((s & 0x001F) << 3);
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					sl_uint32 s = b >> 3;
					s = (s << 5) | (g >> 2);
					s = (s << 6) | (r >> 3);
					p[0] = (sl_uint8)(s >> 8);
					p[1] = (sl_uint8)(s);
				}
			};

			class BGR565LE_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 2;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					sl_uint16 s = p[1];
					s = (s << 8) | p[0];
					b = (sl_uint8)((s & 0xF800) >> 8);
					g = (sl_uint8)((s & 0x07E0) >> 3);
					r = (sl_uint8)((s & 0x001F) << 3);
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					sl_uint32 s = b >> 3;
					s = (s << 5) | (g >> 2);
					s = (s << 6) | (r >> 3);
					p[1] = (sl_uint8)(s >> 8);
					p[0] = (sl_uint8)(s);
				}
			};

			class GRAY8_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 1;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					sl_uint8 v = p[0];
					r = v;
					g = v;
					b = v;
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					sl_uint32 v = r;
					v += g;
					v += b;
					p[0] = (sl_uint8)(v / 3);
				}
			};

			class YUVA_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					YUV::convertYUVToRGB(p[0], p[1], p[2], r, g, b);
					a = p[3];
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					YUV::convertRGBToYUV(r, g, b, p[0], p[1], p[2]);
					p[3] = a;
				}
			};

			class YUVA_PA_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 4;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					Color c;
					YUV::convertYUVToRGB(p[0], p[1], p[2], c.r, c.g, c.b);
					c.a = p[3];
					c.convertPAtoNPA();
					r = c.r;
					g = c.g;
					b = c.b;
					a = c.a;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					Color c;
					c.r = r;
					c.g = g;
					c.b = b;
					c.a = a;
					c.convertNPAtoPA();
					YUV::convertRGBToYUV(c.r, c.g, c.b, p[0], p[1], p[2]);
					p[3] = c.a;
				}
			};

			class YUV444_PROC
			{
			public:
				static constexpr sl_int32 BytesPerSample = 3;
				
				SLIB_INLINE static void readSample(sl_uint8* p, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					YUV::convertYUVToRGB(p[0], p[1], p[2], r, g, b);
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					YUV::convertRGBToYUV(r, g, b, p[0], p[1], p[2]);
				}
			};

			class RGBA_PLANAR_PROC
			{
			public:
				SLIB_INLINE static void readSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					r = *p0;
					g = *p1;
					b = *p2;
					a = *p3;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					*p0 = r;
					*p1 = g;
					*p2 = b;
					*p3 = a;
				}
			};
			
			class RGBA_PLANAR_PA_PROC
			{
			public:
				SLIB_INLINE static void readSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					Color c;
					c.r = *p0;
					c.g = *p1;
					c.b = *p2;
					c.a = *p3;
					c.convertPAtoNPA();
					r = c.r;
					g = c.g;
					b = c.b;
					a = c.a;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					Color c;
					c.r = r;
					c.g = g;
					c.b = b;
					c.a = a;
					c.convertNPAtoPA();
					*p0 = c.r;
					*p1 = c.g;
					*p2 = c.b;
					*p3 = c.a;
				}
			};
			
			class RGB_PLANAR_PROC
			{
			public:
				SLIB_INLINE static void readSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					r = *p0;
					g = *p1;
					b = *p2;
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					*p0 = r;
					*p1 = g;
					*p2 = b;
				}
			};

			class YUVA_PLANAR_PROC
			{
			public:
				SLIB_INLINE static void readSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					YUV::convertYUVToRGB(*p0, *p1, *p2, r, g, b);
					a = *p3;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					YUV::convertRGBToYUV(r, g, b, *p0, *p1, *p2);
					*p3 = a;
				}
			};
			
			class YUVA_PLANAR_PA_PROC
			{
			public:
				SLIB_INLINE static void readSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					Color c;
					YUV::convertYUVToRGB(*p0, *p1, *p2, c.r, c.g, c.b);
					c.a = *p3;
					c.convertPAtoNPA();
					r = c.r;
					g = c.g;
					b = c.b;
					a = c.a;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					Color c;
					c.r = r;
					c.g = g;
					c.b = b;
					c.a = a;
					c.convertNPAtoPA();
					YUV::convertRGBToYUV(c.r, c.g, c.b, *p0, *p1, *p2);
					*p3 = c.a;
				}
			};

			class YUV444_PLANAR_PROC
			{
			public:
				SLIB_INLINE static void readSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8& r, sl_uint8& g, sl_uint8& b, sl_uint8& a)
				{
					YUV::convertYUVToRGB(*p0, *p1, *p2, r, g, b);
					a = 255;
				}
				
				SLIB_INLINE static void writeSample(sl_uint8* p0, sl_uint8* p1, sl_uint8* p2, sl_uint8* p3, sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a)
				{
					YUV::convertRGBToYUV(r, g, b, *p0, *p1, *p2);
				}
			};
	
#define CASES_FOR_NORMAL_RGB(CASE) \
	CASE(RGBA) \
	CASE(RGBA_PA) \
	CASE(BGRA) \
	CASE(BGRA_PA) \
	CASE(ARGB) \
	CASE(ARGB_PA) \
	CASE(ABGR) \
	CASE(ABGR_PA) \
	CASE(RGB) \
	CASE(BGR) \
	CASE(RGB565BE) \
	CASE(RGB565LE) \
	CASE(BGR565BE) \
	CASE(BGR565LE) \
	CASE(GRAY8)

#define CASES_FOR_NORMAL_YUV(CASE) \
	CASE(YUVA) \
	CASE(YUVA_PA) \
	CASE(YUV444)
	
#define CASES_FOR_NORMAL(CASE) \
	CASES_FOR_NORMAL_RGB(CASE) \
	CASES_FOR_NORMAL_YUV(CASE)
	
#define CASES_FOR_PLANAR_RGB(CASE) \
	CASE(RGBA_PLANAR) \
	CASE(RGBA_PLANAR_PA) \
	CASE(RGB_PLANAR)

#define CASES_FOR_PLANAR_YUV(CASE) \
	CASE(YUVA_PLANAR) \
	CASE(YUVA_PLANAR_PA) \
	CASE(YUV444_PLANAR)
	
#define CASES_FOR_PLANAR(CASE) \
	CASES_FOR_PLANAR_RGB(CASE) \
	CASES_FOR_PLANAR_YUV(CASE)

			template<class SourceProc, class TargetProc>
			static void CopyPixels_Normal_Step2(sl_uint32 width, sl_uint32 height, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				sl_uint8* src_row = src;
				sl_uint8* dst_row = dst;
				sl_uint8 c0, c1, c2, c3;
				
#define __SUB(SRC_SAMPLE_STRIDE, DST_SAMPLE_STRIDE) \
				for (sl_uint32 i = 0; i < height; i++) { \
					sl_uint8* s= src_row; \
					sl_uint8* d = dst_row; \
					for (sl_uint32 j = 0; j < width; j++) { \
						SourceProc::readSample(s, c0, c1, c2, c3); \
						TargetProc::writeSample(d, c0, c1, c2, c3); \
						s += SRC_SAMPLE_STRIDE; \
						d += DST_SAMPLE_STRIDE; \
					} \
					src_row += src_pitch; \
					dst_row += dst_pitch; \
				}
				
				if (src_sample_stride == SourceProc::BytesPerSample) {
					if (dst_sample_stride == TargetProc::BytesPerSample) {
						__SUB(SourceProc::BytesPerSample, TargetProc::BytesPerSample)
					} else {
						__SUB(SourceProc::BytesPerSample, dst_sample_stride)
					}
				} else {
					if (dst_sample_stride == TargetProc::BytesPerSample) {
						__SUB(src_sample_stride, TargetProc::BytesPerSample)
					} else {
						__SUB(src_sample_stride, dst_sample_stride)
					}
				}
#undef __SUB
			}

			template<class SourceProc>
			static void CopyPixels_Normal_Step1(sl_uint32 width, sl_uint32 height, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, BitmapFormat dst_format, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				switch (dst_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_Normal_Step2<SourceProc, FORMAT##_PROC>(width, height, src, src_pitch, src_sample_stride, dst, dst_pitch, dst_sample_stride); \
						break;
					CASES_FOR_NORMAL(__CASE)
					default:
						break;
#undef __CASE
				}
			}

			static void CopyPixels_Normal(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, BitmapFormat dst_format, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				switch (src_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_Normal_Step1<FORMAT##_PROC>(width, height, src, src_pitch, src_sample_stride, dst_format, dst, dst_pitch, dst_sample_stride); \
						break;
					CASES_FOR_NORMAL(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			SLIB_INLINE static sl_bool IsPackedPlanar(sl_uint8** planes, sl_int32* sample_strides)
			{
				return sample_strides[0] == 1 && sample_strides[1] == 1 && sample_strides[2] == 1 && (!(planes[3]) || sample_strides[3] == 1);
			}

			template<class SourceProc, class TargetProc>
			static void CopyPixels_Planar_Step2(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				sl_uint8* src_row0 = src_planes[0];
				sl_uint8* src_row1 = src_planes[1];
				sl_uint8* src_row2 = src_planes[2];
				sl_uint8* src_row3 = src_planes[3];
				sl_uint8* dst_row0 = dst_planes[0];
				sl_uint8* dst_row1 = dst_planes[1];
				sl_uint8* dst_row2 = dst_planes[2];
				sl_uint8* dst_row3 = dst_planes[3];
				sl_uint8 c0, c1, c2, c3;
				sl_uint8 *s0, *s1, *s2, *s3, *d0, *d1, *d2, *d3;
		
#define __SUB(S0, S1, S2, S3, D0, D1, D2, D3) \
				for (sl_uint32 i = 0; i < height; i++) { \
					s0 = src_row0; s1 = src_row1; s2 = src_row2; s3 = src_row3; \
					d0 = dst_row0; d1 = dst_row1; d2 = dst_row2; d3 = dst_row3; \
					for (sl_uint32 j = 0; j < width; j++) { \
						SourceProc::readSample(s0, s1, s2, s3, c0, c1, c2, c3); \
						TargetProc::writeSample(d0, d1, d2, d3, c0, c1, c2, c3); \
						s0 += S0; s1 += S1; s2 += S2; s3 += S3; \
						d0 += D0; d1 += D1; d2 += D2; d3 += D3; \
					} \
					src_row0 += src_pitches[0]; src_row1 += src_pitches[1]; src_row2 += src_pitches[2]; src_row3 += src_pitches[3]; \
					dst_row0 += dst_pitches[0]; dst_row1 += dst_pitches[1]; dst_row2 += dst_pitches[2]; dst_row3 += dst_pitches[3]; \
				}
				
				if (IsPackedPlanar(src_planes, src_sample_strides)) {
					if (IsPackedPlanar(dst_planes, dst_sample_strides)) {
						__SUB(1, 1, 1, 1, 1, 1, 1, 1)
					} else {
						__SUB(1, 1, 1, 1, dst_sample_strides[0], dst_sample_strides[1], dst_sample_strides[2], dst_sample_strides[3])
					}
				} else {
					if (IsPackedPlanar(dst_planes, dst_sample_strides)) {
						__SUB(src_sample_strides[0], src_sample_strides[1], src_sample_strides[2], src_sample_strides[3], 1, 1, 1, 1)
					} else {
						__SUB(src_sample_strides[0], src_sample_strides[1], src_sample_strides[2], src_sample_strides[3], dst_sample_strides[0], dst_sample_strides[1], dst_sample_strides[2], dst_sample_strides[3])
					}
				}
#undef __SUB
			}

			template<class SourceProc>
			static void CopyPixels_Planar_Step1(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				switch (dst_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_Planar_Step2<SourceProc, FORMAT##_PROC>(width, height, src_planes, src_pitches, src_sample_strides, dst_planes, dst_pitches, dst_sample_strides); \
						break;
					CASES_FOR_PLANAR(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			static void CopyPixels_Planar(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				switch (src_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_Planar_Step1<FORMAT##_PROC>(width, height, src_planes, src_pitches, src_sample_strides, dst_format, dst_planes, dst_pitches, dst_sample_strides); \
						break;
					CASES_FOR_PLANAR(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			template<class SourceProc, class TargetProc>
			static void CopyPixels_NormalToPlanar_Step2(sl_uint32 width, sl_uint32 height, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				sl_uint8* src_row = src;
				sl_uint8* dst_row0 = dst_planes[0];
				sl_uint8* dst_row1 = dst_planes[1];
				sl_uint8* dst_row2 = dst_planes[2];
				sl_uint8* dst_row3 = dst_planes[3];
				sl_uint8 c0, c1, c2, c3;
				sl_uint8 *d0, *d1, *d2, *d3;
		
#define __SUB(S, D0, D1, D2, D3) \
				for (sl_uint32 i = 0; i < height; i++) { \
					sl_uint8* s = src_row; \
					d0 = dst_row0; d1 = dst_row1; d2 = dst_row2; d3 = dst_row3; \
					for (sl_uint32 j = 0; j < width; j++) { \
						SourceProc::readSample(s, c0, c1, c2, c3); \
						TargetProc::writeSample(d0, d1, d2, d3, c0, c1, c2, c3); \
						s += S; \
						d0 += D0; d1 += D1; d2 += D2; d3 += D3; \
					} \
					src_row += src_pitch; \
					dst_row0 += dst_pitches[0]; dst_row1 += dst_pitches[1]; dst_row2 += dst_pitches[2]; dst_row3 += dst_pitches[3]; \
				}
				
				if (src_sample_stride == SourceProc::BytesPerSample) {
					if (IsPackedPlanar(dst_planes, dst_sample_strides)) {
						__SUB(SourceProc::BytesPerSample, 1, 1, 1, 1)
					} else {
						__SUB(SourceProc::BytesPerSample, dst_sample_strides[0], dst_sample_strides[1], dst_sample_strides[2], dst_sample_strides[3])
					}
				} else {
					if (IsPackedPlanar(dst_planes, dst_sample_strides)) {
						__SUB(src_sample_stride, 1, 1, 1, 1)
					} else {
						__SUB(src_sample_stride, dst_sample_strides[0], dst_sample_strides[1], dst_sample_strides[2], dst_sample_strides[3])
					}
				}
#undef __SUB
			}

			template<class SourceProc>
			static void CopyPixels_NormalToPlanar_Step1(sl_uint32 width, sl_uint32 height, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				switch (dst_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_NormalToPlanar_Step2<SourceProc, FORMAT##_PROC>(width, height, src, src_pitch, src_sample_stride, dst_planes, dst_pitches, dst_sample_strides); \
						break;
					CASES_FOR_PLANAR(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			static void CopyPixels_NormalToPlanar(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				switch (src_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_NormalToPlanar_Step1<FORMAT##_PROC>(width, height, src, src_pitch, src_sample_stride, dst_format, dst_planes, dst_pitches, dst_sample_strides); \
					break;
					CASES_FOR_NORMAL(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			template<class SourceProc, class TargetProc>
			static void CopyPixels_PlanarToNormal_Step2(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				sl_uint8* src_row0 = src_planes[0];
				sl_uint8* src_row1 = src_planes[1];
				sl_uint8* src_row2 = src_planes[2];
				sl_uint8* src_row3 = src_planes[3];
				sl_uint8* dst_row = dst;
				sl_uint8 c0, c1, c2, c3;
				sl_uint8 *s0, *s1, *s2, *s3;
		
#define __SUB(S0, S1, S2, S3, D) \
				for (sl_uint32 i = 0; i < height; i++) { \
					s0 = src_row0; s1 = src_row1; s2 = src_row2; s3 = src_row3; \
					sl_uint8* d = dst_row; \
					for (sl_uint32 j = 0; j < width; j++) { \
						SourceProc::readSample(s0, s1, s2, s3, c0, c1, c2, c3); \
						TargetProc::writeSample(d, c0, c1, c2, c3); \
						s0 += S0; s1 += S1; s2 += S2; s3 += S3; \
						d += D; \
					} \
					src_row0 += src_pitches[0]; src_row1 += src_pitches[1]; src_row2 += src_pitches[2]; src_row3 += src_pitches[3]; \
					dst_row += dst_pitch; \
				}
				
				if (IsPackedPlanar(src_planes, src_sample_strides)) {
					if (dst_sample_stride == TargetProc::BytesPerSample) {
						__SUB(1, 1, 1, 1, TargetProc::BytesPerSample)
					} else {
						__SUB(1, 1, 1, 1, dst_sample_stride)
					}
				} else {
					if (dst_sample_stride == TargetProc::BytesPerSample) {
						__SUB(src_sample_strides[0], src_sample_strides[1], src_sample_strides[2], src_sample_strides[3], TargetProc::BytesPerSample)
					} else {
						__SUB(src_sample_strides[0], src_sample_strides[1], src_sample_strides[2], src_sample_strides[3], dst_sample_stride)
					}
				}
#undef __SUB
			}
	
			template<class SourceProc>
			static void CopyPixels_PlanarToNormal_Step1(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, BitmapFormat dst_format, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				switch (dst_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_PlanarToNormal_Step2<SourceProc, FORMAT##_PROC>(width, height, src_planes, src_pitches, src_sample_strides, dst, dst_pitch, dst_sample_stride); \
						break;
					CASES_FOR_NORMAL(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			static void CopyPixels_PlanarToNormal(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, BitmapFormat dst_format, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				switch (src_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_PlanarToNormal_Step1<FORMAT##_PROC>(width, height, src_planes, src_pitches, src_sample_strides, dst_format, dst, dst_pitch, dst_sample_stride); \
						break;
					CASES_FOR_PLANAR(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			template<class TargetProc>
			static void CopyPixels_YUV420ToYUVNormal_Step1(sl_uint32 width, sl_uint32 height, BitmapData& src, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				ColorComponentBuffer components[3];
				if (src.getColorComponentBuffers(components) != 3) {
					return;
				}
				sl_uint32 W2 = width >> 1;
				sl_uint32 H2 = height >> 1;
				sl_uint8* row_y = (sl_uint8*)(components[0].data);
				sl_uint8* row_u = (sl_uint8*)(components[1].data);
				sl_uint8* row_v = (sl_uint8*)(components[2].data);
				sl_uint8* dst_row = dst;
		
#define __SUB(D, Y, U, V) \
				for (sl_uint32 i = 0; i < H2; i++) { \
					sl_uint8* y0 = row_y; \
					sl_uint8* y1 = y0 + components[0].pitch; \
					sl_uint8* u = row_u; \
					sl_uint8* v = row_v; \
					sl_uint8* d0 = dst_row; \
					sl_uint8* d1 = d0 + dst_pitch; \
					for (sl_uint32 j = 0; j < W2; j++) { \
						TargetProc::writeSample(d0, *y0, *u, *v, 255); \
						y0 += Y; d0 += D; \
						TargetProc::writeSample(d0, *y0, *u, *v, 255); \
						y0 += Y; d0 += D; \
						TargetProc::writeSample(d1, *y1, *u, *v, 255); \
						y1 += Y; d1 += D; \
						TargetProc::writeSample(d1, *y1, *u, *v, 255); \
						y1 += Y; d1 += D; \
						u += U; v += V; \
					} \
					row_y += components[0].pitch + components[0].pitch; \
					row_u += components[1].pitch; \
					row_v += components[2].pitch; \
					dst_row += dst_pitch + dst_pitch; \
				}
				
				if (components[0].sampleStride == 1) {
					if (components[1].sampleStride == 1 && components[2].sampleStride == 1) {
						if (dst_sample_stride == TargetProc::BytesPerSample) {
							__SUB(TargetProc::BytesPerSample, 1, 1, 1)
						} else {
							__SUB(dst_sample_stride, 1, 1, 1)
						}
						return;
					} else if (components[1].sampleStride == 2 && components[2].sampleStride == 2) {
						if (dst_sample_stride == TargetProc::BytesPerSample) {
							__SUB(TargetProc::BytesPerSample, 1, 2, 2)
						} else {
							__SUB(dst_sample_stride, 1, 2, 2)
						}
						return;
					}
				}
				{
					__SUB(dst_sample_stride, components[0].sampleStride, components[1].sampleStride, components[2].sampleStride)
				}
		
#undef __SUB
			}

			static void CopyPixels_YUV420ToYUVNormal(sl_uint32 width, sl_uint32 height, BitmapData& src, BitmapFormat dst_format, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				switch (dst_format) {
					case BitmapFormat::YUVA:
						CopyPixels_YUV420ToYUVNormal_Step1<RGBA_PROC>(width, height, src, dst, dst_pitch, dst_sample_stride);
						break;
					case BitmapFormat::YUVA_PA:
						CopyPixels_YUV420ToYUVNormal_Step1<RGBA_PA_PROC>(width, height, src, dst, dst_pitch, dst_sample_stride);
						break;
					case BitmapFormat::YUV444:
						CopyPixels_YUV420ToYUVNormal_Step1<RGB_PROC>(width, height, src, dst, dst_pitch, dst_sample_stride);
						break;
					default:
						break;
				}
			}
			
			template<class TargetProc>
			static void CopyPixels_YUV420ToYUVPlanar_Step1(sl_uint32 width, sl_uint32 height, BitmapData& src, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				ColorComponentBuffer components[3];
				if (src.getColorComponentBuffers(components) != 3) {
					return;
				}
				sl_uint32 W2 = width >> 1;
				sl_uint32 H2 = height >> 1;
				sl_uint8* row_y = (sl_uint8*)(components[0].data);
				sl_uint8* row_u = (sl_uint8*)(components[1].data);
				sl_uint8* row_v = (sl_uint8*)(components[2].data);
				sl_uint8* dst_row0 = dst_planes[0];
				sl_uint8* dst_row1 = dst_planes[1];
				sl_uint8* dst_row2 = dst_planes[2];
				sl_uint8* dst_row3 = dst_planes[3];

#define __SUB(D0, D1, D2, D3, Y, U, V) \
				for (sl_uint32 i = 0; i < H2; i++) { \
					sl_uint8* y0 = row_y; \
					sl_uint8* y1 = y0 + components[0].pitch; \
					sl_uint8* u = row_u; \
					sl_uint8* v = row_v; \
					sl_uint8* d0u = dst_row0; sl_uint8* d1u = dst_row1; sl_uint8* d2u = dst_row2; sl_uint8* d3u = dst_row3; \
					sl_uint8* d0d = d0u + dst_pitches[0]; sl_uint8* d1d = d1u + dst_pitches[1]; sl_uint8* d2d = d2u + dst_pitches[2]; sl_uint8* d3d = d3u + dst_pitches[3]; \
					for (sl_uint32 j = 0; j < W2; j++) { \
						TargetProc::writeSample(d0u, d1u, d2u, d3u, *y0, *u, *v, 255); \
						y0 += Y; d0u += D0; d1u += D1; d2u += D2; d3u += D3; \
						TargetProc::writeSample(d0u, d1u, d2u, d3u, *y0, *u, *v, 255); \
						y0 += Y; d0u += D0; d1u += D1; d2u += D2; d3u += D3; \
						TargetProc::writeSample(d0d, d1d, d2d, d3d, *y1, *u, *v, 255); \
						y1 += Y; d0d += D0; d1d += D1; d2d += D2; d3d += D3; \
						TargetProc::writeSample(d0d, d1d, d2d, d3d, *y1, *u, *v, 255); \
						y1 += Y; d0d += D0; d1d += D1; d2d += D2; d3d += D3; \
						u += U; v += V; \
					} \
					row_y += components[0].pitch + components[0].pitch; \
					row_u += components[1].pitch; \
					row_v += components[2].pitch; \
					dst_row0 += dst_pitches[0] + dst_pitches[0]; \
					dst_row1 += dst_pitches[1] + dst_pitches[1]; \
					dst_row2 += dst_pitches[2] + dst_pitches[2]; \
					dst_row3 += dst_pitches[3] + dst_pitches[3]; \
				}
				
				if (components[0].sampleStride == 1) {
					if (components[1].sampleStride == 1 && components[2].sampleStride == 1) {
						if (IsPackedPlanar(dst_planes, dst_sample_strides)) {
							__SUB(1, 1, 1, 1, 1, 1, 1)
						} else {
							__SUB(dst_sample_strides[0], dst_sample_strides[1], dst_sample_strides[2], dst_sample_strides[3], 1, 1, 1)
						}
						return;
					} else if (components[1].sampleStride == 2 && components[2].sampleStride == 2) {
						if (IsPackedPlanar(dst_planes, dst_sample_strides)) {
							__SUB(1, 1, 1, 1, 1, 2, 2)
						} else {
							__SUB(dst_sample_strides[0], dst_sample_strides[1], dst_sample_strides[2], dst_sample_strides[3], 1, 2, 2)
						}
						return;
					}
				}
				{
					__SUB(dst_sample_strides[0], dst_sample_strides[1], dst_sample_strides[2], dst_sample_strides[3], components[0].sampleStride, components[1].sampleStride, components[2].sampleStride)
				}
		
#undef __SUB
			}

			
			static void CopyPixels_YUV420ToYUVPlanar(sl_uint32 width, sl_uint32 height, BitmapData& src, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				switch (dst_format) {
					case BitmapFormat::YUVA_PLANAR:
						CopyPixels_YUV420ToYUVPlanar_Step1<RGBA_PLANAR_PROC>(width, height, src, dst_planes, dst_pitches, dst_sample_strides);
						break;
					case BitmapFormat::YUVA_PLANAR_PA:
						CopyPixels_YUV420ToYUVPlanar_Step1<RGBA_PLANAR_PA_PROC>(width, height, src, dst_planes, dst_pitches, dst_sample_strides);
						break;
					case BitmapFormat::YUV444_PLANAR:
						CopyPixels_YUV420ToYUVPlanar_Step1<RGB_PLANAR_PROC>(width, height, src, dst_planes, dst_pitches, dst_sample_strides);
						break;
					default:
						break;
				}
			}

			template<class TargetProc>
			static void CopyPixels_YUV420ToOtherNormal_Step1(sl_uint32 width, sl_uint32 height, BitmapData& src, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				ColorComponentBuffer components[3];
				if (src.getColorComponentBuffers(components) != 3) {
					return;
				}
				sl_uint32 W2 = width >> 1;
				sl_uint32 H2 = height >> 1;
				sl_uint8* row_y = (sl_uint8*)(components[0].data);
				sl_uint8* row_u = (sl_uint8*)(components[1].data);
				sl_uint8* row_v = (sl_uint8*)(components[2].data);
				sl_uint8* dst_row = dst;
				sl_uint8 r, g, b;

				for (sl_uint32 i = 0; i < H2; i++) {
					sl_uint8* y0 = row_y;
					sl_uint8* y1 = y0 + components[0].pitch;
					sl_uint8* u = row_u;
					sl_uint8* v = row_v;
					sl_uint8* d0 = dst_row;
					sl_uint8* d1 = d0 + dst_pitch;
					for (sl_uint32 j = 0; j < W2; j++) {
						YUV::convertYUVToRGB(*y0, *u, *v, r, g, b);
						TargetProc::writeSample(d0, r, g, b, 255);
						y0 += components[0].sampleStride; d0 += dst_pitch;
						YUV::convertYUVToRGB(*y0, *u, *v, r, g, b);
						TargetProc::writeSample(d0, r, g, b, 255);
						y0 += components[0].sampleStride; d0 += dst_pitch;
						YUV::convertYUVToRGB(*y1, *u, *v, r, g, b);
						TargetProc::writeSample(d1, r, g, b, 255);
						y1 += components[0].sampleStride; d1 += dst_pitch;
						YUV::convertYUVToRGB(*y1, *u, *v, r, g, b);
						TargetProc::writeSample(d1, r, g, b, 255);
						y1 += components[0].sampleStride; d1 += dst_pitch;
						u += components[1].sampleStride; v += components[2].sampleStride;
					}
					row_y += components[0].pitch + components[0].pitch;
					row_u += components[1].pitch;
					row_v += components[2].pitch;
					dst_row += dst_pitch + dst_pitch;
				}
			}

			static void CopyPixels_YUV420ToOtherNormal(sl_uint32 width, sl_uint32 height, BitmapData& src, BitmapFormat dst_format, sl_uint8* dst, sl_int32 dst_pitch, sl_int32 dst_sample_stride)
			{
				switch (dst_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_YUV420ToOtherNormal_Step1<FORMAT##_PROC>(width, height, src, dst, dst_pitch, dst_sample_stride); \
						break;
					CASES_FOR_NORMAL_RGB(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			template<class TargetProc>
			static void CopyPixels_YUV420ToOtherPlanar_Step1(sl_uint32 width, sl_uint32 height, BitmapData& src, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				ColorComponentBuffer components[3];
				if (src.getColorComponentBuffers(components) != 3) {
					return;
				}
				sl_uint32 W2 = width >> 1;
				sl_uint32 H2 = height >> 1;
				sl_uint8* row_y = (sl_uint8*)(components[0].data);
				sl_uint8* row_u = (sl_uint8*)(components[1].data);
				sl_uint8* row_v = (sl_uint8*)(components[2].data);
				sl_uint8* dst_row0 = dst_planes[0];
				sl_uint8* dst_row1 = dst_planes[1];
				sl_uint8* dst_row2 = dst_planes[2];
				sl_uint8* dst_row3 = dst_planes[3];
				sl_uint8 r, g, b;

				for (sl_uint32 i = 0; i < H2; i++) {
					sl_uint8* y0 = row_y;
					sl_uint8* y1 = y0 + components[0].pitch;
					sl_uint8* u = row_u;
					sl_uint8* v = row_v;
					sl_uint8* d0u = dst_row0; sl_uint8* d1u = dst_row1; sl_uint8* d2u = dst_row2; sl_uint8* d3u = dst_row3;
					sl_uint8* d0d = d0u + dst_pitches[0]; sl_uint8* d1d = d1u + dst_pitches[1]; sl_uint8* d2d = d2u + dst_pitches[2]; sl_uint8* d3d = d3u + dst_pitches[3];
					for (sl_uint32 j = 0; j < W2; j++) {
						YUV::convertYUVToRGB(*y0, *u, *v, r, g, b);
						TargetProc::writeSample(d0u, d1u, d2u, d3u, r, g, b, 255);
						y0 += components[0].sampleStride; d0u += dst_sample_strides[0]; d1u += dst_sample_strides[1]; d2u += dst_sample_strides[2]; d3u += dst_sample_strides[3];
						YUV::convertYUVToRGB(*y0, *u, *v, r, g, b);
						TargetProc::writeSample(d0u, d1u, d2u, d3u, r, g, b, 255);
						y0 += components[0].sampleStride; d0u += dst_sample_strides[0]; d1u += dst_sample_strides[1]; d2u += dst_sample_strides[2]; d3u += dst_sample_strides[3];
						YUV::convertYUVToRGB(*y1, *u, *v, r, g, b);
						TargetProc::writeSample(d0d, d1d, d2d, d3d, r, g, b, 255);
						y1 += components[0].sampleStride; d0d += dst_sample_strides[0]; d1d += dst_sample_strides[1]; d2d += dst_sample_strides[2]; d3d += dst_sample_strides[3];
						YUV::convertYUVToRGB(*y1, *u, *v, r, g, b);
						TargetProc::writeSample(d0d, d1d, d2d, d3d, r, g, b, 255);
						y1 += components[0].sampleStride; d0d += dst_sample_strides[0]; d1d += dst_sample_strides[1]; d2d += dst_sample_strides[2]; d3d += dst_sample_strides[3];
						u += components[1].sampleStride; v += components[2].sampleStride;
					}
					row_y += components[0].pitch + components[0].pitch;
					row_u += components[1].pitch;
					row_v += components[2].pitch;
					dst_row0 += dst_pitches[0] + dst_pitches[0];
					dst_row1 += dst_pitches[1] + dst_pitches[1];
					dst_row2 += dst_pitches[2] + dst_pitches[2];
					dst_row3 += dst_pitches[3] + dst_pitches[3];
				}
			}

			static void CopyPixels_YUV420ToOtherPlanar(sl_uint32 width, sl_uint32 height, BitmapData& src, BitmapFormat dst_format, sl_uint8** dst_planes, sl_int32* dst_pitches, sl_int32* dst_sample_strides)
			{
				switch (dst_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_YUV420ToOtherPlanar_Step1<FORMAT##_PROC>(width, height, src, dst_planes, dst_pitches, dst_sample_strides); \
						break;
					CASES_FOR_PLANAR_RGB(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
			template<class SourceProc>
			static void CopyPixels_YUVNormalToYUV420_Step1(sl_uint32 width, sl_uint32 height, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, BitmapData& dst)
			{
				ColorComponentBuffer components[3];
				if (dst.getColorComponentBuffers(components) != 3) {
					return;
				}
				sl_uint32 W2 = width >> 1;
				sl_uint32 H2 = height >> 1;
				sl_uint8* row_y = (sl_uint8*)(components[0].data);
				sl_uint8* row_u = (sl_uint8*)(components[1].data);
				sl_uint8* row_v = (sl_uint8*)(components[2].data);
				sl_uint8* src_row = src;
				sl_uint8 A, U, V;
				sl_uint32 TU, TV;
		
#define __SUB(S, DY, DU, DV) \
				for (sl_uint32 i = 0; i < H2; i++) { \
					sl_uint8* y0 = row_y; \
					sl_uint8* y1 = y0 + components[0].pitch; \
					sl_uint8* u = row_u; \
					sl_uint8* v = row_v; \
					sl_uint8* s0 = src_row; \
					sl_uint8* s1 = s0 + src_pitch; \
					for (sl_uint32 j = 0; j < W2; j++) { \
						SourceProc::readSample(s0, *y0, U, V, A); \
						s0 += S; y0 += DY; \
						TU = U; TV = V; \
						SourceProc::readSample(s0, *y0, U, V, A); \
						s0 += S; y0 += DY; \
						TU += U; TV += V; \
						SourceProc::readSample(s0, *y1, U, V, A); \
						s1 += S; y1 += DY; \
						TU += U; TV += V; \
						SourceProc::readSample(s0, *y1, U, V, A); \
						s1 += S; y1 += DY; \
						TU += U; TV += V; \
						*u = TU >> 2; *v = TV >> 2; \
						u += DU; v += DV; \
					} \
					row_y += components[0].pitch + components[0].pitch; \
					row_u += components[1].pitch; \
					row_v += components[2].pitch; \
					src_row += src_pitch + src_pitch; \
				}

				if (components[0].sampleStride == 1) {
					if (components[1].sampleStride == 1 && components[2].sampleStride == 1) {
						if (src_sample_stride == SourceProc::BytesPerSample) {
							__SUB(SourceProc::BytesPerSample, 1, 1, 1)
						} else {
							__SUB(src_sample_stride, 1, 1, 1)
						}
						return;
					} else if (components[1].sampleStride == 2 && components[2].sampleStride == 2) {
						if (src_sample_stride == SourceProc::BytesPerSample) {
							__SUB(SourceProc::BytesPerSample, 1, 2, 2)
						} else {
							__SUB(src_sample_stride, 1, 2, 2)
						}
						return;
					}
				}
				{
					__SUB(src_sample_stride, components[0].sampleStride, components[1].sampleStride, components[2].sampleStride)
				}
		
#undef __SUB
			}
			
			void CopyPixels_YUVNormalToYUV420(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, BitmapData& dst)
			{
				switch (src_format) {
					case BitmapFormat::YUVA:
						CopyPixels_YUVNormalToYUV420_Step1<RGBA_PROC>(width, height, src, src_sample_stride, src_sample_stride, dst);
						break;
					case BitmapFormat::YUVA_PA:
						CopyPixels_YUVNormalToYUV420_Step1<RGBA_PA_PROC>(width, height, src, src_sample_stride, src_sample_stride, dst);
						break;
					case BitmapFormat::YUV444:
						CopyPixels_YUVNormalToYUV420_Step1<RGB_PROC>(width, height, src, src_sample_stride, src_sample_stride, dst);
						break;
					default:
						break;
				}
			}
			
			template<class SourceProc>
			static void CopyPixels_YUVPlanarToYUV420_Step1(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, BitmapData& dst)
			{
				ColorComponentBuffer components[3];
				if (dst.getColorComponentBuffers(components) != 3) {
					return;
				}
				sl_uint32 W2 = width >> 1;
				sl_uint32 H2 = height >> 1;
				sl_uint8* row_y = (sl_uint8*)(components[0].data);
				sl_uint8* row_u = (sl_uint8*)(components[1].data);
				sl_uint8* row_v = (sl_uint8*)(components[2].data);
				sl_uint8* src_row0 = src_planes[0];
				sl_uint8* src_row1 = src_planes[1];
				sl_uint8* src_row2 = src_planes[2];
				sl_uint8* src_row3 = src_planes[3];
				sl_uint8 A, U, V;
				sl_uint32 TU, TV;
		
#define __SUB(S0, S1, S2, S3, DY, DU, DV) \
				for (sl_uint32 i = 0; i < H2; i++) { \
					sl_uint8* y0 = row_y; \
					sl_uint8* y1 = y0 + components[0].pitch; \
					sl_uint8* u = row_u; \
					sl_uint8* v = row_v; \
					sl_uint8* s0u = src_row0; sl_uint8* s1u = src_row1; sl_uint8* s2u = src_row2; sl_uint8* s3u = src_row3; \
					sl_uint8* s0d = s0u + src_pitches[0]; sl_uint8* s1d = s1u + src_pitches[1]; sl_uint8* s2d = s2u + src_pitches[2]; sl_uint8* s3d = s3u + src_pitches[3]; \
					for (sl_uint32 j = 0; j < W2; j++) { \
						SourceProc::readSample(s0u, s1u, s2u, s3u, *y0, U, V, A); \
						s0u += S0; s1u += S1; s2u += S2; s3u += S3; y0 += DY; \
						TU = U; TV = V; \
						SourceProc::readSample(s0u, s1u, s2u, s3u, *y0, U, V, A); \
						s0u += S0; s1u += S1; s2u += S2; s3u += S3; y0 += DY; \
						TU += U; TV += V; \
						SourceProc::readSample(s0d, s1d, s2d, s3d, *y1, U, V, A); \
						s0d += S0; s1d += S1; s2d += S2; s3d += S3; y1 += DY; \
						TU += U; TV += V; \
						SourceProc::readSample(s0d, s1d, s2d, s3d, *y1, U, V, A); \
						s0d += S0; s1d += S1; s2d += S2; s3d += S3; y1 += DY; \
						TU += U; TV += V; \
						*u = TU >> 2; *v = TV >> 2; \
						u += DU; v += DV; \
					} \
					row_y += components[0].pitch + components[0].pitch; \
					row_u += components[1].pitch; \
					row_v += components[2].pitch; \
					src_row0 += src_pitches[0] + src_pitches[0]; \
					src_row1 += src_pitches[1] + src_pitches[1]; \
					src_row2 += src_pitches[2] + src_pitches[2]; \
					src_row3 += src_pitches[3] + src_pitches[3]; \
				}
				
				if (components[0].sampleStride == 1) {
					if (components[1].sampleStride == 1 && components[2].sampleStride == 1) {
						if (IsPackedPlanar(src_planes, src_sample_strides)) {
							__SUB(1, 1, 1, 1, 1, 1, 1)
						} else {
							__SUB(src_sample_strides[0], src_sample_strides[1], src_sample_strides[2], src_sample_strides[3], 1, 1, 1)
						}
						return;
					} else if (components[1].sampleStride == 2 && components[2].sampleStride == 2) {
						if (IsPackedPlanar(src_planes, src_sample_strides)) {
							__SUB(1, 1, 1, 1, 1, 2, 2)
						} else {
							__SUB(src_sample_strides[0], src_sample_strides[1], src_sample_strides[2], src_sample_strides[3], 1, 2, 2)
						}
						return;
					}
				}
				{
					__SUB(src_sample_strides[0], src_sample_strides[1], src_sample_strides[2], src_sample_strides[3], components[0].sampleStride, components[1].sampleStride, components[2].sampleStride)
				}
		
#undef __SUB
			}
			
			void CopyPixels_YUVPlanarToYUV420(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, BitmapData& dst)
			{
				switch (src_format) {
					case BitmapFormat::YUVA_PLANAR:
						CopyPixels_YUVPlanarToYUV420_Step1<RGBA_PLANAR_PROC>(width, height, src_planes, src_pitches, src_sample_strides, dst);
						break;
					case BitmapFormat::YUVA_PLANAR_PA:
						CopyPixels_YUVPlanarToYUV420_Step1<RGBA_PLANAR_PA_PROC>(width, height, src_planes, src_pitches, src_sample_strides, dst);
						break;
					case BitmapFormat::YUV444_PLANAR:
						CopyPixels_YUVPlanarToYUV420_Step1<RGB_PLANAR_PROC>(width, height, src_planes, src_pitches, src_sample_strides, dst);
						break;
					default:
						break;
				}
			}

			template<class SourceProc>
			static void CopyPixels_OtherNormalToYUV420_Step1(sl_uint32 width, sl_uint32 height, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, BitmapData& dst)
			{
				ColorComponentBuffer components[3];
				if (dst.getColorComponentBuffers(components) != 3) {
					return;
				}
				sl_uint32 W2 = width >> 1;
				sl_uint32 H2 = height >> 1;
				sl_uint8* row_y = (sl_uint8*)(components[0].data);
				sl_uint8* row_u = (sl_uint8*)(components[1].data);
				sl_uint8* row_v = (sl_uint8*)(components[2].data);
				sl_uint8* src_row = src;
				sl_uint8 R, G, B, A, U, V;
				sl_uint32 TU, TV;
				for (sl_uint32 i = 0; i < H2; i++) {
					sl_uint8* y0 = row_y;
					sl_uint8* y1 = y0 + components[0].pitch;
					sl_uint8* u = row_u;
					sl_uint8* v = row_v;
					sl_uint8* s0 = src_row;
					sl_uint8* s1 = s0 + src_pitch;
					for (sl_uint32 j = 0; j < W2; j++) {
						SourceProc::readSample(s0, R, G, B, A);
						YUV::convertRGBToYUV(R, G, B, *y0, U, V);
						s0 += src_sample_stride; y0 += components[0].sampleStride;
						TU = U; TV = V;
						SourceProc::readSample(s0, R, G, B, A);
						YUV::convertRGBToYUV(R, G, B, *y0, U, V);
						s0 += src_sample_stride; y0 += components[0].sampleStride;
						TU += U; TV += V;
						SourceProc::readSample(s1, R, G, B, A);
						YUV::convertRGBToYUV(R, G, B, *y1, U, V);
						s1 += src_sample_stride; y1 += components[0].sampleStride;
						TU += U; TV += V;
						SourceProc::readSample(s1, R, G, B, A);
						YUV::convertRGBToYUV(R, G, B, *y1, U, V);
						s1 += src_sample_stride; y1 += components[0].sampleStride;
						TU += U; TV += V;
						*u = TU >> 2; *v = TV >> 2;
						u += components[1].sampleStride; v += components[2].sampleStride;
					}
					row_y += components[0].pitch + components[0].pitch;
					row_u += components[1].pitch;
					row_v += components[2].pitch;
					src_row += src_pitch + src_pitch;
				}
			}

			void CopyPixels_OtherNormalToYUV420(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8* src, sl_int32 src_pitch, sl_int32 src_sample_stride, BitmapData& dst)
			{
				switch (src_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_OtherNormalToYUV420_Step1<FORMAT##_PROC>(width, height, src, src_pitch, src_sample_stride, dst); \
						break;
					CASES_FOR_NORMAL_RGB(__CASE)
					default:
						break;
				}
#undef __CASE
			}

			template<class SourceProc>
			static void CopyPixels_OtherPlanarToYUV420_Step1(sl_uint32 width, sl_uint32 height, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, BitmapData& dst)
			{
				ColorComponentBuffer components[3];
				if (dst.getColorComponentBuffers(components) != 3) {
					return;
				}
				sl_uint32 W2 = width >> 1;
				sl_uint32 H2 = height >> 1;
				sl_uint8* row_y = (sl_uint8*)(components[0].data);
				sl_uint8* row_u = (sl_uint8*)(components[1].data);
				sl_uint8* row_v = (sl_uint8*)(components[2].data);
				sl_uint8* src_row0 = src_planes[0];
				sl_uint8* src_row1 = src_planes[1];
				sl_uint8* src_row2 = src_planes[2];
				sl_uint8* src_row3 = src_planes[3];
				sl_uint8 R, G, B, A, U, V;
				sl_uint32 TU, TV;
				for (sl_uint32 i = 0; i < H2; i++) {
					sl_uint8* y0 = row_y;
					sl_uint8* y1 = y0 + components[0].pitch;
					sl_uint8* u = row_u;
					sl_uint8* v = row_v;
					sl_uint8* s0u = src_row0; sl_uint8* s1u = src_row1; sl_uint8* s2u = src_row2; sl_uint8* s3u = src_row3;
					sl_uint8* s0d = s0u + src_pitches[0]; sl_uint8* s1d = s1u + src_pitches[1]; sl_uint8* s2d = s2u + src_pitches[2]; sl_uint8* s3d = s3u + src_pitches[3];
					for (sl_uint32 j = 0; j < W2; j++) {
						SourceProc::readSample(s0u, s1u, s2u, s3u, R, G, B, A);
						YUV::convertRGBToYUV(R, G, B, *y0, U, V);
						s0u += src_sample_strides[0]; s1u += src_sample_strides[1]; s2u += src_sample_strides[2]; s3u += src_sample_strides[3]; y0 += components[0].sampleStride;
						TU = U; TV = V;
						SourceProc::readSample(s0u, s1u, s2u, s3u, R, G, B, A);
						YUV::convertRGBToYUV(R, G, B, *y0, U, V);
						s0u += src_sample_strides[0]; s1u += src_sample_strides[1]; s2u += src_sample_strides[2]; s3u += src_sample_strides[3]; y0 += components[0].sampleStride;
						TU += U; TV += V;
						SourceProc::readSample(s0d, s1d, s2d, s3d, R, G, B, A);
						YUV::convertRGBToYUV(R, G, B, *y1, U, V);
						s0d += src_sample_strides[0]; s1d += src_sample_strides[1]; s2d += src_sample_strides[2]; s3d += src_sample_strides[3]; y1 += components[0].sampleStride;
						TU += U; TV += V;
						SourceProc::readSample(s0d, s1d, s2d, s3d, R, G, B, A);
						YUV::convertRGBToYUV(R, G, B, *y1, U, V);
						s0d += src_sample_strides[0]; s1d += src_sample_strides[1]; s2d += src_sample_strides[2]; s3d += src_sample_strides[3]; y1 += components[0].sampleStride;
						TU += U; TV += V;
						*u = TU >> 2; *v = TV >> 2;
						u += components[1].sampleStride; v += components[2].sampleStride;
					}
					row_y += components[0].pitch + components[0].pitch;
					row_u += components[1].pitch;
					row_v += components[2].pitch;
					src_row0 += src_pitches[0] + src_pitches[0];
					src_row1 += src_pitches[1] + src_pitches[1];
					src_row2 += src_pitches[2] + src_pitches[2];
					src_row3 += src_pitches[3] + src_pitches[3];
				}
			}
			
			void CopyPixels_OtherPlanarToYUV420(sl_uint32 width, sl_uint32 height, BitmapFormat src_format, sl_uint8** src_planes, sl_int32* src_pitches, sl_int32* src_sample_strides, BitmapData& dst)
			{
				switch (src_format) {
#define __CASE(FORMAT) \
					case BitmapFormat::FORMAT: \
						CopyPixels_OtherPlanarToYUV420_Step1<FORMAT##_PROC>(width, height, src_planes, src_pitches, src_sample_strides, dst); \
						break;
					CASES_FOR_PLANAR_RGB(__CASE)
					default:
						break;
#undef __CASE
				}
			}
			
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
		sl_int32 src_sample_strides[4];
		sl_uint8* dst_planes[4];
		sl_int32 dst_pitches[4];
		sl_int32 dst_sample_strides[4];

		for (sl_uint32 i = 0; i < 4; i++) {
			src_planes[i] = (sl_uint8*)(src.planeData(i));
			src_pitches[i] = src.planePitch(i);
			src_sample_strides[i] = src.planeSampleStride(i);
			dst_planes[i] = (sl_uint8*)(dst.planeData(i));
			dst_pitches[i] = dst.planePitch(i);
			dst_sample_strides[i] = dst.planeSampleStride(i);
		}
		
		if (BitmapFormats::isYUV_420(src.format)) {
			
			if (BitmapFormats::isYUV_420(dst.format)) {
				// yuv420 -> yuv420
				ColorComponentBuffer src_comps[3];
				ColorComponentBuffer dst_comps[3];
				if (src.getColorComponentBuffers(src_comps) != 3) {
					return;
				}
				if (dst.getColorComponentBuffers(dst_comps) != 3) {
					return;
				}
				for (sl_uint32 iPlane = 0; iPlane < 3; iPlane++) {
					ColorComponentBuffer& src_comp = src_comps[iPlane];
					ColorComponentBuffer& dst_comp = dst_comps[iPlane];
					sl_uint32 w = src_comp.width;
					sl_uint32 h = dst_comp.height;
					sl_uint8* src_row = (sl_uint8*)(src_comp.data);
					sl_uint8* dst_row = (sl_uint8*)(dst_comp.data);
					sl_int32 src_pitch = src_comp.pitch;
					sl_int32 dst_pitch = dst_comp.pitch;
					sl_int32 src_stride = src_comp.sampleStride;
					sl_int32 dst_stride = dst_comp.sampleStride;
					if (src_stride == 1 && dst_stride == 1) {
						for (sl_uint32 i = 0; i < h; i++) {
							sl_uint8* s = src_row;
							sl_uint8* d = dst_row;
							for (sl_uint32 j = 0; j < w; j++) {
								*d = *s;
								s++; d++;
							}
							src_row += src_pitch;
							dst_row += dst_pitch;
						}
					} else if (src_stride == 2 && dst_stride == 2) {
						for (sl_uint32 i = 0; i < h; i++) {
							sl_uint8* s = src_row;
							sl_uint8* d = dst_row;
							for (sl_uint32 j = 0; j < w; j++) {
								*d = *s;
								s += 2;
								d += 2;
							}
							src_row += src_pitch;
							dst_row += dst_pitch;
						}
					} else {
						for (sl_uint32 i = 0; i < h; i++) {
							sl_uint8* s = src_row;
							sl_uint8* d = dst_row;
							for (sl_uint32 j = 0; j < w; j++) {
								*d = *s;
								s += src_stride;
								d += dst_stride;
							}
							src_row += src_pitch;
							dst_row += dst_pitch;
						}
					}
				}
			} else {
				if (BitmapFormats::getColorSpace(dst.format) == ColorSpace::YUV) {
					// yuv420 -> YUV
					if (BitmapFormats::getPlanesCount(dst.format) == 1) {
						priv::bitmap_data::CopyPixels_YUV420ToYUVNormal(width, height, src, dst.format, dst_planes[0], dst_pitches[0], dst_sample_strides[0]);
					} else {
						priv::bitmap_data::CopyPixels_YUV420ToYUVPlanar(width, height, src, dst.format, dst_planes, dst_pitches, dst_sample_strides);
					}
				} else {
					// yuv420 -> other
					if (BitmapFormats::getPlanesCount(dst.format) == 1) {
						priv::bitmap_data::CopyPixels_YUV420ToOtherNormal(width, height, src, dst.format, dst_planes[0], dst_pitches[0], dst_sample_strides[0]);
					} else {
						priv::bitmap_data::CopyPixels_YUV420ToOtherPlanar(width, height, src, dst.format, dst_planes, dst_pitches, dst_sample_strides);
					}
				}
			}
		} else {
			if (BitmapFormats::isYUV_420(dst.format)) {
				if (BitmapFormats::getColorSpace(src.format) == ColorSpace::YUV) {
					// YUV -> yuv420
					if (BitmapFormats::getPlanesCount(src.format) == 1) {
						priv::bitmap_data::CopyPixels_YUVNormalToYUV420(width, height, src.format, src_planes[0], src_pitches[0], src_sample_strides[0], dst);
					} else {
						priv::bitmap_data::CopyPixels_YUVPlanarToYUV420(width, height, src.format, src_planes, src_pitches, src_sample_strides, dst);
					}
				} else {
					// other -> yuv420
					if (BitmapFormats::getPlanesCount(src.format) == 1) {
						priv::bitmap_data::CopyPixels_OtherNormalToYUV420(width, height, src.format, src_planes[0], src_pitches[0], src_sample_strides[0], dst);
					} else {
						priv::bitmap_data::CopyPixels_OtherPlanarToYUV420(width, height, src.format, src_planes, src_pitches, src_sample_strides, dst);
					}
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
					sl_uint32 nPlanes = BitmapFormats::getPlanesCount(src.format);
					for (sl_uint32 iPlane = 0; iPlane < nPlanes; iPlane++) {
						sl_uint8* src_row = src_planes[iPlane];
						sl_int32 src_pitch = src_pitches[iPlane];
						sl_int32 src_stride = src_sample_strides[iPlane];
						sl_uint8* dst_row = dst_planes[iPlane];
						sl_int32 dst_pitch = dst_pitches[iPlane];
						sl_int32 dst_stride = dst_sample_strides[iPlane];
						sl_uint32 bytesPerSample = BitmapFormats::getBytesPerSample(src.format);
						if (bytesPerSample == src_stride && bytesPerSample == dst_stride) {
							sl_uint32 n = bytesPerSample * width;
							for (sl_uint32 i = 0; i < height; i++) {
								sl_uint8* s = src_row;
								sl_uint8* d = dst_row;
								for (sl_uint32 k = 0; k < n; k++) {
									*d = *s;
									d++; s++;
								}
								src_row += src_pitch;
								dst_row += dst_pitch;
							}
						} else {
							if (bytesPerSample == 1) {
								for (sl_uint32 i = 0; i < height; i++) {
									sl_uint8* s = src_row;
									sl_uint8* d = dst_row;
									for (sl_uint32 j = 0; j < width; j++) {
										*d = *s;
										s += src_stride;
										d += dst_stride;
									}
									src_row += src_pitch;
									dst_row += dst_pitch;
								}
							} else if (bytesPerSample == 2) {
								for (sl_uint32 i = 0; i < height; i++) {
									sl_uint8* s = src_row;
									sl_uint8* d = dst_row;
									for (sl_uint32 j = 0; j < width; j++) {
										d[0] = s[0];
										d[1] = s[1];
										s += src_stride;
										d += dst_stride;
									}
									src_row += src_pitch;
									dst_row += dst_pitch;
								}
							} else if (bytesPerSample == 3) {
								for (sl_uint32 i = 0; i < height; i++) {
									sl_uint8* s = src_row;
									sl_uint8* d = dst_row;
									for (sl_uint32 j = 0; j < width; j++) {
										d[0] = s[0];
										d[1] = s[1];
										d[2] = s[2];
										s += src_stride;
										d += dst_stride;
									}
									src_row += src_pitch;
									dst_row += dst_pitch;
								}
							} else if (bytesPerSample == 4) {
								for (sl_uint32 i = 0; i < height; i++) {
									sl_uint8* s = src_row;
									sl_uint8* d = dst_row;
									for (sl_uint32 j = 0; j < width; j++) {
										d[0] = s[0];
										d[1] = s[1];
										d[2] = s[2];
										d[3] = s[3];
										s += src_stride;
										d += dst_stride;
									}
									src_row += src_pitch;
									dst_row += dst_pitch;
								}
							} else {
								for (sl_uint32 i = 0; i < height; i++) {
									sl_uint8* s = src_row;
									sl_uint8* d = dst_row;
									for (sl_uint32 j = 0; j < width; j++) {
										for (sl_uint32 k = 0; k < bytesPerSample; k++) {
											d[k] = s[k];
										}
										s += src_stride;
										d += dst_stride;
									}
									src_row += src_pitch;
									dst_row += dst_pitch;
								}
							}
						}
					}
				} else {
					if (BitmapFormats::getPlanesCount(src.format) == 1) {
						if (BitmapFormats::getPlanesCount(dst.format) == 1) {
							priv::bitmap_data::CopyPixels_Normal(width, height, src.format, src_planes[0], src_pitches[0], src_sample_strides[0], dst.format, dst_planes[0], dst_pitches[0], dst_sample_strides[0]);
						} else {
							priv::bitmap_data::CopyPixels_NormalToPlanar(width, height, src.format, src_planes[0], src_pitches[0], src_sample_strides[0], dst.format, dst_planes, dst_pitches, dst_sample_strides);
						}
					} else {
						if (BitmapFormats::getPlanesCount(dst.format) == 1) {
							priv::bitmap_data::CopyPixels_PlanarToNormal(width, height, src.format, src_planes, src_pitches, src_sample_strides, dst.format, dst_planes[0], dst_pitches[0], dst_sample_strides[0]);
						} else {
							priv::bitmap_data::CopyPixels_Planar(width, height, src.format, src_planes, src_pitches, src_sample_strides, dst.format, dst_planes, dst_pitches, dst_sample_strides);
						}
					}
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
		this->sampleStride = 4;

		this->data1 = sl_null;
		this->pitch1 = 0;
		this->sampleStride1 = 0;

		this->data2 = sl_null;
		this->pitch2 = 0;
		this->sampleStride2 = 0;

		this->data3 = sl_null;
		this->pitch3 = 0;
		this->sampleStride3 = 0;
	}

}
