/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	
	public:
		FontDesc();

		~FontDesc();

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

		static Ref<Font> create(String familyName, sl_real size, sl_bool flagBold = sl_false, sl_bool flagItalic = sl_false, sl_bool flagUnderline = sl_false);
	
		static sl_real getDefaultFontSize();

		static void setDefaultFontSize(sl_real size);

	public:
		void getDesc(FontDesc& desc);

		String getFamilyName();

		sl_real getSize();

		sl_bool isBold();

		sl_bool isItalic();

		sl_bool isUnderline();

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
