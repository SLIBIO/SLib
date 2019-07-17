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

#if defined(SLIB_GRAPHICS_IS_CAIRO)

#include "slib/graphics/font.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace cairo
		{
			
			class FontPlatformObject : public Referable
			{
			public:
				cairo_scaled_font_t* m_font;

			public:
				FontPlatformObject(const FontDesc& desc)
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

				~FontPlatformObject()
				{
					if (m_font) {
						::cairo_scaled_font_destroy(m_font);
					}
				}

			};

			class FontHelper : public Font
			{
			public:
				FontPlatformObject* getPlatformObject()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new FontPlatformObject(m_desc);
						}
					}
					return (FontPlatformObject*)(m_platformObject.get());;
				}

				cairo_scaled_font_t* getPlatformHandle()
				{
					FontPlatformObject* po = getPlatformObject();
					if (po) {
						return po->m_font;
					}
					return 0;
				}
			};

		}
	}

	using namespace priv::cairo;

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

	cairo_scaled_font_t* GraphicsPlatform::getCairoFont(Font* font)
	{
		if (font) {
			return ((FontHelper*)font)->getPlatformHandle();
		}
		return 0;
	}

}

#endif
