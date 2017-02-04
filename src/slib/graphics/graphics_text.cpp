#include "../../../inc/slib/graphics/text.h"

#include "../../../inc/slib/graphics/font_atlas.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(TextItem, Object)

TextItem::TextItem(TextItemType type)
 : m_type(type), m_layoutPosition(0, 0), m_layoutSize(0, 0)
{
}

TextItem::~TextItem()
{
}

TextItemType TextItem::getType()
{
	return m_type;
}

Ref<Font> TextItem::getFont()
{
	return m_font;
}

void TextItem::setFont(const Ref<Font>& font)
{
	m_font = font;
}

Point TextItem::getLayoutPosition()
{
	return m_layoutPosition;
}

void TextItem::setLayoutPosition(const Point& pt)
{
	m_layoutPosition = pt;
}

Size TextItem::getLayoutSize()
{
	return m_layoutSize;
}

void TextItem::setLayoutSize(const Size& size)
{
	m_layoutSize = size;
}

Rectangle TextItem::getLayoutFrame()
{
	return Rectangle(m_layoutPosition.x, m_layoutPosition.y, m_layoutPosition.x + m_layoutSize.x, m_layoutPosition.y + m_layoutSize.y);
}


SLIB_DEFINE_OBJECT(TextWordItem, TextItem)

TextWordItem::TextWordItem()
 : TextItem(TextItemType::Word)
{
	m_widthCached = 0;
	m_heightCached = 0;
}

TextWordItem::~TextWordItem()
{
}

Ref<TextWordItem> TextWordItem::create(const String16& text, const Ref<Font>& font, const Color& textColor, const Color& backgroundColor)
{
	if (font.isNotNull()) {
		Ref<TextWordItem> ret = new TextWordItem;
		if (ret.isNotNull()) {
			ret->m_text = text;
			ret->m_font = font;
			ret->m_textColor = textColor;
			ret->m_backColor = backgroundColor;
			return ret;
		}
	}
	return sl_null;
}

String16 TextWordItem::getText()
{
	return m_text;
}

void TextWordItem::setText(const String16& text)
{
	m_text = text;
}

Color TextWordItem::getTextColor()
{
	return m_textColor;
}

void TextWordItem::setTextColor(const Color& color)
{
	m_textColor = color;
}

Color TextWordItem::getBackgroundColor()
{
	return m_backColor;
}

void TextWordItem::setBackgroundColor(const Color& color)
{
	m_backColor = color;
}

Size TextWordItem::getSize()
{
	ObjectLocker lock(this);
	String16 text = m_text;
	Ref<Font> font = m_font;
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

TextSpaceItem::TextSpaceItem()
 : TextItem(TextItemType::Space)
{
}

TextSpaceItem::~TextSpaceItem()
{
}

Ref<TextSpaceItem> TextSpaceItem::create(const Ref<Font>& font)
{
	if (font.isNotNull()) {
		Ref<TextSpaceItem> ret = new TextSpaceItem;
		if (ret.isNotNull()) {
			ret->m_font = font;
			return ret;
		}
	}
	return sl_null;
}

Size TextSpaceItem::getSize()
{
	Ref<Font> font = m_font;
	if (font.isNotNull()) {
		sl_real h = font->getFontHeight();
		return Size(h * 0.3f, h);
	} else {
		return Size::zero();
	}
}


SLIB_DEFINE_OBJECT(TextTabItem, TextItem)

TextTabItem::TextTabItem()
 : TextItem(TextItemType::Tab)
{
}

TextTabItem::~TextTabItem()
{
}

Ref<TextTabItem> TextTabItem::create(const Ref<Font>& font)
{
	if (font.isNotNull()) {
		Ref<TextTabItem> ret = new TextTabItem;
		if (ret.isNotNull()) {
			ret->m_font = font;
			return ret;
		}
	}
	return sl_null;
}

sl_real TextTabItem::getHeight()
{
	Ref<Font> font = m_font;
	if (font.isNotNull()) {
		return font->getFontHeight();
	} else {
		return 0;
	}
}


SLIB_DEFINE_OBJECT(TextLineBreakItem, TextItem)

TextLineBreakItem::TextLineBreakItem()
: TextItem(TextItemType::LineBreak)
{
}

TextLineBreakItem::~TextLineBreakItem()
{
}

Ref<TextLineBreakItem> TextLineBreakItem::create(const Ref<Font>& font)
{
	if (font.isNotNull()) {
		Ref<TextLineBreakItem> ret = new TextLineBreakItem;
		if (ret.isNotNull()) {
			ret->m_font = font;
			return ret;
		}
	}
	return sl_null;
}

sl_real TextLineBreakItem::getHeight()
{
	Ref<Font> font = m_font;
	if (font.isNotNull()) {
		return font->getFontHeight();
	} else {
		return 0;
	}
}


SLIB_DEFINE_OBJECT(TextAttachItem, TextItem)

TextAttachItem::TextAttachItem()
 : TextItem(TextItemType::Attach)
{
}

TextAttachItem::~TextAttachItem()
{
}


TextParagraphLayoutParam::TextParagraphLayoutParam()
{
	width = 1;
	tabWidth = 1;
	tabMargin = 1;
	align = Alignment::Left;
	multiLineMode = MultiLineMode::Single;
}

TextParagraphLayoutParam::~TextParagraphLayoutParam()
{
}

SLIB_DEFINE_OBJECT(TextParagraph, Object)

TextParagraph::TextParagraph()
{
	m_maxWidth = 0;
	m_totalHeight = 0;
}

TextParagraph::~TextParagraph()
{
}

void TextParagraph::addText(const String16& text, const Ref<Font>& font, const Color& textColor, const Color& backgroundColor)
{
	if (text.isEmpty()) {
		return;
	}
	if (font.isNull()) {
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
				Ref<TextWordItem> item = TextWordItem::create(String16(sz + startWord, pos - startWord), font, textColor, backgroundColor);
				if (item.isNotNull()) {
					m_items.add_NoLock(item);
				}
			}
			if (ch == ' ') {
				Ref<TextSpaceItem> item = TextSpaceItem::create(font);
				if (item.isNotNull()) {
					m_items.add_NoLock(item);
				}
			} else if (ch == '\t') {
				Ref<TextTabItem> item = TextTabItem::create(font);
				if (item.isNotNull()) {
					m_items.add_NoLock(item);
				}
			} else if (ch == '\r' || ch == '\n') {
				Ref<TextLineBreakItem> item = TextLineBreakItem::create(font);
				if (item.isNotNull()) {
					m_items.add_NoLock(item);
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
		Ref<TextWordItem> item = TextWordItem::create(text, font, textColor, backgroundColor);
		if (item.isNotNull()) {
			m_items.add_NoLock(item);
		}
	} else if (startWord < len) {
		Ref<TextWordItem> item = TextWordItem::create(String16(sz + startWord, len - startWord), font, textColor, backgroundColor);
		if (item.isNotNull()) {
			m_items.add_NoLock(item);
		}
	}
}

void TextParagraph::setFont(const Ref<Font>& font)
{
	ObjectLocker lock(this);
	{
		ListElements< Ref<TextItem> > items(m_items);
		for (sl_size i = 0; i < items.count; i++) {
			Ref<TextItem>& item = items[i];
			item->setFont(font);
		}
	}
	{
		ListElements< Ref<TextItem> > items(m_layoutItems);
		for (sl_size i = 0; i < items.count; i++) {
			Ref<TextItem>& item = items[i];
			item->setFont(font);
		}
	}
}

void TextParagraph::setTextColor(const Color& color)
{
	ObjectLocker lock(this);
	{
		ListElements< Ref<TextItem> > items(m_items);
		for (sl_size i = 0; i < items.count; i++) {
			TextItem* item = items[i].get();
			if (item->getType() == TextItemType::Word) {
				(static_cast<TextWordItem*>(item))->setTextColor(color);
			}
		}
	}
	{
		ListElements< Ref<TextItem> > items(m_layoutItems);
		for (sl_size i = 0; i < items.count; i++) {
			TextItem* item = items[i].get();
			if (item->getType() == TextItemType::Word) {
				(static_cast<TextWordItem*>(item))->setTextColor(color);
			}
		}
	}
}

class _TextParagraph_Layouter
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
	_TextParagraph_Layouter(CList< Ref<TextItem> >* layoutItems, const TextParagraphLayoutParam& param)
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
	
	void endLine()
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
	
	void breakWord(TextWordItem* breakItem)
	{
		Ref<Font> font = breakItem->getFont();
		String16 text = breakItem->getText();
		
		if (font.isNull()) {
			return;
		}
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
				Ref<TextWordItem> newItem = TextWordItem::create(String(sz + startLine, pos - startLine), font, breakItem->getTextColor(), breakItem->getBackgroundColor());
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
			Ref<TextWordItem> newItem = TextWordItem::create(String(sz + startLine, len - startLine), font, breakItem->getTextColor(), breakItem->getBackgroundColor());
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
	
	sl_size processWords(Ref<TextItem>* items, sl_size nItems)
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
	
	void processSpace(TextSpaceItem* item)
	{
		Size size = item->getSize();
		if (size.y > m_lineHeight) {
			m_lineHeight = size.y;
		}
		item->setLayoutSize(size);
		m_lineItems.add_NoLock(item);
		m_x += size.x;
	}
	
	void processTab(TextTabItem* item)
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
	
	void processLineBreak(TextLineBreakItem* item)
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
	
	void processAttach(TextAttachItem* item)
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

	void layout(CList< Ref<TextItem> >* list)
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

void TextParagraph::layout(const TextParagraphLayoutParam& param)
{
	ObjectLocker lock(this);

	m_layoutItems.removeAll_NoLock();
	
	if (param.multiLineMode == MultiLineMode::WordWrap || param.multiLineMode == MultiLineMode::BreakWord) {
		if (param.width < SLIB_EPSILON) {
			return;
		}
	}
	
	_TextParagraph_Layouter layouter(&m_layoutItems, param);
	
	layouter.layout(&m_items);

	m_maxWidth = layouter.m_maxWidth;
	m_totalHeight = layouter.m_y;
	
}

void TextParagraph::draw(Canvas* canvas, sl_real x, sl_real y)
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
				Color backColor = wordItem->getBackgroundColor();
				if (backColor.a > 0) {
					canvas->fillRectangle(Rectangle(x + frame.left, y + frame.top, x + frame.right, y + frame.bottom), wordItem->getBackgroundColor());
				}
				canvas->drawText16(wordItem->getText(), x + frame.left, y + frame.top, wordItem->getFont(), wordItem->getTextColor());
			}
		}
	}
}

sl_real TextParagraph::getMaximumWidth()
{
	return m_maxWidth;
}

sl_real TextParagraph::getTotalHeight()
{
	return m_totalHeight;
}


SLIB_DEFINE_OBJECT(SimpleTextBox, Object)

SimpleTextBox::SimpleTextBox()
{
	m_width = 0;
	m_multiLineMode = MultiLineMode::Single;
	m_align = Alignment::Left;
	m_textColor = Color::zero();
	
	m_contentWidth = 0;
	m_contentHeight = 0;
}

SimpleTextBox::~SimpleTextBox()
{
}

void SimpleTextBox::update(const String& text, const Ref<Font>& font, sl_real width, sl_bool flagWrappingWidth, MultiLineMode multiLineMode, const Alignment& _align)
{
	ObjectLocker lock(this);
	
	if (font.isNull()) {
		return;
	}
	
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
				m_paragraph->addText(text, font, m_textColor);
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
			m_paragraph->setFont(font);
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

void SimpleTextBox::draw(Canvas* canvas, const String& text, const Ref<Font>& font, const Rectangle& frame, sl_bool flagWrappingWidth, MultiLineMode multiLineMode, const Alignment& align, const Color& color)
{
	if (color.isZero()) {
		return;
	}
	sl_real width = frame.getWidth();
	if (width < SLIB_EPSILON) {
		return;
	}
	ObjectLocker lock(this);
	Color colorOld = m_textColor;
	Ref<TextParagraph> paragraphOld = m_paragraph;
	m_textColor = color;
	update(text, font, width, flagWrappingWidth, multiLineMode, align);
	if (colorOld != color && paragraphOld == m_paragraph) {
		if (m_paragraph.isNotNull()) {
			m_paragraph->setTextColor(color);
		}
	}
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

sl_real SimpleTextBox::getContentWidth()
{
	return m_contentWidth;
}

sl_real SimpleTextBox::getContentHeight()
{
	return m_contentHeight;
}

SLIB_GRAPHICS_NAMESPACE_END
