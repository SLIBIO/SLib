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

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/graphics/drawable.h"

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"
#include "slib/core/scoped.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")

namespace slib
{

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
		static Ref<_Gdiplus_ImageDrawable> create(Gdiplus::Image* image, sl_bool flagFreeOnRelease, Referable* ref)
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
			return sl_null;
		}
		
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override
		{
			GraphicsPlatform::drawImage(canvas, rectDst, m_image, rectSrc, param);
		}
		
		sl_real getDrawableWidth() override
		{
			return (sl_real)(m_image->GetWidth());
		}
		
		sl_real getDrawableHeight() override
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
		return sl_null;
	}

	Ref<Drawable> GraphicsPlatform::createImageDrawable(Gdiplus::Image* image, sl_bool flagFreeOnRelease, Referable* ref)
	{
		return _Gdiplus_ImageDrawable::create(image, flagFreeOnRelease, ref);
	}

	Gdiplus::Image* GraphicsPlatform::getImageHandle(Drawable* _drawable)
	{
		if (_Gdiplus_ImageDrawable* drawable = CastInstance<_Gdiplus_ImageDrawable>(_drawable)) {
			return drawable->m_image;
		}
		if (Bitmap* bitmap = CastInstance<Bitmap>(_drawable)) {
			return GraphicsPlatform::getBitmapHandle(bitmap);
		}
		return NULL;
	}

}

#endif
