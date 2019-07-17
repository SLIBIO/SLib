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

#include "slib/graphics/font.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace android
		{

			SLIB_JNI_BEGIN_CLASS(JPointF, "android/graphics/PointF")
				SLIB_JNI_FLOAT_FIELD(x);
				SLIB_JNI_FLOAT_FIELD(y);
			SLIB_JNI_END_CLASS

			SLIB_JNI_BEGIN_CLASS(JFontMetrics, "android/graphics/Paint$FontMetrics")
				SLIB_JNI_FLOAT_FIELD(ascent);
				SLIB_JNI_FLOAT_FIELD(descent);
				SLIB_JNI_FLOAT_FIELD(leading);
			SLIB_JNI_END_CLASS

			SLIB_JNI_BEGIN_CLASS(JFont, "slib/platform/android/ui/UiFont")
				SLIB_JNI_STATIC_METHOD(create, "create", "(Ljava/lang/String;FI)Lslib/platform/android/ui/UiFont;");
				SLIB_JNI_METHOD(getFontMetrics, "getFontMetrics", "()Landroid/graphics/Paint$FontMetrics;");
				SLIB_JNI_METHOD(measureText, "measureText", "(Ljava/lang/String;)Landroid/graphics/PointF;");
			SLIB_JNI_END_CLASS

			class FontPlatformObject : public Referable
			{
			public:
				JniGlobal<jobject> m_font;
				
			public:	
				FontPlatformObject(const FontDesc& desc)
				{
					int style = 0;
					if (desc.flagBold) {
						style |= 1;
					}
					if (desc.flagItalic) {
						style |= 2;
					}
					if (desc.flagUnderline) {
						style |= 4;
					}
					if (desc.flagStrikeout) {
						style |= 8;
					}
					float size = (float)(desc.size);
					JniLocal<jstring> fontName = Jni::getJniString(desc.familyName);
					JniLocal<jobject> jfont = JFont::create.callObject(sl_null, fontName.value, size, style);
					if (jfont.isNotNull()) {
						JniGlobal<jobject> gfont = jfont;
						if (gfont.isNotNull()) {
							m_font = gfont;
						}
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

				jobject getPlatformHandle()
				{
					FontPlatformObject* po = getPlatformObject();
					if (po) {
						return po->m_font.get();
					}
					return 0;
				}
			};

		}
	}

	using namespace priv::android;

	sl_bool Font::_getFontMetrics_PO(FontMetrics& _out)
	{
		jobject font = GraphicsPlatform::getNativeFont(this);
		if (font) {
			JniLocal<jobject> fm = JFont::getFontMetrics.callObject(font);
			if (fm.isNotNull()) {
				_out.ascent = -(JFontMetrics::ascent.get(fm.get()));
				_out.descent = JFontMetrics::descent.get(fm.get());
				_out.leading = JFontMetrics::leading.get(fm.get());
				return sl_true;
			}
		}
		return sl_false;
	}

	Size Font::_measureText_PO(const String& text)
	{
		jobject font = GraphicsPlatform::getNativeFont(this);
		if (font) {	
			JniLocal<jstring> jtext = Jni::getJniString(text);
			JniLocal<jobject> size = JFont::measureText.callObject(font, jtext.get());
			Size ret;
			ret.x = JPointF::x.get(size.get());
			ret.y = JPointF::y.get(size.get());
			return ret;
		}
		return Size::zero();
	}

	jobject GraphicsPlatform::getNativeFont(Font* font)
	{
		if (font) {
			return ((FontHelper*)font)->getPlatformHandle();
		}
		return 0;
	}

}

#endif
