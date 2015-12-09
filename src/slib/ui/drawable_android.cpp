#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/image.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/core/endian.h"
#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidBitmap, "slib/platform/android/ui/UiBitmap")
	SLIB_JNI_STATIC_METHOD(create, "create", "(II)Lslib/platform/android/ui/UiBitmap;");
	SLIB_JNI_STATIC_METHOD(load, "load", "([B)Lslib/platform/android/ui/UiBitmap;");
	SLIB_JNI_METHOD(getWidth, "getWidth", "()I");
	SLIB_JNI_METHOD(getHeight, "getHeight", "()I");
	SLIB_JNI_METHOD(recycle, "recycle", "()V");
	SLIB_JNI_METHOD(read, "read", "(IIII[II)V");
	SLIB_JNI_METHOD(write, "write", "(IIII[II)V");
	SLIB_JNI_METHOD(getCanvas, "getCanvas", "()Lslib/platform/android/ui/Graphics;");
	SLIB_JNI_METHOD(draw, "draw", "(Lslib/platform/android/ui/Graphics;FFFFIIII)V");
	SLIB_JNI_STATIC_METHOD(drawPixels, "drawPixels", "(Lslib/platform/android/ui/Graphics;FFFF[IIII)V");
	SLIB_JNI_STATIC_METHOD(getArrayBuffer, "getArrayBuffer", "()[I");
	SLIB_JNI_STATIC_METHOD(returnArrayBuffer, "returnArrayBuffer", "([I)V");
SLIB_JNI_END_CLASS


class _Android_Bitmap : public Bitmap
{
	SLIB_DECLARE_OBJECT(_Android_Bitmap, Bitmap)
public:
	JniGlobal<jobject> m_bitmap;

	sl_uint32 m_width;
	sl_uint32 m_height;

	sl_bool m_flagFreeOnRelease;
	Ref<Referable> m_ref;

	_Android_Bitmap()
	{
		m_flagFreeOnRelease = sl_true;
	}

	~_Android_Bitmap()
	{
		if (m_flagFreeOnRelease) {
			_JAndroidBitmap::recycle.call(m_bitmap);
		}
	}

	static Ref<_Android_Bitmap> create(jobject jbitmap, sl_bool flagFreeOnRelease, Referable* ref)
	{
		Ref<_Android_Bitmap> ret;
		if (jbitmap) {
			jint width = _JAndroidBitmap::getWidth.callInt(jbitmap);
			jint height = _JAndroidBitmap::getHeight.callInt(jbitmap);
			if (width > 0 && height > 0) {
				JniGlobal<jobject> bitmap = jbitmap;
				if (bitmap.isNotNull()) {
					ret = new _Android_Bitmap();
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
				_JAndroidBitmap::recycle.call(jbitmap);
			}
		}
		return ret;
	}

	static Ref<_Android_Bitmap> create(sl_uint32 width, sl_uint32 height)
	{
		Ref<_Android_Bitmap> ret;
		if (width > 0 && height > 0) {
			JniLocal<jobject> jbitmap = _JAndroidBitmap::create.callObject(sl_null, width, height);
			if (jbitmap.isNotNull()) {
				JniGlobal<jobject> bitmap = jbitmap;
				if (bitmap.isNotNull()) {
					ret = new _Android_Bitmap();
					if (ret.isNotNull()) {
						ret->m_width = width;
						ret->m_height = height;
						ret->m_bitmap = bitmap;
						return ret;
					}
				}
				_JAndroidBitmap::recycle.call(jbitmap);
			}
		}
		return ret;
	}


	static jobject loadJBitmap(const void* buf, sl_size size)
	{
		JniLocal<jbyteArray> imageData = Jni::newByteArray(size);
		if (imageData.isNotNull()) {
			Jni::setByteArrayRegion(imageData, 0, size, (jbyte*)buf);
			return _JAndroidBitmap::load.callObject(sl_null, imageData.value);
		}
		return 0;
	}

	static Ref<_Android_Bitmap> load(const void* buf, sl_size size)
	{
		Ref<_Android_Bitmap> ret;
		JniLocal<jobject> jbitmap = loadJBitmap(buf, size);
		if (jbitmap.isNotNull()) {
			JniGlobal<jobject> bitmap = jbitmap;
			if (bitmap.isNotNull()) {
				ret = new _Android_Bitmap();
				if (ret.isNotNull()) {
					ret->m_width = _JAndroidBitmap::getWidth.callInt(jbitmap);
					ret->m_height = _JAndroidBitmap::getHeight.callInt(jbitmap);
					ret->m_bitmap = bitmap;
					return ret;
				}
			}
			_JAndroidBitmap::recycle.call(jbitmap);
		}
		return ret;
	}

	sl_uint32 getBitmapWidth()
	{
		return m_width;
	}

	sl_uint32 getBitmapHeight()
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

		BitmapFormat formatRaw = Endian::isLE() ? bitmapFormatBGRA : bitmapFormatARGB;

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
			data.data = memData.getBuf();
			data.pitch = width << 2;
		}
		if (!flagRead) {
			data.copyPixelsFrom(bitmapData);
		}

		sl_bool ret = sl_false;
		JniLocal<jintArray> abuf = (jintArray)(_JAndroidBitmap::getArrayBuffer.callObject(sl_null));
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
						_JAndroidBitmap::read.call(jbitmap, x, yCurrent, width, heightSegment, abuf.value, width);
						for (sl_uint32 r = 0; r < heightSegment; r++) {
							Jni::getIntArrayRegion(abuf.value, r*width, width, (jint*)(pixelsCurrent));
							pixelsCurrent += data.pitch;
						}
					} else {
						for (sl_uint32 r = 0; r < heightSegment; r++) {
							Jni::setIntArrayRegion(abuf.value, r*width, width, (jint*)(pixelsCurrent));
							pixelsCurrent += data.pitch;
						}
						_JAndroidBitmap::write.call(jbitmap, x, yCurrent, width, heightSegment, abuf.value, width);
					}
					yCurrent += heightSegment;
					heightRemain -= heightSegment;
				}
				ret = sl_true;
			}
			_JAndroidBitmap::returnArrayBuffer.call(sl_null, abuf.value);
		}
		if (ret && flagRead) {
			bitmapData.copyPixelsFrom(data);
		}
		return ret;
	}

	sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& bitmapData)
	{
		return readWritePixels(sl_true, x, y, bitmapData);
	}

	sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& bitmapData)
	{
		return readWritePixels(sl_false, x, y, *((BitmapData*)&bitmapData));
	}

	sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color)
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

		JniLocal<jintArray> abuf = (jintArray)(_JAndroidBitmap::getArrayBuffer.callObject(sl_null));

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
						_JAndroidBitmap::write.call(jbitmap, x, yCurrent, width, heightSegment, abuf.value, width);
						yCurrent += heightSegment;
						heightRemain -= heightSegment;
					}
					ret = sl_true;
				}
			}
			_JAndroidBitmap::returnArrayBuffer.call(sl_null, abuf.value);
		}
		return ret;
	}

	Ref<Canvas> getCanvas()
	{
		JniLocal<jobject> jcanvas = _JAndroidBitmap::getCanvas.callObject(m_bitmap.get());
		if (jcanvas.isNotNull()) {
			return UIPlatform::createCanvas(jcanvas);
		}
		return Ref<Canvas>::null();
	}

	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
	{
		jobject jcanvas = UIPlatform::getCanvasHandle(canvas);
		if (!jcanvas) {
			return;
		}
		_JAndroidBitmap::draw.call(m_bitmap, jcanvas, rectDst.left, rectDst.top, rectDst.right, rectDst.bottom
			, (sl_int32)(rectSrc.left), (sl_int32)(rectSrc.top), (sl_int32)(rectSrc.right), (sl_int32)(rectSrc.bottom));
	}
};

Ref<Drawable> UIPlatform::createImageDrawable(jobject bitmap, sl_bool flagFreeOnRelease, Referable* ref)
{
	Ref<_Android_Bitmap> ret = _Android_Bitmap::create(bitmap, flagFreeOnRelease, ref);
	return Ref<Drawable>::from(ret);
}

jobject UIPlatform::getImageDrawableHandle(Drawable* _drawable)
{
	if (!(_Android_Bitmap::checkInstance(_drawable))) {
		return 0;
	}
	_Android_Bitmap* drawable = (_Android_Bitmap*)_drawable;
	return drawable->m_bitmap;
}

class _Android_ImageDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT(_Android_ImageDrawable, Drawable);
public:
	ImageDesc m_image;

	_Android_ImageDrawable()
	{
	}

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

	void onDraw(Canvas* canvas, const Rectangle& _rectDst, const Rectangle& _rectSrc)
	{
		jobject jcanvas = UIPlatform::getCanvasHandle(canvas);
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
		if (Math::isNearZero(osw) || Math::isNearZero(osh)) {
			return;
		}
		sl_real odw = rectDst.getWidth();
		sl_real odh = rectDst.getHeight();
		if (Math::isNearZero(odw) || Math::isNearZero(odh)) {
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
						_JAndroidBitmap::drawPixels.call(sl_null, jcanvas, rectDst.left, yCurrent, rectDst.right, yCurrent + heightSegmentDst
							, abuf.value, width, width, heightSegment);

						yCurrent += heightSegmentDst;
						heightRemain -= heightSegment;
					}
					delete[] jpixels;
				}
			}
			_JAndroidBitmap::returnArrayBuffer.call(sl_null, abuf.value);
		}
	}

	sl_real getDrawableWidth()
	{
		return (sl_real)(m_image.width);
	}

	sl_real getDrawableHeight()
	{
		return (sl_real)(m_image.height);
	}
};

Ref<Drawable> UI::createDrawableFromImage(const ImageDesc& desc)
{
	Ref<_Android_ImageDrawable> ret = _Android_ImageDrawable::create(desc);
	return Ref<Drawable>::from(ret);
}

Ref<Drawable> UI::loadDrawableFromMemory(const void* buf, sl_size size)
{
	return Ref<Drawable>::from(_Android_Bitmap::load(buf, size));
}

Ref<Bitmap> UI::createBitmap(sl_uint32 width, sl_uint32 height)
{
	return Ref<Bitmap>::from(_Android_Bitmap::create(width, height));
}

Ref<Drawable> UI::createDrawableFromBitmap(const Ref<Bitmap>& bitmap)
{
	return Ref<Drawable>::null();
}

Ref<Bitmap> UI::loadBitmapFromMemory(const void* mem, sl_size size)
{
	return Ref<Bitmap>::from(_Android_Bitmap::load(mem, size));
}

SLIB_UI_NAMESPACE_END

#endif
