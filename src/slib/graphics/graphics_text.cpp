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

#include "slib/graphics/text.h"

#include "slib/graphics/font_atlas.h"

namespace slib
{

	TextStyle::TextStyle() noexcept : textColor(Color::Zero), backgroundColor(Color::Zero)
	{
	}
	
	TextStyle::~TextStyle() noexcept
	{
	}
	
	SLIB_DEFINE_OBJECT(TextItem, Object)

	TextItem::TextItem(TextItemType type) noexcept
	 : m_type(type), m_layoutPosition(0, 0), m_layoutSize(0, 0)
	{
	}

	TextItem::~TextItem() noexcept
	{
	}

	TextItemType TextItem::getType() noexcept
	{
		return m_type;
	}
	
	Ref<TextStyle> TextItem::getStyle() noexcept
	{
		return m_style;
	}
	
	void TextItem::setStyle(const Ref<TextStyle>& style) noexcept
	{
		m_style = style;
	}
	
	Ref<Font> TextItem::getFont() noexcept
	{
		Ref<TextStyle> style = m_style;
		if (style.isNotNull()) {
			return style->font;
		}
		return sl_null;
	}

	Point TextItem::getLayoutPosition() noexcept
	{
		return m_layoutPosition;
	}

	void TextItem::setLayoutPosition(const Point& pt) noexcept
	{
		m_layoutPosition = pt;
	}

	Size TextItem::getLayoutSize() noexcept
	{
		return m_layoutSize;
	}

	void TextItem::setLayoutSize(const Size& size) noexcept
	{
		m_layoutSize = size;
	}

	Rectangle TextItem::getLayoutFrame() noexcept
	{
		return Rectangle(m_layoutPosition.x, m_layoutPosition.y, m_layoutPosition.x + m_layoutSize.x, m_layoutPosition.y + m_layoutSize.y);
	}


	SLIB_DEFINE_OBJECT(TextWordItem, TextItem)

	TextWordItem::TextWordItem() noexcept
	 : TextItem(TextItemType::Word)
	{
		m_widthCached = 0;
		m_heightCached = 0;
	}

	TextWordItem::~TextWordItem() noexcept
	{
	}

	Ref<TextWordItem> TextWordItem::create(const String16& text, const Ref<TextStyle>& style) noexcept
	{
		if (style.isNotNull()) {
			Ref<TextWordItem> ret = new TextWordItem;
			if (ret.isNotNull()) {
				ret->m_text = text;
				ret->m_style = style;
				return ret;
			}
		}
		return sl_null;
	}

	String16 TextWordItem::getText() noexcept
	{
		return m_text;
	}

	void TextWordItem::setText(const String16& text) noexcept
	{
		m_text = text;
	}

	Size TextWordItem::getSize() noexcept
	{
		ObjectLocker lock(this);
		
		String16 text = m_text;
		Ref<Font> font = getFont();
		if (m_textCached == text && m_fontCached == font) {
			return Size(m_widthCached, m_heightCached);
		}
		if (font.isNotNull()) {
			Ref<FontAtlas> atlas = font->getSharedAtlas();
			if (atlas.isNotNull()) {
				Size size = atlas->measureText(text, sl_false);
				m_textCached = text;
				m_fontCached = font;
				m_widthCached = size.x;
				m_heightCached = size.y;
				return size;
			}
		}
		m_widthCached = 0;
		m_heightCached = 0;
		return Size::zero();
	}
	
	
	SLIB_DEFINE_OBJECT(TextSpaceItem, TextItem)

	TextSpaceItem::TextSpaceItem() noexcept
	 : TextItem(TextItemType::Space)
	{
	}

	TextSpaceItem::~TextSpaceItem() noexcept
	{
	}

	Ref<TextSpaceItem> TextSpaceItem::create(const Ref<TextStyle>& style) noexcept
	{
		if (style.isNotNull()) {
			Ref<TextSpaceItem> ret = new TextSpaceItem;
			if (ret.isNotNull()) {
				ret->m_style = style;
				return ret;
			}
		}
		return sl_null;
	}

	Size TextSpaceItem::getSize() noexcept
	{
		Ref<Font> font = getFont();
		if (font.isNotNull()) {
			sl_real h = font->getFontHeight();
			return Size(h * 0.3f, h);
		} else {
			return Size::zero();
		}
	}


	SLIB_DEFINE_OBJECT(TextTabItem, TextItem)

	TextTabItem::TextTabItem() noexcept
	 : TextItem(TextItemType::Tab)
	{
	}

	TextTabItem::~TextTabItem() noexcept
	{
	}

	Ref<TextTabItem> TextTabItem::create(const Ref<TextStyle>& style) noexcept
	{
		if (style.isNotNull()) {
			Ref<TextTabItem> ret = new TextTabItem;
			if (ret.isNotNull()) {
				ret->m_style = style;
				return ret;
			}
		}
		return sl_null;
	}

	sl_real TextTabItem::getHeight() noexcept
	{
		Ref<Font> font = getFont();
		if (font.isNotNull()) {
			return font->getFontHeight();
		} else {
			return 0;
		}
	}


	SLIB_DEFINE_OBJECT(TextLineBreakItem, TextItem)

	TextLineBreakItem::TextLineBreakItem() noexcept
	 : TextItem(TextItemType::LineBreak)
	{
	}

	TextLineBreakItem::~TextLineBreakItem() noexcept
	{
	}

	Ref<TextLineBreakItem> TextLineBreakItem::create(const Ref<TextStyle>& style) noexcept
	{
		if (style.isNotNull()) {
			Ref<TextLineBreakItem> ret = new TextLineBreakItem;
			if (ret.isNotNull()) {
				ret->m_style = style;
				return ret;
			}
		}
		return sl_null;
	}

	sl_real TextLineBreakItem::getHeight() noexcept
	{
		Ref<Font> font = getFont();
		if (font.isNotNull()) {
			return font->getFontHeight();
		} else {
			return 0;
		}
	}


	SLIB_DEFINE_OBJECT(TextAttachItem, TextItem)

	TextAttachItem::TextAttachItem() noexcept
	 : TextItem(TextItemType::Attach)
	{
	}

	TextAttachItem::~TextAttachItem() noexcept
	{
	}


	TextParagraphLayoutParam::TextParagraphLayoutParam() noexcept
	{
		width = 1;
		tabWidth = 1;
		tabMargin = 1;
		align = Alignment::Left;
		multiLineMode = MultiLineMode::Single;
	}

	TextParagraphLayoutParam::~TextParagraphLayoutParam() noexcept
	{
	}

	SLIB_DEFINE_OBJECT(TextParagraph, Object)

	TextParagraph::TextParagraph() noexcept
	{
		m_maxWidth = 0;
		m_totalHeight = 0;
		
		m_positionLength = 0;
	}

	TextParagraph::~TextParagraph() noexcept
	{
	}

	void TextParagraph::addText(const String16& text, const Ref<TextStyle>& style) noexcept
	{
		if (text.isEmpty()) {
			return;
		}
		if (style.isNull()) {
			return;
		}

		ObjectLocker lock(this);
		
		sl_char16* sz = text.getData();
		sl_size len = text.getLength();
		sl_size startWord = 0;
		sl_size pos = 0;
		while (pos < len) {
			sl_char16 ch = sz[pos];
			if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
				if (startWord < pos) {
					Ref<TextWordItem> item = TextWordItem::create(String16(sz + startWord, pos - startWord), style);
					if (item.isNotNull()) {
						m_items.add_NoLock(item);
						m_positionLength += pos - startWord;
					}
				}
				if (ch == ' ') {
					Ref<TextSpaceItem> item = TextSpaceItem::create(style);
					if (item.isNotNull()) {
						m_items.add_NoLock(item);
						m_positionLength++;
					}
				} else if (ch == '\t') {
					Ref<TextTabItem> item = TextTabItem::create(style);
					if (item.isNotNull()) {
						m_items.add_NoLock(item);
						m_positionLength++;
					}
				} else if (ch == '\r' || ch == '\n') {
					Ref<TextLineBreakItem> item = TextLineBreakItem::create(style);
					if (item.isNotNull()) {
						m_items.add_NoLock(item);
						m_positionLength++;
					}
					if (ch == '\r' && pos + 1 < len) {
						if (sz[pos + 1] == '\n') {
							pos++;
						}
					}
				}
				startWord = pos + 1;
			}
			pos++;
		}
		if (startWord == 0) {
			Ref<TextWordItem> item = TextWordItem::create(text, style);
			if (item.isNotNull()) {
				m_items.add_NoLock(item);
				m_positionLength += len;
			}
		} else if (startWord < len) {
			Ref<TextWordItem> item = TextWordItem::create(String16(sz + startWord, len - startWord), style);
			if (item.isNotNull()) {
				m_items.add_NoLock(item);
				m_positionLength += len - startWord;
			}
		}
	}

	class _priv_TextParagraph_Layouter
	{
	public:
		CList< Ref<TextItem> >* m_layoutItems;
		sl_real m_layoutWidth;
		Alignment m_align;
		MultiLineMode m_multiLineMode;
		sl_real m_tabMargin;
		sl_real m_tabWidth;
		
		sl_bool m_flagEnd;
		sl_real m_x;
		sl_real m_y;
		
		CList< Ref<TextItem> > m_lineItems;
		sl_real m_lineWidth;
		sl_real m_lineHeight;
		
		sl_real m_maxWidth;
		
	public:
		_priv_TextParagraph_Layouter(CList< Ref<TextItem> >* layoutItems, const TextParagraphLayoutParam& param) noexcept
		{
			m_layoutItems = layoutItems;
			m_layoutWidth = param.width;
			m_align = param.align & Alignment::HorizontalMask;
			m_multiLineMode = param.multiLineMode;
			m_tabWidth = param.tabWidth;
			m_tabMargin = param.tabMargin;
			
			m_flagEnd = sl_false;
			m_x = 0;
			m_y = 0;

			m_lineWidth = 0;
			m_lineHeight = 0;
			m_maxWidth = 0;
		}
		
		void endLine() noexcept
		{
			sl_size n = m_lineItems.getCount();
			if (n == 0) {
				return;
			}
			sl_real x;
			if (m_align == Alignment::Center) {
				x = (m_layoutWidth - m_lineWidth) / 2;
			} else if (m_align == Alignment::Right) {
				x = m_layoutWidth - m_lineWidth;
			} else {
				x = 0;
			}
			sl_real bottom = m_y + m_lineHeight;
			Ref<TextItem>* p = m_lineItems.getData();
			for (sl_size i = 0; i < n; i++) {
				TextItem* item = p[i].get();
				Size size = item->getLayoutSize();
				Point pt(x, bottom - size.y);
				item->setLayoutPosition(pt);
				TextItemType type = item->getType();
				if (type == TextItemType::Word) {
					m_layoutItems->add_NoLock(item);
				} else if (type == TextItemType::Attach) {
					(static_cast<TextAttachItem*>(item))->setPosition(pt);
				}
				x += size.x;
			}
			m_lineItems.removeAll_NoLock();
			if (m_lineWidth > m_maxWidth) {
				m_maxWidth = m_lineWidth;
			}
			m_x = 0;
			m_lineWidth = 0;
			m_y = bottom;
			m_lineHeight = 0;
		}
		
		void breakWord(TextWordItem* breakItem) noexcept
		{
			Ref<TextStyle> style = breakItem->getStyle();
			if (style.isNull()) {
				return;
			}
			
			Ref<Font> font = style->font;
			if (font.isNull()) {
				return;
			}
			
			String16 text = breakItem->getText();
			if (text.isEmpty()) {
				return;
			}
			
			Ref<FontAtlas> atlas = font->getSharedAtlas();
			if (atlas.isNull()) {
				return;
			}
			
			ObjectLocker lockAtlas(atlas.get());
			
			sl_char16* sz = text.getData();
			sl_size len = text.getLength();
			
			sl_real widthRemaining = m_layoutWidth - m_x;
			
			Size size = atlas->getFontSize_NoLock(sz[0]);
			sl_real x = size.x;
			sl_real height = size.y;
			sl_size startLine = 0;
			sl_size pos = 1;
			
			if (size.x > widthRemaining && m_x > 0) {
				endLine();
				widthRemaining = m_layoutWidth;
				if (len > 1) {
					size = atlas->getFontSize_NoLock(sz[1]);
					x = size.x;
					height = size.y;
					startLine = 1;
					pos = 2;
				}
			}

			while (pos < len) {
				size = atlas->getFontSize_NoLock(sz[pos]);
				if (pos > startLine && x + size.x > widthRemaining) {
					Ref<TextWordItem> newItem = TextWordItem::create(String(sz + startLine, pos - startLine), style);
					if (newItem.isNotNull()) {
						newItem->setLayoutSize(Size(x, height));
						m_lineItems.add(newItem);
					}
					startLine = pos;
					m_x += x;
					m_lineWidth = m_x;
					if (height > m_lineHeight) {
						m_lineHeight = height;
					}
					endLine();
					x = 0;
					height = 0;
					widthRemaining = m_layoutWidth;
				}
				x += size.x;
				if (size.y > height) {
					height = size.y;
				}
				pos++;
			}
			if (len > startLine) {
				Ref<TextWordItem> newItem = TextWordItem::create(String(sz + startLine, len - startLine), style);
				if (newItem.isNotNull()) {
					newItem->setLayoutSize(Size(x, height));
					m_lineItems.add(newItem);
				}
				m_x += x;
				m_lineWidth = m_x;
				if (height > m_lineHeight) {
					m_lineHeight = height;
				}
			}
		}
		
		sl_size processWords(Ref<TextItem>* items, sl_size nItems) noexcept
		{
			sl_real x = 0;
			sl_real height = 0;

			sl_size nWords = 0;
			while (nWords < nItems) {
				TextItem* item = items[nWords].get();
				if (item->getType() == TextItemType::Word) {
					TextWordItem* word = static_cast<TextWordItem*>(item);
					Size size = word->getSize();
					word->setLayoutSize(size);
					x += size.x;
					if (height < size.y) {
						height = size.y;
					}
				} else {
					break;
				}
				nWords++;
			}
			if (nWords == 0) {
				return 0;
			}
			
			sl_bool flagBreakWord = sl_false;
			if (m_x + x > m_layoutWidth) {
				if (m_multiLineMode == MultiLineMode::WordWrap) {
					if (m_lineItems.getCount() == 0) {
						flagBreakWord = sl_true;
					} else {
						endLine();
						if (x > m_layoutWidth) {
							flagBreakWord = sl_true;
						}
					}
				} else if (m_multiLineMode == MultiLineMode::BreakWord) {
					flagBreakWord = sl_true;
				}
			}
			
			if (flagBreakWord) {
				for (sl_size i = 0; i < nWords; i++) {
					TextWordItem* item = static_cast<TextWordItem*>(items[i].get());
					Size size = item->getLayoutSize();
					if (m_x + size.x > m_layoutWidth) {
						breakWord(item);
						return i;
					} else {
						m_lineItems.add_NoLock(item);
						m_x += size.x;
						m_lineWidth = m_x;
						if (m_lineHeight < size.y) {
							m_lineHeight = size.y;
						}
					}
				}
			} else {
				m_lineItems.addElements_NoLock(items, nWords);
				m_x += x;
				m_lineWidth = m_x;
				if (m_lineHeight < height) {
					m_lineHeight = height;
				}
			}
			return nWords - 1;
		}
		
		void processSpace(TextSpaceItem* item) noexcept
		{
			Size size = item->getSize();
			if (size.y > m_lineHeight) {
				m_lineHeight = size.y;
			}
			item->setLayoutSize(size);
			m_lineItems.add_NoLock(item);
			m_x += size.x;
		}
		
		void processTab(TextTabItem* item) noexcept
		{
			sl_real h = item->getHeight();
			if (h > m_lineHeight) {
				m_lineHeight = h;
			}
			sl_real tabX = m_x + m_tabMargin;
			tabX = (Math::floor(tabX / m_tabWidth) + 1 ) * m_tabWidth;
			item->setLayoutSize(Size(tabX - m_x, h));
			m_lineItems.add_NoLock(item);
			m_x = tabX;
		}
		
		void processLineBreak(TextLineBreakItem* item) noexcept
		{
			sl_real h = item->getHeight();
			if (h > m_lineHeight) {
				m_lineHeight = h;
			}
			item->setLayoutSize(Size(h / 2, h));
			m_lineItems.add_NoLock(item);
			endLine();
			if (m_multiLineMode == MultiLineMode::Single) {
				m_flagEnd = sl_true;
			}
		}
		
		void processAttach(TextAttachItem* item) noexcept
		{
			Size size = item->getSize();
			if (size.y > m_lineHeight) {
				m_lineHeight = size.y;
			}
			item->setLayoutSize(size);
			m_lineItems.add_NoLock(item);
			m_x += size.x;
			m_lineWidth = m_x;
		}

		void layout(CList< Ref<TextItem> >* list) noexcept
		{
			sl_size n = list->getCount();
			Ref<TextItem>* items = list->getData();
			
			for (sl_size i = 0; i < n; i++) {
				
				TextItem* item = items[i].get();
				
				TextItemType type = item->getType();
				
				switch (type) {
						
					case TextItemType::Word:
						i += processWords(items + i, n - i);
						break;
						
					case TextItemType::Space:
						processSpace(static_cast<TextSpaceItem*>(item));
						break;
						
					case TextItemType::Tab:
						processTab(static_cast<TextTabItem*>(item));
						break;
						
					case TextItemType::LineBreak:
						processLineBreak(static_cast<TextLineBreakItem*>(item));
						break;
						
					case TextItemType::Attach:
						processAttach(static_cast<TextAttachItem*>(item));
						break;
				}
				
				if (m_flagEnd) {
					break;
				}
			}
			
			endLine();

		}
		
	};

	void TextParagraph::layout(const TextParagraphLayoutParam& param) noexcept
	{
		ObjectLocker lock(this);

		m_layoutItems.removeAll_NoLock();
		
		if (param.multiLineMode == MultiLineMode::WordWrap || param.multiLineMode == MultiLineMode::BreakWord) {
			if (param.width < SLIB_EPSILON) {
				return;
			}
		}
		
		_priv_TextParagraph_Layouter layouter(&m_layoutItems, param);
		
		layouter.layout(&m_items);

		m_maxWidth = layouter.m_maxWidth;
		m_totalHeight = layouter.m_y;
		
	}

	void TextParagraph::draw(Canvas* canvas, sl_real x, sl_real y) noexcept
	{
		Rectangle rc = canvas->getInvalidatedRect();
		
		ObjectLocker lock(this);
		
		ListElements< Ref<TextItem> > items(m_layoutItems);
		for (sl_size i = 0; i < items.count; i++) {
			TextItem* item = items[i].get();
			TextItemType type = item->getType();
			if (type == TextItemType::Word) {
				TextWordItem* wordItem = static_cast<TextWordItem*>(item);
				Rectangle frame = wordItem->getLayoutFrame();
				if (rc.intersectRectangle(frame)) {
					Ref<TextStyle> style = wordItem->getStyle();
					if (style.isNotNull()) {
						Ref<Font> font = style->font;
						if (font.isNotNull()) {
							Color backColor = style->backgroundColor;
							if (backColor.a > 0) {
								canvas->fillRectangle(Rectangle(x + frame.left, y + frame.top, x + frame.right, y + frame.bottom), backColor);
							}
							canvas->drawText16(wordItem->getText(), x + frame.left, y + frame.top, wordItem->getFont(), style->textColor);
						}
					}
				}
			}
		}
	}

	sl_real TextParagraph::getMaximumWidth() noexcept
	{
		return m_maxWidth;
	}

	sl_real TextParagraph::getTotalHeight() noexcept
	{
		return m_totalHeight;
	}
	
	sl_real TextParagraph::getPositionLength() noexcept
	{
		return m_positionLength;
	}


	SLIB_DEFINE_OBJECT(SimpleTextBox, Object)

	SimpleTextBox::SimpleTextBox() noexcept
	{
		m_width = 0;
		m_multiLineMode = MultiLineMode::Single;
		m_align = Alignment::Left;
		
		m_contentWidth = 0;
		m_contentHeight = 0;
		
		m_style = new TextStyle;
	}

	SimpleTextBox::~SimpleTextBox() noexcept
	{
	}

	void SimpleTextBox::update(const String& text, const Ref<Font>& font, sl_real width, sl_bool flagWrappingWidth, MultiLineMode multiLineMode, const Alignment& _align) noexcept
	{
		ObjectLocker lock(this);
		
		if (font.isNull()) {
			return;
		}

		m_style->font = font;
		
		if (width < 0) {
			width = 0;
		}
		
		if (flagWrappingWidth) {
			if (multiLineMode != MultiLineMode::Single) {
				multiLineMode = MultiLineMode::Multiple;
			}
		}
		
		Alignment align = _align & Alignment::HorizontalMask;
		
		if (align == Alignment::Left && (multiLineMode == MultiLineMode::Single || multiLineMode == MultiLineMode::Multiple)) {
			width = 0;
		}
		
		sl_bool flagReLayout = sl_false;
		if (m_text != text) {
			m_paragraph.setNull();
			m_contentWidth = 0;
			m_contentHeight = 0;
			if (text.isNotEmpty()) {
				m_paragraph = new TextParagraph;
				if (m_paragraph.isNotNull()) {
					m_paragraph->addText(text, m_style);
				}
			}
			m_text = text;
			flagReLayout = sl_true;
		}
		if (text.isEmpty()) {
			return;
		}
		if (m_paragraph.isNotNull()) {
			if (m_font != font) {
				flagReLayout = sl_true;
			}
			if (!(Math::isAlmostZero(m_width - width)) || m_multiLineMode != multiLineMode || m_align != align) {
				flagReLayout = sl_true;
			}
			if (flagReLayout) {
				TextParagraphLayoutParam param;
				param.width = width;
				param.tabWidth = font->getFontHeight() * 2;
				param.tabMargin = param.tabWidth / 4;
				param.multiLineMode = multiLineMode;
				param.align = align;
				m_paragraph->layout(param);
				
				m_font = font;
				m_multiLineMode = multiLineMode;
				m_align = align;
				m_width = width;
				
				m_contentWidth = m_paragraph->getMaximumWidth();
				m_contentHeight = m_paragraph->getTotalHeight();
			}
		}
	}

	void SimpleTextBox::draw(Canvas* canvas, const String& text, const Ref<Font>& font, const Rectangle& frame, sl_bool flagWrappingWidth, MultiLineMode multiLineMode, const Alignment& align, const Color& color) noexcept
	{
		if (color.isZero()) {
			return;
		}
		sl_real width = frame.getWidth();
		if (width < SLIB_EPSILON) {
			return;
		}
		ObjectLocker lock(this);
		Ref<TextParagraph> paragraphOld = m_paragraph;
		m_style->textColor = color;
		update(text, font, width, flagWrappingWidth, multiLineMode, align);
		if (m_paragraph.isNotNull()) {
			sl_real height = m_paragraph->getTotalHeight();
			Alignment valign = align & Alignment::VerticalMask;
			sl_real y;
			if (valign == Alignment::Middle) {
				y = (frame.left + frame.bottom - height) / 2;
			} else if (valign == Alignment::Bottom) {
				y = frame.bottom - height;
			} else {
				y = frame.top;
			}
			m_paragraph->draw(canvas, frame.left, y);
		}
	}

	sl_real SimpleTextBox::getContentWidth() noexcept
	{
		return m_contentWidth;
	}

	sl_real SimpleTextBox::getContentHeight() noexcept
	{
		return m_contentHeight;
	}

}
