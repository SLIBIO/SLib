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
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace cairo
		{
			
			class FontPlatformObject : public Referable
			{
			public:
				PangoFontDescription* m_font;

			public:
				FontPlatformObject(const FontDesc& desc)
				{
					PangoFontDescription* font = pango_font_description_new();
					if (font) {
						pango_font_description_set_family(font, desc.familyName.getData());
						if (desc.flagBold) {
							pango_font_description_set_weight(font, PANGO_WEIGHT_BOLD);
						}
						if (desc.flagItalic) {
							pango_font_description_set_style(font, PANGO_STYLE_ITALIC);
						}
						pango_font_description_set_absolute_size (font, desc.size * PANGO_SCALE);
					}
					m_font = font;
				}

				~FontPlatformObject()
				{
					if (m_font) {
						pango_font_description_free(m_font);
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

				PangoFontDescription* getPlatformHandle()
				{
					FontPlatformObject* po = getPlatformObject();
					if (po) {
						return po->m_font;
					}
					return 0;
				}
			};

			class StaticContext
			{
			public:
				cairo_surface_t* surface;
				cairo_t* cairo;
				PangoContext* pango;

			public:
				StaticContext()
				{
					surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 32, 32);
					cairo = cairo_create(surface);
					pango = pango_cairo_create_context(cairo);
				}

				~StaticContext()
				{
					cairo_surface_destroy(surface);
					cairo_destroy(cairo);
					g_object_unref(pango);
				}

			};

			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext)

		}
	}

	using namespace priv::cairo;

	sl_bool Font::_getFontMetrics_PO(FontMetrics& _out)
	{
		StaticContext* context = GetStaticContext();
		if (context) {
			PangoFontDescription* handle = GraphicsPlatform::getCairoFont(this);
			if (handle) {
				PangoFontMetrics* metrics = pango_context_get_metrics(context->pango, handle, NULL);
				if (metrics) {
					_out.ascent = (sl_real)(pango_font_metrics_get_ascent(metrics));
					_out.descent = (sl_real)(pango_font_metrics_get_descent(metrics));
					_out.leading = 0;
					pango_font_metrics_unref(metrics);
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	Size Font::_measureText_PO(const StringParam& _text)
	{
		StringData text(_text);
		sl_size len = text.getLength();
		if (len) {
			StaticContext* context = GetStaticContext();
			if (context) {
				PangoFontDescription* handle = GraphicsPlatform::getCairoFont(this);
				if (handle) {
					PangoLayout* layout = pango_layout_new(context->pango);
					if (layout) {
						pango_layout_set_font_description(layout, handle);
						pango_layout_set_text(layout, text.getData(), len);
						int w = 0;
						int h = 0;
						pango_layout_get_pixel_size(layout, &w, &h);
						return Size((sl_real)w, (sl_real)h);
					}
				}
			}
		}
		return Size::zero();
	}

	PangoFontDescription* GraphicsPlatform::getCairoFont(Font* font)
	{
		if (font) {
			return ((FontHelper*)font)->getPlatformHandle();
		}
		return 0;
	}

}

#endif
