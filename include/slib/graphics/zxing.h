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

#ifndef CHECKHEADER_SLIB_GRAPHICS_ZXING
#define CHECKHEADER_SLIB_GRAPHICS_ZXING

#include "definition.h"

#include "image.h"

namespace slib
{
	
	enum class ZXingFormat
	{
		AZTEC, // Aztec 2D barcode
		CODABAR, // CODABAR 1D
		CODE_39, // Code 39 1D
		CODE_93, // Code 93 1D
		CODE_128, // Code 128 1D
		DATA_MATRIX, // Data Matrix 2D barcode
		EAN_8, // EAN-8 1D
		EAN_13, // EAN-13 1D
		ITF, // ITF (Interleaved Two of Five) 1D
		MAXICODE, // MaxiCode 2D barcode
		PDF_417, // PDF417
		QR_CODE, // QR Code 2D barcode
		RSS_14, // RSS 14
		RSS_EXPANDED, // RSS EXPANDED
		UPC_A, // UPC-A 1D
		UPC_E, // UPC-E 1D
		UPC_EAN_EXTENSION // UPC/EAN extension, Not a stand-alone format
	};
	
	class ZXingGenerateParam
	{
	public:
		ZXingFormat format;
		String text;
		
		sl_uint32 margin;
		sl_uint32 width;
		sl_uint32 height;
		
	public:
		ZXingGenerateParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ZXingGenerateParam)
		
	};
	
	class ZXingScanParam
	{
	public:
		ZXingFormat format;
		Ref<Image> image;

		sl_bool flagTryHarder;
		sl_bool flagTryRotate;
		
		sl_bool flagSubRegion;
		Rectanglei subRegion;
		
	public:
		ZXingScanParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ZXingScanParam)
		
	};
	
	class ZXing
	{
	public:
		static Ref<Image> generate(const ZXingGenerateParam& param);
		
		static String scan(const ZXingScanParam& param);
		
	};
	
}

#endif
