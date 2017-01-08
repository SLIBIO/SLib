#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/platform.h"

#include "../../../inc/slib/core/endian.h"
#include "../../../inc/slib/core/scoped.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidBitmap, "slib/platform/android/ui/UiBitmap")
	SLIB_JNI_STATIC_METHOD(create, "create", "(II)Lslib/platform/android/ui/UiBitmap;");
	SLIB_JNI_STATIC_METHOD(load, "load", "([B)Lslib/platform/android/ui/UiBitmap;");
	SLIB_JNI_METHOD(getWidth, "getWidth", "()I");
	SLIB_JNI_METHOD(getHeight, "getHeight", "()I");
	SLIB_JNI_METHOD(recycle, "recycle", "()V");
	SLIB_JNI_METHOD(read, "read", "(IIII[II)V");
	SLIB_JNI_METHOD(write, "write", "(IIII[II)V");
	SLIB_JNI_METHOD(getCanvas, "getCanvas", "()Lslib/platform/android/ui/Graphics;");
	SLIB_JNI_METHOD(draw, "draw", "(Lslib/platform/android/ui/Graphics;FFFFIIIIFFI)V");
	SLIB_JNI_METHOD(drawWithColorMatrix, "draw", "(Lslib/platform/android/ui/Graphics;FFFFIIIIFFIFFFFFFFFFFFFFFFFFFFF)V");
	SLIB_JNI_STATIC_METHOD(getArrayBuffer, "getArrayBuffer", "()[I");
	SLIB_JNI_STATIC_METHOD(returnArrayBuffer, "returnArrayBuffer", "([I)V");
SLIB_JNI_END_CLASS

class _Android_Bitmap : public Bitmap
{
	SLIB_DECLARE_OBJECT
public:
	JniGlobal<jobject> m_bitmap;

	sl_uint32 m_width;
	sl_uint32 m_height;

	sl_bool m_flagFreeOnRelease;
	Ref<Referable> m_ref;

public:
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

public:
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

    // override
	sl_uint32 getBitmapWidth()
	{
		return m_width;
	}

    // override
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

    // override
	sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& bitmapData)
	{
		return readWritePixels(sl_true, x, y, bitmapData);
	}

    // override
	sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& bitmapData)
	{
		return readWritePixels(sl_false, x, y, *((BitmapData*)&bitmapData));
	}

    // override
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

    // override
	Ref<Canvas> getCanvas()
	{
		JniLocal<jobject> jcanvas = _JAndroidBitmap::getCanvas.callObject(m_bitmap.get());
		if (jcanvas.isNotNull()) {
			return GraphicsPlatform::createCanvas(CanvasType::Bitmap, jcanvas);
		}
		return sl_null;
	}

    // override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
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
		int tileMode = 0;
		if (param.tiled) {
			tileMode = 1;
		}
		if (param.useColorMatrix) {
			const Color4f& cr = param.colorMatrix.red;
			const Color4f& cg = param.colorMatrix.green;
			const Color4f& cb = param.colorMatrix.blue;
			const Color4f& ca = param.colorMatrix.alpha;
			const Color4f& cc = param.colorMatrix.bias;
			_JAndroidBitmap::drawWithColorMatrix.call(m_bitmap, jcanvas,
				rectDst.left, rectDst.top, rectDst.right, rectDst.bottom,
				(sl_int32)(rectSrc.left), (sl_int32)(rectSrc.top), (sl_int32)(rectSrc.right), (sl_int32)(rectSrc.bottom),
				alpha, blur, tileMode,
				cr.x, cr.y, cr.z, cr.w,
				cg.x, cg.y, cg.z, cg.w,
				cb.x, cb.y, cb.z, cb.w,
				ca.x, ca.y, ca.z, ca.w,
				cc.x, cc.y, cc.z, cc.w
				);
		} else {
			_JAndroidBitmap::draw.call(m_bitmap, jcanvas,
				rectDst.left, rectDst.top, rectDst.right, rectDst.bottom,
				(sl_int32)(rectSrc.left), (sl_int32)(rectSrc.top), (sl_int32)(rectSrc.right), (sl_int32)(rectSrc.bottom),
				alpha, blur, tileMode);
		}
	}
};

SLIB_DEFINE_OBJECT(_Android_Bitmap, Bitmap)

Ref<Bitmap> Bitmap::create(sl_uint32 width, sl_uint32 height)
{
	return _Android_Bitmap::create(width, height);
}

Ref<Bitmap> Bitmap::loadFromMemory(const void* mem, sl_size size)
{
	return _Android_Bitmap::load(mem, size);
}

Ref<Drawable> GraphicsPlatform::createImageDrawable(jobject bitmap, sl_bool flagFreeOnRelease, Referable* ref)
{
	return _Android_Bitmap::create(bitmap, flagFreeOnRelease, ref);
}

jobject GraphicsPlatform::getImageDrawableHandle(Drawable* _drawable)
{
	if (_Android_Bitmap* drawable = CastInstance<_Android_Bitmap>(_drawable)) {
		return drawable->m_bitmap;
	}
	return 0;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
