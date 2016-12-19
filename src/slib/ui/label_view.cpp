#include "../../../inc/slib/ui/label_view.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/variant.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LabelView, View)

static List<String16> _Label_getSentences(String16 text)
{
	const sl_char16* buf = text.getData();
	sl_uint32 len = text.getLength();
	sl_uint32 indexStart = 0;
	List<String16> sentences;
	for (sl_size i = 0; i <= len; i++) {
		sl_char16 ch = buf[i];
		if (ch == '\r' || ch == '\n' || ch == 0) {
			String16 sentence = String16(buf + indexStart, i - indexStart).trim();
			indexStart = i + 1;
			sentences.add(sentence);
		}
	}
	return sentences;
}

LabelView::LabelView()
{
	setCreatingNativeWidget(sl_false);
	setUsingFont(sl_true);
	
	m_textAlignment = Alignment::Left;
	m_textColor = Color::Black;
	setOccurringClick(sl_true);
	
}

String LabelView::getText()
{
	return m_text;
}

void LabelView::setText(const String& text, UIUpdateMode mode)
{
	m_text = text;
	if (isNativeWidget()) {
		_setText_NW(text);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Color LabelView::getTextColor()
{
	return m_textColor;
}

void LabelView::setTextColor(const Color& color, UIUpdateMode mode)
{
	m_textColor = color;
	if (isNativeWidget()) {
		_setTextColor_NW(color);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Alignment LabelView::getGravity()
{
	return m_textAlignment;
}

void LabelView::setGravity(Alignment align, UIUpdateMode mode)
{
	m_textAlignment = align;
	if (isNativeWidget()) {
		_setTextAlignment_NW(align);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void LabelView::onDraw(Canvas* canvas)
{
	UIRect boundsInnerPadding = getBoundsInnerPadding();
	
	makeMultilineList();
	
	sl_ui_pos ptY = 0.f;
	for (auto line : m_textLines) {
		UISize size;
		Ref<Font> font = getFont();
		if (font.isNotNull()) {
			size = font->getTextSize(line);
		} else {
			size = UISize::zero();
		}
		
		UIRect rt(0, ptY, boundsInnerPadding.getWidth(), size.y + ptY);
		
		canvas->drawText(line, rt, getFont(), m_textColor, m_textAlignment);
		
		ptY += size.y;
	}
}

void LabelView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagVertical && !flagHorizontal) {
		return;
	}
	
	if (flagHorizontal) {
		List<String16> sentences = _Label_getSentences(m_text);
		sl_ui_pos width = 0.f, height = 0.f;
		for (auto sentence : sentences) {
			UISize size;
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				size = font->getTextSize(sentence);
			} else {
				size = UISize::zero();
			}
			if (size.x > width) {
				width = size.x;
			}
			height += size.y;
			m_textLines.add(sentence);
		}
		setMeasuredWidth(width);
		if (flagVertical) {
			setMeasuredHeight(height);
		}
	}
	if (!flagHorizontal && flagVertical) {
		makeMultilineList();
		sl_ui_pos height = 0.f;
		for (auto line : m_textLines) {
			UISize size;
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				size = font->getTextSize(line);
			} else {
				size = UISize::zero();
			}
			height += size.y;
		}
		setMeasuredHeight(height);
		m_currentSize.y = height;
	}
}

void LabelView::makeMultilineList()
{
	UIRect rect = getBoundsInnerPadding();
	auto maxWidth = rect.getWidth();
	
	if (m_currentSize.x > 0.f && m_currentSize.y > 0.f && m_currentSize.x == rect.getWidth() && m_currentSize.y == rect.getHeight()) {
		return;
	}
	
	m_textLines.removeAll();
	String16 text = m_text;
	sl_uint32 textLen = text.getLength();
	sl_uint32 lineIndex = 0;
	
	const sl_char16* buf = text.getData();
	
	String16 strLine;
	
	sl_bool nextChangeSize = true;
	
	for (int index = 0; index < textLen; )
	{
		sl_char16 character = buf[index];
		if (character == '\n' || character == '\r') {
			lineIndex++;
			m_textLines.add(strLine);
			strLine = "";
			continue;
		}
		
		auto tokenLen = getFirstWordLen(text, index);
		sl_bool newLine = false;
		
		String16 token;
		
		for (int tmp = 0; tmp < tokenLen; ++tmp)
		{
			int letterIndex = index + tmp;
			character = buf[letterIndex];
			if (character == '\r')
			{
				continue;
			}
			
			UISize size;
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				size = font->getTextSize(strLine + token + String16(character, 1));
			} else {
				size = UISize::zero();
			}
			
			if (maxWidth > 0.f && size.x > maxWidth && !SLIB_CHAR_IS_SPACE_TAB(character))
			{
				lineIndex++;
				newLine = true;
				if (strLine.getLength() == 0) {
					strLine = token;
					index += tmp;
				}
				break;
			}
			else {
				token += String16(character, 1);
			}
			
			if (tmp == tokenLen - 1) {
				strLine += token;
			}
		}
		
		if (newLine) {
			m_textLines.add(strLine);
			strLine = "";
			continue;
		}
		
		index += tokenLen;
	}
	
	if(strLine.getLength() > 0) {
		m_textLines.add(strLine);
	}
	
	m_currentSize = rect.getSize();
}

sl_uint32 LabelView::getFirstWordLen(const slib::String16 &text, sl_uint32 startIndex
								  )
{
	sl_uint32 textLen = text.getLength();
	const sl_char16* buf = text.getData();
	sl_char16 character = buf[startIndex];
	if (SLIB_CHAR_IS_WHITE_SPACE(character)) {
		return 1;
	}
	
	sl_uint32 len = 0;
	sl_ui_pos letterX = 0;
	
	UIRect rect = getBoundsInnerPadding();
	auto maxWidth = rect.getWidth();
	
	String16 tmp;
	
	for (int index = startIndex; index < textLen; ++index)
	{
		character = buf[index];
		
		UISize size;
		Ref<Font> font = getFont();
		if (font.isNotNull()) {
			size = font->getTextSize(String16(&character, 1));
		} else {
			size = UISize::zero();
		}
		
		if(maxWidth > 0.f && letterX + size.x > maxWidth && !SLIB_CHAR_IS_SPACE_TAB(character))
		{
			if (len >= 2) {
				return len - 1;
			}
		}
		
		letterX += size.x;
		
		if (character == '\n' || character == '\r' || SLIB_CHAR_IS_SPACE_TAB(character)) {
			break;
		}
		len++;
		tmp += String16(&character, 1);
	}
	
	return len;
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void LabelView::_setText_NW(const String& text)
{
}

void LabelView::_setTextColor_NW(const Color& color)
{
}

void LabelView::_setTextAlignment_NW(Alignment align)
{
}

void LabelView::_setFont_NW(const Ref<Font>& font)
{
}

void LabelView::_setBorder_NW(sl_bool flag)
{
}

void LabelView::_setBackgroundColor_NW(const Color& color)
{
}

#endif

SLIB_UI_NAMESPACE_END
