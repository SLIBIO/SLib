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

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "slib/graphics/font.h"

#include "slib/graphics/platform.h"

namespace slib
{

	sl_bool Font::_getFontMetrics_PO(FontMetrics& _out)
	{
		cairo_scaled_font_t* handle = GraphicsPlatform::getCairoFont(this);
		if (handle) {
			cairo_font_extents_t extents;
			::cairo_scaled_font_extents(handle, &extents);
			_out.ascent = (sl_real)(extents.ascent);
			_out.descent = (sl_real)(extents.descent);
			_out.leading = (sl_real)(extents.height - extents.ascent - extents.descent);
			return sl_true;
		}
		return sl_false;
	}

	Size Font::_measureText_PO(const String& text)
	{
		cairo_scaled_font_t* handle = GraphicsPlatform::getCairoFont(this);
		if (handle) {
			cairo_text_extents_t extents;
			::cairo_scaled_font_text_extents(handle, text.getData(), &extents);
			return Size((sl_real)(extents.x_advance), getFontHeight());
		}
		return Size::zero();
	}

	class Cairo_FontObject : public Referable
	{
	public:
		cairo_scaled_font_t* m_font;

	public:
		Cairo_FontObject(const FontDesc& desc)
		{
			cairo_scaled_font_t* font = sl_null;
			cairo_font_slant_t slant = desc.flagItalic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL;
			cairo_font_weight_t weight = desc.flagBold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL;
			cairo_font_face_t* face = ::cairo_toy_font_face_create(desc.familyName.getData(), slant, weight);
			if (face) {
				cairo_font_options_t* options = ::cairo_font_options_create();
				if (options) {
					cairo_matrix_t mat;
					::cairo_matrix_init_scale(&mat, desc.size, desc.size);
					cairo_matrix_t identity;
					::cairo_matrix_init_identity(&identity);
					font = ::cairo_scaled_font_create(face, &mat, &identity, options);
					::cairo_font_options_destroy(options);
				}
				::cairo_font_face_destroy(face);
			}
			m_font = font;
		}

		~Cairo_FontObject()
		{
			if (m_font) {
				::cairo_scaled_font_destroy(m_font);
			}
		}

	};

	class Font_Ext : public Font
	{
	public:
		Cairo_FontObject* getPlatformObject()
		{
			if (m_platformObject.isNull()) {
				SpinLocker lock(&m_lock);
				if (m_platformObject.isNull()) {
					m_platformObject = new Cairo_FontObject(m_desc);
				}
			}
			return (Cairo_FontObject*)(m_platformObject.get());;
		}

		cairo_scaled_font_t* getPlatformHandle()
		{
			Cairo_FontObject* po = getPlatformObject();
			if (po) {
				return po->m_font;
			}
			return 0;
		}
	};

	cairo_scaled_font_t* GraphicsPlatform::getCairoFont(Font* font)
	{
		if (font) {
			return ((Font_Ext*)font)->getPlatformHandle();
		}
		return 0;
	}

}

#endif
