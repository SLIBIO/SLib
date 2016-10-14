#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/image.h"
#include "../../../inc/slib/graphics/platform.h"

#include "../../../inc/slib/core/scoped_pointer.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")

SLIB_GRAPHICS_NAMESPACE_BEGIN

class _Gdiplus_ImageDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
public:
	Gdiplus::Image* m_image;
	sl_bool m_flagFreeOnRelease;
	Ref<Referable> m_ref;
	
public:
	_Gdiplus_ImageDrawable()
	{
	}
	
	~_Gdiplus_ImageDrawable()
	{
		if (m_flagFreeOnRelease) {
			delete m_image;
		}
	}
	
public:
	static Ref<_Gdiplus_ImageDrawable> create(Gdiplus::Image* image, sl_bool flagFreeOnRelease, const Referable* ref)
	{
		if (image) {
			Ref<_Gdiplus_ImageDrawable> ret = new _Gdiplus_ImageDrawable();
			if (ret.isNotNull()) {
				ret->m_image = image;
				ret->m_flagFreeOnRelease = flagFreeOnRelease;
				ret->m_ref = ref;
				return ret;
			}
			if (flagFreeOnRelease) {
				delete image;
			}
		}
		return Ref<_Gdiplus_ImageDrawable>::null();
	}
	
    // override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		GraphicsPlatform::drawImage(canvas, rectDst, m_image, rectSrc, param);
	}
	
    // override
	sl_real getDrawableWidth()
	{
		return (sl_real)(m_image->GetWidth());
	}
	
    // override
	sl_real getDrawableHeight()
	{
		return (sl_real)(m_image->GetHeight());
	}
    
};

SLIB_DEFINE_OBJECT(_Gdiplus_ImageDrawable, Drawable)

Ref<Drawable> PlatformDrawable::create(const ImageDesc& desc)
{
	return Bitmap::create(desc);
}

Ref<Drawable> PlatformDrawable::loadFromMemory(const void* buf, sl_size size)
{
	IStream* stream = ::SHCreateMemStream((BYTE*)buf, (sl_uint32)size);
	if (stream) {
		Gdiplus::Image* image = new Gdiplus::Image(stream);
		stream->Release();
		if (image && image->GetWidth() > 0 && image->GetHeight() > 0) {
			return GraphicsPlatform::createImageDrawable(image, sl_true);
		}
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> GraphicsPlatform::createImageDrawable(Gdiplus::Image* image, sl_bool flagFreeOnRelease, const Referable* ref)
{
	return _Gdiplus_ImageDrawable::create(image, flagFreeOnRelease, ref);
}

Gdiplus::Image* GraphicsPlatform::getImageHandle(Drawable* _drawable)
{
	if (_Gdiplus_ImageDrawable::checkInstance(_drawable)) {
		_Gdiplus_ImageDrawable* drawable = (_Gdiplus_ImageDrawable*)_drawable;
		return drawable->m_image;
	}
	if (Bitmap::checkInstance(_drawable)) {
		return GraphicsPlatform::getBitmapHandle((Bitmap*)_drawable);
	}
	return NULL;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
