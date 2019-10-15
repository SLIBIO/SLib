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

#include "slib/core/definition.h"

#if defined(SLIB_GRAPHICS_IS_ANDROID)

#include "slib/graphics/bitmap.h"

#include "slib/graphics/platform.h"
#include "slib/core/endian.h"
#include "slib/core/scoped.h"

namespace slib
{

	namespace priv
	{
		namespace android
		{

			SLIB_JNI_BEGIN_CLASS(JBitmap, "slib/platform/android/ui/UiBitmap")
				SLIB_JNI_STATIC_METHOD(create, "create", "(II)Lslib/platform/android/ui/UiBitmap;");
				SLIB_JNI_STATIC_METHOD(load, "load", "(Landroid/app/Activity;[B)Lslib/platform/android/ui/UiBitmap;");
				SLIB_JNI_METHOD(getWidth, "getWidth", "()I");
				SLIB_JNI_METHOD(getHeight, "getHeight", "()I");
				SLIB_JNI_METHOD(recycle, "recycle", "()V");
				SLIB_JNI_METHOD(read, "read", "(IIII[II)V");
				SLIB_JNI_METHOD(write, "write", "(IIII[II)V");
				SLIB_JNI_METHOD(getCanvas, "getCanvas", "()Lslib/platform/android/ui/Graphics;");
				SLIB_JNI_METHOD(draw, "draw", "(Lslib/platform/android/ui/Graphics;FFFFIIIIFF)V");
				SLIB_JNI_METHOD(drawWithColorMatrix, "draw", "(Lslib/platform/android/ui/Graphics;FFFFIIIIFFFFFFFFFFFFFFFFFFFFFF)V");
				SLIB_JNI_STATIC_METHOD(getArrayBuffer, "getArrayBuffer", "()[I");
				SLIB_JNI_STATIC_METHOD(returnArrayBuffer, "returnArrayBuffer", "([I)V");
			SLIB_JNI_END_CLASS

			class BitmapImpl : public Bitmap
			{
				SLIB_DECLARE_OBJECT
			public:
				JniGlobal<jobject> m_bitmap;

				sl_uint32 m_width;
				sl_uint32 m_height;

				sl_bool m_flagFreeOnRelease;
				Ref<Referable> m_ref;

			public:
				BitmapImpl()
				{
					m_flagFreeOnRelease = sl_true;
				}

				~BitmapImpl()
				{
					if (m_flagFreeOnRelease) {
						JBitmap::recycle.call(m_bitmap);
					}
				}

			public:
				static Ref<BitmapImpl> create(jobject jbitmap, sl_bool flagFreeOnRelease, Referable* ref)
				{
					Ref<BitmapImpl> ret;
					if (jbitmap) {
						jint width = JBitmap::getWidth.callInt(jbitmap);
						jint height = JBitmap::getHeight.callInt(jbitmap);
						if (width > 0 && height > 0) {
							JniGlobal<jobject> bitmap = jbitmap;
							if (bitmap.isNotNull()) {
								ret = new BitmapImpl();
								if (ret.isNotNull()) {
									ret->m_bitmap = bitmap;
									ret->m_width = (sl_real)width;
									ret->m_height = (sl_real)height;
									ret->m_flagFreeOnRelease = flagFreeOnRelease;
									ret->m_ref = ref;
									return ret;
								}
							}
						}
						if (flagFreeOnRelease) {
							JBitmap::recycle.call(jbitmap);
						}
					}
					return ret;
				}

				static Ref<BitmapImpl> create(sl_uint32 width, sl_uint32 height)
				{
					Ref<BitmapImpl> ret;
					if (width > 0 && height > 0) {
						JniLocal<jobject> jbitmap = JBitmap::create.callObject(sl_null, width, height);
						if (jbitmap.isNotNull()) {
							JniGlobal<jobject> bitmap = jbitmap;
							if (bitmap.isNotNull()) {
								ret = new BitmapImpl();
								if (ret.isNotNull()) {
									ret->m_width = width;
									ret->m_height = height;
									ret->m_bitmap = bitmap;
									return ret;
								}
							}
							JBitmap::recycle.call(jbitmap);
						}
					}
					return ret;
				}

				static jobject loadJBitmap(const void* buf, sl_size size)
				{
					JniLocal<jbyteArray> imageData = Jni::newByteArray(size);
					if (imageData.isNotNull()) {
						Jni::setByteArrayRegion(imageData, 0, size, (jbyte*)buf);
						return JBitmap::load.callObject(sl_null, Android::getCurrentActivity(), imageData.value);
					}
					return 0;
				}

				static Ref<BitmapImpl> load(const void* buf, sl_size size)
				{
					Ref<BitmapImpl> ret;
					JniLocal<jobject> jbitmap = loadJBitmap(buf, size);
					if (jbitmap.isNotNull()) {
						JniGlobal<jobject> bitmap = jbitmap;
						if (bitmap.isNotNull()) {
							ret = new BitmapImpl();
							if (ret.isNotNull()) {
								ret->m_width = JBitmap::getWidth.callInt(jbitmap);
								ret->m_height = JBitmap::getHeight.callInt(jbitmap);
								ret->m_bitmap = bitmap;
								return ret;
							}
						}
						JBitmap::recycle.call(jbitmap);
					}
					return ret;
				}

				sl_uint32 getBitmapWidth() override
				{
					return m_width;
				}

				sl_uint32 getBitmapHeight() override
				{
					return m_height;
				}

				sl_bool readWritePixels(sl_bool flagRead, sl_uint32 x, sl_uint32 y, BitmapData& bitmapData)
				{
					jobject jbitmap = m_bitmap;
					sl_uint32 w = m_width;
					sl_uint32 h = m_height;

					if (x >= w || y >= h) {
						return sl_false;
					}
					sl_uint32 width = bitmapData.width;
					sl_uint32 height = bitmapData.height;
					if (width > w - x) {
						width = w - x;
					}
					if (height > h - y) {
						height = h - y;
					}
					if (width == 0 || height == 0) {
						return sl_true;
					}

					BitmapFormat formatRaw = Endian::isLE() ? BitmapFormat::BGRA : BitmapFormat::ARGB;

					BitmapData data;
					Memory memData;
					data.width = width;
					data.height = height;
					data.format = formatRaw;
					if (bitmapData.format == formatRaw) {
						data.data = bitmapData.data;
						data.pitch = bitmapData.pitch;
					} else {
						memData = Memory::create((width * height) << 2);
						if (memData.isNull()) {
							return sl_false;
						}
						data.data = memData.getData();
						data.pitch = width << 2;
					}
					if (!flagRead) {
						data.copyPixelsFrom(bitmapData);
					}

					sl_bool ret = sl_false;
					JniLocal<jintArray> abuf = (jintArray)(JBitmap::getArrayBuffer.callObject(sl_null));
					if (abuf.isNotNull()) {
						sl_uint32 nAbuf = Jni::getArrayLength(abuf);
						if (nAbuf >= width) {

							sl_uint32 nRowsMax = nAbuf / width;
							sl_uint32 heightRemain = height;
							sl_uint32 yCurrent = y;
							sl_uint8* pixelsCurrent = (sl_uint8*)(data.data);

							while (heightRemain > 0) {
								sl_uint32 heightSegment = heightRemain;
								if (heightSegment > nRowsMax) {
									heightSegment = nRowsMax;
								}
								if (flagRead) {
									JBitmap::read.call(jbitmap, x, yCurrent, width, heightSegment, abuf.value, width);
									for (sl_uint32 r = 0; r < heightSegment; r++) {
										Jni::getIntArrayRegion(abuf.value, r*width, width, (jint*)(pixelsCurrent));
										pixelsCurrent += data.pitch;
									}
								} else {
									for (sl_uint32 r = 0; r < heightSegment; r++) {
										Jni::setIntArrayRegion(abuf.value, r*width, width, (jint*)(pixelsCurrent));
										pixelsCurrent += data.pitch;
									}
									JBitmap::write.call(jbitmap, x, yCurrent, width, heightSegment, abuf.value, width);
								}
								yCurrent += heightSegment;
								heightRemain -= heightSegment;
							}
							ret = sl_true;
						}
						JBitmap::returnArrayBuffer.call(sl_null, abuf.value);
					}
					if (ret && flagRead) {
						bitmapData.copyPixelsFrom(data);
					}
					return ret;
				}

				sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& bitmapData) override
				{
					return readWritePixels(sl_true, x, y, bitmapData);
				}

				sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& bitmapData) override
				{
					return readWritePixels(sl_false, x, y, *((BitmapData*)&bitmapData));
				}

				sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color) override
				{
					jobject jbitmap = m_bitmap;
					sl_uint32 w = m_width;
					sl_uint32 h = m_height;

					if (x >= w || y >= h) {
						return sl_false;
					}
					if (width > w - x) {
						width = w - x;
					}
					if (height > h - y) {
						height = h - y;
					}
					if (width == 0 || height == 0) {
						return sl_true;
					}

					sl_bool ret = sl_false;

					JniLocal<jintArray> abuf = (jintArray)(JBitmap::getArrayBuffer.callObject(sl_null));

					if (abuf.isNotNull()) {

						sl_uint32 nAbuf = Jni::getArrayLength(abuf);
						if (nAbuf >= width) {

							sl_uint32 nRowsMax = nAbuf / width;
							sl_uint32 heightRemain = height;
							sl_uint32 yCurrent = y;
							sl_uint32 colorValue = color.getARGB();

							SLIB_SCOPED_BUFFER(sl_uint32, 2048, bufRow, width);
							if (bufRow) {
								for (sl_uint32 k = 0; k < width; k++) {
									bufRow[k] = colorValue;
								}
								while (heightRemain > 0) {
									sl_uint32 heightSegment = heightRemain;
									if (heightSegment > nRowsMax) {
										heightSegment = nRowsMax;
									}
									for (sl_uint32 r = 0; r < heightSegment; r++) {
										Jni::setIntArrayRegion(abuf.value, r*width, width, (jint*)(bufRow));
									}
									JBitmap::write.call(jbitmap, x, yCurrent, width, heightSegment, abuf.value, width);
									yCurrent += heightSegment;
									heightRemain -= heightSegment;
								}
								ret = sl_true;
							}
						}
						JBitmap::returnArrayBuffer.call(sl_null, abuf.value);
					}
					return ret;
				}

				Ref<Canvas> getCanvas() override
				{
					JniLocal<jobject> jcanvas = JBitmap::getCanvas.callObject(m_bitmap.get());
					if (jcanvas.isNotNull()) {
						return GraphicsPlatform::createCanvas(CanvasType::Bitmap, jcanvas);
					}
					return sl_null;
				}

				void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override
				{
					jobject jcanvas = GraphicsPlatform::getCanvasHandle(canvas);
					if (!jcanvas) {
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
					if (param.useColorMatrix) {
						const Color4f& cr = param.colorMatrix.red;
						const Color4f& cg = param.colorMatrix.green;
						const Color4f& cb = param.colorMatrix.blue;
						const Color4f& ca = param.colorMatrix.alpha;
						const Color4f& cc = param.colorMatrix.bias;
						JBitmap::drawWithColorMatrix.call(m_bitmap, jcanvas,
							rectDst.left, rectDst.top, rectDst.right, rectDst.bottom,
							(sl_int32)(rectSrc.left), (sl_int32)(rectSrc.top), (sl_int32)(rectSrc.right), (sl_int32)(rectSrc.bottom),
							alpha, blur,
							cr.x, cr.y, cr.z, cr.w,
							cg.x, cg.y, cg.z, cg.w,
							cb.x, cb.y, cb.z, cb.w,
							ca.x, ca.y, ca.z, ca.w,
							cc.x, cc.y, cc.z, cc.w
							);
					} else {
						JBitmap::draw.call(m_bitmap, jcanvas,
							rectDst.left, rectDst.top, rectDst.right, rectDst.bottom,
							(sl_int32)(rectSrc.left), (sl_int32)(rectSrc.top), (sl_int32)(rectSrc.right), (sl_int32)(rectSrc.bottom),
							alpha, blur);
					}
				}
			};

			SLIB_DEFINE_OBJECT(BitmapImpl, Bitmap)

		}
	}

	using namespace priv::android;

	Ref<Bitmap> Bitmap::create(sl_uint32 width, sl_uint32 height)
	{
		return BitmapImpl::create(width, height);
	}

	Ref<Bitmap> Bitmap::loadFromMemory(const void* mem, sl_size size)
	{
		return BitmapImpl::load(mem, size);
	}

	Ref<Bitmap> GraphicsPlatform::createBitmap(jobject bitmap, sl_bool flagFreeOnRelease, Referable* ref)
	{
		return BitmapImpl::create(bitmap, flagFreeOnRelease, ref);
	}

	jobject GraphicsPlatform::getBitmapHandle(Bitmap* _bitmap)
	{
		if (BitmapImpl* bitmap = CastInstance<BitmapImpl>(_bitmap)) {
			return bitmap->m_bitmap;
		}
		return 0;
	}

}

#endif
