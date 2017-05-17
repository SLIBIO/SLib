/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/graphics/drawable.h"

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"
#include "slib/core/scoped.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_JAndroidBitmap, "slib/platform/android/ui/UiBitmap")
		SLIB_JNI_STATIC_METHOD(drawPixels, "drawPixels", "(Lslib/platform/android/ui/Graphics;FFFF[IIIIFFI)V");
		SLIB_JNI_STATIC_METHOD(drawPixelsWithColorMatrix, "drawPixels", "(Lslib/platform/android/ui/Graphics;FFFF[IIIIFFIFFFFFFFFFFFFFFFFFFFF)V");
		SLIB_JNI_STATIC_METHOD(getArrayBuffer, "getArrayBuffer", "()[I");
		SLIB_JNI_STATIC_METHOD(returnArrayBuffer, "returnArrayBuffer", "([I)V");
	SLIB_JNI_END_CLASS

	class _Android_ImageDrawable : public Drawable
	{
		SLIB_DECLARE_OBJECT
	public:
		ImageDesc m_image;

	public:
		static Ref<_Android_ImageDrawable> create(const ImageDesc& image)
		{
			Ref<_Android_ImageDrawable> ret;
			sl_uint32 width = image.width;
			sl_uint32 height = image.height;
			if (width > 0 && height > 0) {
				ret = new _Android_ImageDrawable();
				if (ret.isNotNull()) {
					ret->m_image = image;
					return ret;
				}
			}
			return ret;
		}

		void onDraw(Canvas* canvas, const Rectangle& _rectDst, const Rectangle& _rectSrc, const DrawParam& param) override
		{
			jobject jcanvas = GraphicsPlatform::getCanvasHandle(canvas);
			if (!jcanvas) {
				return;
			}
			
			ImageDesc& image = m_image;

			sl_uint32 bw = image.width;
			sl_uint32 bh = image.height;
			Rectangle rectDst = _rectDst;
			Rectangle rectSrc = _rectSrc;
			sl_real osw = rectSrc.getWidth();
			sl_real osh = rectSrc.getHeight();
			if (Math::isAlmostZero(osw) || Math::isAlmostZero(osh)) {
				return;
			}
			sl_real odw = rectDst.getWidth();
			sl_real odh = rectDst.getHeight();
			if (Math::isAlmostZero(odw) || Math::isAlmostZero(odh)) {
				return;
			}
			if (rectSrc.right < rectSrc.left || rectSrc.bottom < rectSrc.top) {
				return;
			}
			if (rectDst.right < rectDst.left || rectDst.bottom < rectDst.top) {
				return;
			}
			if (rectSrc.left < 0) {
				rectDst.left -= rectSrc.left * odw / osw;
				rectSrc.left = 0;
			}
			if (rectSrc.right > bw) {
				rectDst.right -= (rectSrc.right - bw) * odw / osw;
				rectSrc.right = bw;
			}
			if (rectSrc.top < 0) {
				rectDst.top -= rectSrc.top * odh / osh;
				rectSrc.top = 0;
			}
			if (rectSrc.bottom > bh) {
				rectDst.bottom -= (rectSrc.bottom - bh) * odh / osh;
				rectSrc.bottom = bh;
			}
			if (rectSrc.right < rectSrc.left || rectSrc.bottom < rectSrc.top) {
				return;
			}
			if (rectDst.right < rectDst.left || rectDst.bottom < rectDst.top) {
				return;
			}

			float alpha = 1;
			if (param.useAlpha) {
				alpha = (float)(param.alpha);
			}
			float blur = 0;
			if (param.useBlur) {
				blur = (float)(param.blurRadius);
			}
			int tileMode = 0;
			if (param.tiled) {
				tileMode = 1;
			}
			const Color4f& cr = param.colorMatrix.red;
			const Color4f& cg = param.colorMatrix.green;
			const Color4f& cb = param.colorMatrix.blue;
			const Color4f& ca = param.colorMatrix.alpha;
			const Color4f& cc = param.colorMatrix.bias;

			sl_uint32 width = (sl_uint32)(rectSrc.getWidth());
			sl_uint32 height = (sl_uint32)(rectSrc.getHeight());

			JniLocal<jintArray> abuf = (jintArray)(_JAndroidBitmap::getArrayBuffer.callObject(sl_null));
			if (abuf.isNotNull()) {
				sl_uint32 nAbuf = Jni::getArrayLength(abuf);
				if (nAbuf >= width) {
					sl_uint32 nRowsMax = nAbuf / width;
					sl_uint32 heightRemain = height;

					float yCurrent = rectDst.top;
					Color* pixelsCurrent = image.colors + (sl_int32)(rectSrc.left) + ((sl_int32)(rectSrc.top)) * image.stride;

					sl_uint32* jpixels = new sl_uint32[width];

					if (jpixels) {

						while (heightRemain > 0) {

							sl_uint32 heightSegment = heightRemain;
							if (heightSegment > nRowsMax) {
								heightSegment = nRowsMax;
							}
							float heightSegmentDst = (sl_real)heightSegment * rectDst.getHeight() / height;
							for (sl_uint32 r = 0; r < heightSegment; r++) {
								for (sl_uint32 x = 0; x < width; x++) {
									jpixels[x] = pixelsCurrent[x].getARGB();
								}
								Jni::setIntArrayRegion(abuf.value, r*width, width, (jint*)(jpixels));
								pixelsCurrent += image.stride;
							}
							if (param.useColorMatrix) {
								_JAndroidBitmap::drawPixelsWithColorMatrix.call(sl_null, jcanvas,
									rectDst.left, yCurrent, rectDst.right, yCurrent + heightSegmentDst,
									abuf.value, width, width, heightSegment,
									alpha, blur, tileMode,
									cr.x, cr.y, cr.z, cr.w,
									cg.x, cg.y, cg.z, cg.w,
									cb.x, cb.y, cb.z, cb.w,
									ca.x, ca.y, ca.z, ca.w,
									cc.x, cc.y, cc.z, cc.w
									);
							} else {
								_JAndroidBitmap::drawPixels.call(sl_null, jcanvas,
									rectDst.left, yCurrent, rectDst.right, yCurrent + heightSegmentDst,
									abuf.value, width, width, heightSegment,
									alpha, blur, tileMode);
							}

							yCurrent += heightSegmentDst;
							heightRemain -= heightSegment;
						}
						delete[] jpixels;
					}
				}
				_JAndroidBitmap::returnArrayBuffer.call(sl_null, abuf.value);
			}
		}

		sl_real getDrawableWidth() override
		{
			return (sl_real)(m_image.width);
		}

		sl_real getDrawableHeight() override
		{
			return (sl_real)(m_image.height);
		}
	};

	SLIB_DEFINE_OBJECT(_Android_ImageDrawable, Drawable);

	Ref<Drawable> PlatformDrawable::create(const ImageDesc& desc)
	{
		return _Android_ImageDrawable::create(desc);
	}

	Ref<Drawable> PlatformDrawable::loadFromMemory(const void* buf, sl_size size)
	{
		return Bitmap::loadFromMemory(buf, size);
	}

}

#endif
