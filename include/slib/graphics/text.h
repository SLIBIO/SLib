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
		Emoji = 50,
		Attach = 100
	};
	
	typedef sl_size sl_text_pos;
	
#define SLIB_TEXT_RANGE_NOT_FOUND SLIB_SIZE_MAX
	
	class SLIB_EXPORT TextRange
	{
	public:
		sl_text_pos location;
		sl_text_pos length;
		
	public:
		SLIB_INLINE constexpr TextRange() noexcept : location(SLIB_TEXT_RANGE_NOT_FOUND), length(0) {}
		
		SLIB_INLINE constexpr TextRange(sl_null_t) noexcept : location(SLIB_TEXT_RANGE_NOT_FOUND), length(0) {}
		
		SLIB_INLINE constexpr TextRange(sl_text_pos _location, sl_text_pos _length) noexcept : location(_location), length(_length) {}
		
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(TextRange)
		
	public:
		
		SLIB_INLINE constexpr sl_bool operator==(const TextRange& other) const noexcept
		{
			return location == other.location && length == other.length;
		}
		
		SLIB_INLINE constexpr sl_bool operator!=(const TextRange& other) const noexcept
		{
			return location != other.location || length != other.length;
		}
		
		SLIB_INLINE constexpr sl_bool isNotFound() const noexcept
		{
			return location == SLIB_TEXT_RANGE_NOT_FOUND;
		}
		
	};
	
	class SLIB_EXPORT TextStyle : public Referable
	{
	public:
		AtomicRef<Font> font;
		AtomicString emojiFamilyName;
		sl_bool flagUnderline;
		sl_bool flagOverline;
		sl_bool flagLineThrough;
		Color textColor;
		Color backgroundColor;
		String href;
		sl_real lineHeight;
		sl_real yOffset;
		
	public:
		TextStyle() noexcept;
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(TextStyle)
		
	public:
		Ref<TextStyle> duplicate() const noexcept;

	};
	
	class SLIB_EXPORT TextItem : public Object
	{
		SLIB_DECLARE_OBJECT

	protected:
		TextItem(TextItemType type) noexcept;

		~TextItem() noexcept;
	
	public:
		TextItemType getType() noexcept;
		
		Ref<TextStyle> getStyle() noexcept;
		
		void setStyle(const Ref<TextStyle>& style) noexcept;
		
		Ref<Font> getFont() noexcept;
	
		Ref<Font> getEmojiFont() noexcept;
		
		Point getLayoutPosition() noexcept;

		void setLayoutPosition(const Point& pt) noexcept;

		Size getLayoutSize() noexcept;

		void setLayoutSize(const Size& size) noexcept;

		Rectangle getLayoutFrame() noexcept;

	protected:
		TextItemType m_type;
		AtomicRef<TextStyle> m_style;
		Point m_layoutPosition;
		Size m_layoutSize;
		AtomicRef<Font> m_emojiFont;
		AtomicRef<Font> m_emojiFontBase;

	};
	
	class SLIB_EXPORT TextWordItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	private:
		TextWordItem() noexcept;

		~TextWordItem() noexcept;

	public:
		static Ref<TextWordItem> create(const String16& text, const Ref<TextStyle>& style) noexcept;

	public:
		String16 getText() noexcept;

		void setText(const String16& text) noexcept;

		Size getSize() noexcept;
		
		void draw(Canvas* canvas, sl_real x, sl_real y, const Color& color);
		
	private:
		AtomicString16 m_text;

		Ref<Font> m_fontCached;
		String16 m_textCached;
		sl_real m_widthCached;
		sl_real m_heightCached;

	};
	
	class SLIB_EXPORT TextEmojiItem : public TextItem
	{
		SLIB_DECLARE_OBJECT
		
	private:
		TextEmojiItem() noexcept;
		
		~TextEmojiItem() noexcept;
		
	public:
		static Ref<TextEmojiItem> create(const String16& text, const Ref<TextStyle>& style) noexcept;
		
	public:
		Size getSize() noexcept;
		
		void draw(Canvas* canvas, sl_real x, sl_real y, const Color& color);
		
	private:
		String16 m_text;
		
		Ref<Font> m_fontCached;
		sl_real m_widthCached;
		sl_real m_heightCached;

	};
	
	class SLIB_EXPORT TextSpaceItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	private:
		TextSpaceItem() noexcept;

		~TextSpaceItem() noexcept;

	public:
		static Ref<TextSpaceItem> create(const Ref<TextStyle>& style) noexcept;

	public:
		Size getSize() noexcept;

	};
	
	class SLIB_EXPORT TextTabItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	private:
		TextTabItem() noexcept;

		~TextTabItem() noexcept;

	public:
		static Ref<TextTabItem> create(const Ref<TextStyle>& style) noexcept;

	public:
		sl_real getHeight() noexcept;

	};
	
	class SLIB_EXPORT TextLineBreakItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	private:
		TextLineBreakItem() noexcept;

		~TextLineBreakItem() noexcept;

	public:
		static Ref<TextLineBreakItem> create(const Ref<TextStyle>& style) noexcept;

	public:
		sl_real getHeight() noexcept;

	};
	
	
	class SLIB_EXPORT TextAttachItem : public TextItem
	{
		SLIB_DECLARE_OBJECT

	public:
		TextAttachItem() noexcept;

		~TextAttachItem() noexcept;

	public:
		virtual Size getSize() noexcept = 0;

		virtual void setPosition(const Point& pos) noexcept = 0;

	};
	
	class TextParagraphLayoutParam
	{
	public:
		sl_real width;
		sl_real tabWidth;
		sl_real tabMargin;
		Alignment align;
		MultiLineMode multiLineMode;
		EllipsizeMode ellipsisMode;
		sl_uint32 linesCount;

	public:
		TextParagraphLayoutParam() noexcept;

		~TextParagraphLayoutParam() noexcept;

	};
	
	class XmlNodeGroup;
	class XmlElement;
	
	class SLIB_EXPORT TextParagraph : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		TextParagraph() noexcept;

		~TextParagraph() noexcept;

	public:
		void addText(const String16& text, const Ref<TextStyle>& style) noexcept;
		
		void addHyperTextNodeGroup(const Ref<XmlNodeGroup>& group, const Ref<TextStyle>& style) noexcept;
		
		void addHyperTextElement(const Ref<XmlElement>& element, const Ref<TextStyle>& style) noexcept;
		
		void addHyperText(const String16& text, const Ref<TextStyle>& style) noexcept;

		void layout(const TextParagraphLayoutParam& param) noexcept;

		void draw(Canvas* canvas, sl_real x, sl_real y, const Color& colorDefault) noexcept;
		
		Ref<TextItem> getTextItemAtPosition(sl_real x, sl_real y) noexcept;

		sl_real getMaximumWidth() noexcept;

		sl_real getTotalHeight() noexcept;
		
		sl_real getPositionLength() noexcept;

	protected:
		CList< Ref<TextItem> > m_items;
		CList< Ref<TextItem> > m_layoutItems;
		sl_real m_maxWidth;
		sl_real m_totalHeight;
		sl_real m_positionLength;

	};
	
	class SLIB_EXPORT SimpleTextBoxParam
	{
	public:
		Ref<Font> font;
		String text;
		sl_bool flagHyperText;
		sl_real width;
		MultiLineMode multiLineMode;
		EllipsizeMode ellipsizeMode;
		sl_uint32 linesCount;
		Alignment align;
		
	public:
		SimpleTextBoxParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(SimpleTextBoxParam)
		
	};
	
	class SLIB_EXPORT SimpleTextBoxDrawParam
	{
	public:
		Rectangle frame;
		Color color;
		
	public:
		SimpleTextBoxDrawParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(SimpleTextBoxDrawParam)

	};
	
	class SLIB_EXPORT SimpleTextBox : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		SimpleTextBox() noexcept;

		~SimpleTextBox();

	public:
		void update(const SimpleTextBoxParam& param) noexcept;

		void draw(Canvas* canvas, const SimpleTextBoxDrawParam& param) const noexcept;

		Ref<TextItem> getTextItemAtPosition(sl_real x, sl_real y) const noexcept;
		
		sl_real getContentWidth() const noexcept;

		sl_real getContentHeight() const noexcept;
		
	public:
		Ref<Font> getFont() const noexcept;
		
		String getText() const noexcept;
		
		MultiLineMode getMultiLineMode() const noexcept;
		
		EllipsizeMode getEllipsizeMode() const noexcept;
		
		Alignment getAlignment() const noexcept;

	protected:
		Ref<TextParagraph> m_paragraph;
		Ref<TextStyle> m_style;

		Ref<Font> m_font;
		String m_text;
		sl_bool m_flagHyperText;
		MultiLineMode m_multiLineMode;
		EllipsizeMode m_ellipsisMode;
		sl_uint32 m_linesCount;
		Alignment m_alignHorizontal;
		Alignment m_alignVertical;
		sl_real m_width;

		sl_real m_contentWidth;
		sl_real m_contentHeight;

	};

}

#endif
