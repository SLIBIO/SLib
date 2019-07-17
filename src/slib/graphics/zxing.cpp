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

#include "slib/graphics/zxing.h"

#include "zxing/MultiFormatWriter.h"
#include "zxing/MultiFormatReader.h"
#include "zxing/Result.h"
#include "zxing/BitMatrix.h"
#include "zxing/DecodeHints.h"
#include "zxing/HybridBinarizer.h"
#include "zxing/GenericLuminanceSource.h"

using namespace ZXing;

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ZXingGenerateParam)
	
	ZXingGenerateParam::ZXingGenerateParam()
	{
		format = ZXingFormat::QR_CODE;
		margin = 0;
		width = 512;
		height = 512;
	}
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ZXingScanParam)
	
	ZXingScanParam::ZXingScanParam()
	{
		format = ZXingFormat::QR_CODE;
		flagTryHarder = sl_true;
		flagTryRotate = sl_false;
		flagSubRegion = sl_false;
	}

	namespace priv
	{
		namespace zxing
		{
			BarcodeFormat GetBarcodeFormat(ZXingFormat format)
			{
				switch (format) {
					case ZXingFormat::AZTEC:
						return BarcodeFormat::AZTEC;
					case ZXingFormat::CODABAR:
						return BarcodeFormat::CODABAR;
					case ZXingFormat::CODE_39:
						return BarcodeFormat::CODE_39;
					case ZXingFormat::CODE_93:
						return BarcodeFormat::CODE_93;
					case ZXingFormat::CODE_128:
						return BarcodeFormat::CODE_128;
					case ZXingFormat::DATA_MATRIX:
						return BarcodeFormat::DATA_MATRIX;
					case ZXingFormat::EAN_8:
						return BarcodeFormat::EAN_8;
					case ZXingFormat::EAN_13:
						return BarcodeFormat::EAN_13;
					case ZXingFormat::ITF:
						return BarcodeFormat::ITF;
					case ZXingFormat::MAXICODE:
						return BarcodeFormat::MAXICODE;
					case ZXingFormat::PDF_417:
						return BarcodeFormat::PDF_417;
					case ZXingFormat::QR_CODE:
						return BarcodeFormat::QR_CODE;
					case ZXingFormat::RSS_14:
						return BarcodeFormat::RSS_14;
					case ZXingFormat::RSS_EXPANDED:
						return BarcodeFormat::RSS_EXPANDED;
					case ZXingFormat::UPC_A:
						return BarcodeFormat::UPC_A;
					case ZXingFormat::UPC_E:
						return BarcodeFormat::UPC_E;
					case ZXingFormat::UPC_EAN_EXTENSION:
						return BarcodeFormat::UPC_EAN_EXTENSION;
				}
				return BarcodeFormat::QR_CODE;
			}
		}
	}

	Ref<Image> ZXing::generate(const ZXingGenerateParam& param)
	{
		MultiFormatWriter writer(priv::zxing::GetBarcodeFormat(param.format));
		writer.setEncoding(CharacterSet::UTF8);
		writer.setMargin((int)(param.margin));
		Memory data;
		if (sizeof(wchar_t) == 2) {
			data = param.text.toUtf16();
		} else if (sizeof(wchar_t) == 4) {
			data = param.text.toUtf32();
		} else {
			return sl_null;
		}
		try {
			BitMatrix matrix(writer.encode((wchar_t*)(data.getData()), (int)(param.width), (int)(param.height)));
			sl_int32 width = (sl_int32)(matrix.width());
			sl_int32 height = (sl_int32)(matrix.height());
			if (width > 0 && height > 0) {
				Ref<Image> ret = Image::create(width, height);
				if (ret.isNotNull()) {
					for (sl_int32 y = 0; y < height; y++) {
						Color* color = ret->getColorsAt(0, y);
						for (sl_int32 x = 0; x < width; x++) {
							*color = matrix.get(x, y) ? Color::Black : Color::White;
							color++;
						}
					}
					return ret;
				}
			}
		} catch (std::exception&) {
		}
		return sl_null;
	}
	
	String ZXing::scan(const ZXingScanParam& param)
	{
		Ref<Image> image = param.image;
		if (image.isNull()) {
			return sl_null;
		}
		DecodeHints hints;
		hints.setCharacterSet("UTF-8");
		hints.setShouldTryHarder(param.flagTryHarder);
		hints.setShouldTryRotate(sl_false);
		hints.setPossibleFormats({priv::zxing::GetBarcodeFormat(param.format)});
		MultiFormatReader reader(hints);
		try {
			std::shared_ptr<GenericLuminanceSource> src;
			if (param.flagSubRegion) {
				Rectanglei region = param.subRegion;
				int width = region.getWidth();
				if (region.left + width > (int)(image->getWidth())) {
					return sl_null;
				}
				int height = region.getHeight();
				if (region.top + height > (int)(image->getHeight())) {
					return sl_null;
				}
				int stride = image->getWidth();
				src = std::make_shared<GenericLuminanceSource>(width, height, image->getColors() + (stride * region.top + region.left), stride * 4, 4, 0, 1, 2);
			} else {
				src = std::make_shared<GenericLuminanceSource>(image->getWidth(), image->getHeight(), image->getColors(), image->getStride() * 4, 4, 0, 1, 2);
			}
			HybridBinarizer binary(src);
			Result result = reader.read(binary);
			if (param.flagTryRotate) {
				if (!(result.isValid())) {
					std::shared_ptr<BinaryBitmap> r = binary.rotated(180);
					result = reader.read(*r);
				}
				if (!(result.isValid())) {
					std::shared_ptr<BinaryBitmap> r = binary.rotated(90);
					result = reader.read(*r);
				}
				if (!(result.isValid())) {
					std::shared_ptr<BinaryBitmap> r = binary.rotated(270);
					result = reader.read(*r);
				}
			}
			if (result.isValid()) {
				std::wstring wstr(result.text());
				return String(wstr.c_str());
			}
		} catch (std::exception&) {
		}
		return sl_null;
	}

}
