/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_TEXT
#define CHECKHEADER_SLIB_GRAPHICS_TEXT

#include "definition.h"

#include "constants.h"
#include "canvas.h"

#include "../core/string.h"
#include "../core/function.h"
#include "../math/rectangle.h"

namespace slib
{

	enum class TextItemType
	{
		Word = 0,
		Space = 10,
		Tab = 11,
		LineBreak = 20,
		Attach = 100
	};
	
	class SLIB_EXPORT TextStyle : public Referable
	{
	public:
		AtomicRef<Font> font;
		Color textColor;
		Color backgroundColor;
		
	public:
		TextStyle();
		
		~TextStyle();

	};
	
	class SLIB_EXPORT TextItem : public Object
	{
		SLIB_DECLARE_OBJECT

	protected:
		TextItem(TextItemType type);

		~TextItem();
	
	public:
		TextItemType getType();
		
		Ref<TextStyle> getStyle();
		
		void setStyle(const Ref<TextStyle>& style);
		
		Ref<Font> getFont();
	
		Point getLayoutPosition();

		void setLayoutPosition(const Point& pt);

		Size getLayoutSize();

		void setLayoutSize(const Size& size);

		Rectangle getLayoutFrame();

	protected:
		TextItemType m_type;
		AtomicRef<TextStyle> m_style;
		Point m_layoutPosition;
		Size m_layoutSize;

	};
	
	class SLIB_EXPORT TextWordItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	private:
		TextWordItem();

		~TextWordItem();

	public:
		static Ref<TextWordItem> create(const String16& text, const Ref<TextStyle>& style);

	public:
		String16 getText();

		void setText(const String16& text);

		Size getSize();
		
	private:
		AtomicString16 m_text;

		Ref<Font> m_fontCached;
		String16 m_textCached;
		sl_real m_widthCached;
		sl_real m_heightCached;

	};
	
	class SLIB_EXPORT TextSpaceItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	private:
		TextSpaceItem();

		~TextSpaceItem();

	public:
		static Ref<TextSpaceItem> create(const Ref<TextStyle>& style);

	public:
		Size getSize();

	};
	
	class SLIB_EXPORT TextTabItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	private:
		TextTabItem();

		~TextTabItem();

	public:
		static Ref<TextTabItem> create(const Ref<TextStyle>& style);

	public:
		sl_real getHeight();

	};
	
	class SLIB_EXPORT TextLineBreakItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	private:
		TextLineBreakItem();

		~TextLineBreakItem();

	public:
		static Ref<TextLineBreakItem> create(const Ref<TextStyle>& style);

	public:
		sl_real getHeight();

	};
	
	
	class SLIB_EXPORT TextAttachItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	public:
		TextAttachItem();

		~TextAttachItem();

	public:
		virtual Size getSize() = 0;

		virtual void setPosition(const Point& pos) = 0;

	};
	
	class TextParagraphLayoutParam
	{
	public:
		sl_real width;
		sl_real tabWidth;
		sl_real tabMargin;
		Alignment align;
		MultiLineMode multiLineMode;

	public:
		TextParagraphLayoutParam();

		~TextParagraphLayoutParam();

	};
	
	class SLIB_EXPORT TextParagraph : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		TextParagraph();

		~TextParagraph();

	public:
		void addText(const String16& text, const Ref<TextStyle>& style);

		void layout(const TextParagraphLayoutParam& param);

		void draw(Canvas* canvas, sl_real x, sl_real y);

		sl_real getMaximumWidth();

		sl_real getTotalHeight();

	protected:
		CList< Ref<TextItem> > m_items;
		CList< Ref<TextItem> > m_layoutItems;
		sl_real m_maxWidth;
		sl_real m_totalHeight;

	};
	
	class SLIB_EXPORT SimpleTextBox : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		SimpleTextBox();

		~SimpleTextBox();

	public:
		void update(const String& text, const Ref<Font>& font, sl_real width, sl_bool flagWrappingWidth, MultiLineMode multiLineMode, const Alignment& align);

		void draw(Canvas* canvas, const String& text, const Ref<Font>& font, const Rectangle& frame, sl_bool flagWrappingWidth, MultiLineMode multiLineMode, const Alignment& align, const Color& color);

		sl_real getContentWidth();

		sl_real getContentHeight();

	protected:
		Ref<TextParagraph> m_paragraph;

		String m_text;
		Ref<TextStyle> m_style;
		Ref<Font> m_font;
		sl_real m_width;
		MultiLineMode m_multiLineMode;
		Alignment m_align;

		sl_real m_contentWidth;
		sl_real m_contentHeight;

	};

}

#endif
