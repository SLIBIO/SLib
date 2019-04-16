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

#ifndef CHECKHEADER_SLIB_GRAPHICS_BITMAP_DATA
#define CHECKHEADER_SLIB_GRAPHICS_BITMAP_DATA

#include "definition.h"

#include "bitmap_format.h"
#include "color.h"

#include "../core/ref.h"

namespace slib
{
	
	class SLIB_EXPORT ColorComponentBuffer
	{
	public:
		sl_uint32 width;
		sl_uint32 height;
		void* data;
		sl_int32 pitch; // bytes of a physical row
		sl_int32 sampleStride; // The distance between adjacent pixel samples, in bytes
		Ref<Referable> ref;

	public:
		ColorComponentBuffer();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ColorComponentBuffer)
		
	};
	
	class SLIB_EXPORT BitmapData
	{
	public:
		sl_uint32 width;
		sl_uint32 height;
		BitmapFormat format;

		// plane0
		void* data; // samples
		sl_int32 pitch; // number of bytes of a row
		sl_int32 sampleStride; // The distance between adjacent pixel samples, in bytes
		Ref<Referable> ref; // reference for samples

		// plane1
		void* data1; // samples
		sl_int32 pitch1; // number of bytes of a row
		sl_int32 sampleStride1; // The distance between adjacent pixel samples, in bytes
		Ref<Referable> ref1; // reference for samples
	
		// plane2
		void* data2; // samples
		sl_int32 pitch2; // number of bytes of a row
		sl_int32 sampleStride2; // The distance between adjacent pixel samples, in bytes
		Ref<Referable> ref2; // reference for samples

		// plane3
		void* data3; // samples
		sl_int32 pitch3; // number of bytes of a row
		sl_int32 sampleStride3; // The distance between adjacent pixel samples, in bytes
		Ref<Referable> ref3; // reference for samples
	
	public:
		BitmapData();

		BitmapData(sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride = 0);

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(BitmapData)

	public:
		void*& planeData(sl_uint32 plane);

		void* planeData(sl_uint32 plane) const;
	
		sl_int32& planePitch(sl_uint32 plane);

		sl_int32 planePitch(sl_uint32 plane) const;

		sl_int32& planeSampleStride(sl_uint32 plane);

		sl_int32 planeSampleStride(sl_uint32 plane) const;

		static sl_int32 calculatePitchAlign1(sl_uint32 width, sl_uint32 bitsPerSample);

		static sl_int32 calculatePitchAlign2(sl_uint32 width, sl_uint32 bitsPerSample);

		static sl_int32 calculatePitchAlign4(sl_uint32 width, sl_uint32 bitsPerSample);

		static sl_int32 calculatePitchAlign8(sl_uint32 width, sl_uint32 bitsPerSample);

		static sl_int32 calculatePitchAlign16(sl_uint32 width, sl_uint32 bitsPerSample);
	
		void fillDefaultValues();

		sl_size getTotalSize() const;

		// returns the number of color components
		sl_uint32 getColorComponentBuffers(ColorComponentBuffer* buffers) const;

		void copyPixelsFrom(const BitmapData& other) const;

		void setFromColors(sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride = 0);

	};

}

#endif
