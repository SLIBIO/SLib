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

#ifndef CHECKHEADER_SLIB_GRAPHICS_FONT
#define CHECKHEADER_SLIB_GRAPHICS_FONT

#include "definition.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../math/size.h"

#define SLIB_FONT_SIZE_PRECISION_MULTIPLIER 10
#define SLIB_FONT_SIZE_PRECISION_APPLY(f) ((sl_real)((sl_int32)((f) * SLIB_FONT_SIZE_PRECISION_MULTIPLIER + 0.5f)) / SLIB_FONT_SIZE_PRECISION_MULTIPLIER)

namespace slib
{
	
	class SLIB_EXPORT FontDesc
	{
	public:
		String familyName;
		sl_real size;
		sl_bool flagBold;
		sl_bool flagItalic;
		sl_bool flagUnderline;
		sl_bool flagStrikeout;
	
	public:
		FontDesc();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(FontDesc)

	};
	
	struct SLIB_EXPORT FontMetrics
	{
		sl_real ascent;
		sl_real descent;
		sl_real leading;
	};
	
	class FontAtlas;
	
	class SLIB_EXPORT Font : public Referable
	{
		SLIB_DECLARE_OBJECT

	protected:
		Font();

		~Font();

	public:
		static Ref<Font> getDefault();

		static Ref<Font> create(const FontDesc& desc);

		static Ref<Font> create(String familyName, sl_real size, sl_bool flagBold = sl_false, sl_bool flagItalic = sl_false, sl_bool flagUnderline = sl_false, sl_bool flagStrikeout = sl_false);
	
		static sl_real getDefaultFontSize();

		static void setDefaultFontSize(sl_real size);

	public:
		void getDesc(FontDesc& desc);

		String getFamilyName();

		sl_real getSize();

		sl_bool isBold();

		sl_bool isItalic();

		sl_bool isUnderline();
		
		sl_bool isStrikeout();

		sl_bool getFontMetrics(FontMetrics& _out);

		sl_real getFontHeight();

		sl_real getFontAscent();

		sl_real getFontDescent();

		Size measureSingleLineText(const String& text);

		Size measureMultiLineText(const String16& text);

		Size measureText(const String& text, sl_bool flagMultiLine = sl_false);

		Size measureText16(const String16& text, sl_bool flagMultiLine = sl_false);

		Ref<FontAtlas> getAtlas();

		Ref<FontAtlas> getSharedAtlas();

		Ref<Referable> getPlatformObject();

	private:
		sl_bool _getFontMetrics_PO(FontMetrics& _out);

		Size _measureText_PO(const String& text);

	protected:
		FontDesc m_desc;
		FontMetrics m_metricsCache;
		sl_bool m_flagMetricsCache;

		AtomicRef<FontAtlas> m_fontAtlas;

		Ref<Referable> m_platformObject;
		SpinLock m_lock;

	};

}

#endif
