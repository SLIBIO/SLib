#include "../../../inc/slib/ui/label_view.h"

#include "../../../inc/slib/core/scoped.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LabelView, View)

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
	m_currentTextBoundWidth = 0;
	
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
	_makeMultilineList();
	
	ListLocker<String16> lines(m_textLines);
	
	if (lines.count == 0) {
		return;
	}
	
	Ref<Font> font = getFont();
	if (font.isNull()) {
		return;
	}
	
	if (lines.count == 1) {
		canvas->drawText(lines[0], getBoundsInnerPadding(), font, m_textColor, m_textAlignment);
		return;
	}
	
	UIRect rect(getPaddingLeft(), getPaddingTop(), getWidth() - getPaddingRight(), 0);
	
	SLIB_SCOPED_BUFFER(sl_ui_len, 64, widthLines, lines.count)
	SLIB_SCOPED_BUFFER(sl_ui_len, 64, heightLines, lines.count)
	
	sl_size i;
	sl_ui_len totalHeight = 0;
	
	for (i = 0; i < lines.count; i++) {
		String16& line = lines[i];
		UISize size = font->getTextSize(line);
		widthLines[i] = size.x;
		heightLines[i] = size.y;
		totalHeight += size.y;
	}
	
	if (totalHeight <= 0) {
		return;
	}
	
	Alignment alignHorz = m_textAlignment & Alignment::HorizontalMask;
	Alignment alignVert = m_textAlignment & Alignment::VerticalMask;
	
	if (alignVert == Alignment::Middle) {
		rect.top += (getHeight() - totalHeight) / 2;
	} else if (alignVert == Alignment::Bottom) {
		rect.top += (getHeight() - totalHeight);
	}
	
	for (i = 0; i < lines.count; i++) {
		String16& line = lines[i];
		rect.bottom = rect.top + heightLines[i];
		canvas->drawText(line, rect, font, m_textColor, alignHorz);
		rect.top = rect.bottom;
	}

}

static List<String16> _Label_getLines(const String16& text)
{
	const sl_char16* buf = text.getData();
	sl_uint32 len = (sl_uint32)(text.getLength());
	sl_uint32 indexStart = 0;
	List<String16> lines;
	for (sl_uint32 i = 0; i <= len; i++) {
		sl_char16 ch = buf[i];
		if (ch == '\r' || ch == '\n' || ch == 0) {
			String16 line = String16(buf + indexStart, i - indexStart);
			i++;
			if (ch == '\r' && i < len && buf[i] == '\n') {
				i++;
			}
			indexStart = i;
			lines.add(line);
		}
	}
	return lines;
}

void LabelView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagVertical && !flagHorizontal) {
		return;
	}
	
	Ref<Font> font = getFont();
	if (font.isNull()) {
		return;
	}

	if (flagHorizontal) {
		sl_ui_pos maxWidth = 0, height = 0;
		ListLocker<String16> lines(_Label_getLines(m_text));
		for (sl_size i = 0; i < lines.count; i++) {
			String16& line = lines[i];
			UISize size = font->getTextSize(line);
			if (size.x > maxWidth) {
				maxWidth = size.x;
			}
			height += size.y;
		}
		setMeasuredWidth(maxWidth);
		if (flagVertical) {
			setMeasuredHeight(height);
		}
	} else {
		if (flagVertical) {
			_makeMultilineList();
			sl_ui_pos height = 0;
			ListLocker<String16> lines(m_textLines);
			for (sl_size i = 0; i < lines.count; i++) {
				String16& line = lines[i];
				UISize size = font->getTextSize(line);
				height += size.y;
			}
			setMeasuredHeight(height);
		}
	}
}

void LabelView::_makeMultilineList()
{
	
	Ref<Font> font = getFont();
	if (font.isNull()) {
		return;
	}
	
	UIRect rect = getBoundsInnerPadding();
	sl_ui_len maxWidth = rect.getWidth();
	
	if (Math::isAlmostZero(maxWidth)) {
		return;
	}
	
	if (m_currentTextBoundWidth > 0 && m_currentTextBoundWidth == maxWidth && font == m_currentTextFont) {
		return;
	}
	
	String16 text = m_text;
	const sl_char16* buf = text.getData();
	sl_size lenText = text.getLength();
	
	List<String16> lines;
	String16 line;
	sl_size posLine = 0;
	sl_size posWordStart = 0;
	sl_bool flagNewLine = sl_false;
	String16 lineMeasure = String16::fromStatic(buf, 1);

	sl_size pos = 0;
	for (; pos < lenText; pos++) {
		if (flagNewLine) {
			lines.add_NoLock(line);
			lineMeasure = String16::fromStatic(buf + pos, 1);
			posLine = pos;
			posWordStart = pos;
			flagNewLine = sl_false;
		}
		sl_char16 ch = buf[pos];
		if (ch == '\r' || ch == '\n') {
			line = String16(buf + posLine, pos - posLine);
			if (ch == '\r' && pos + 1 < lenText && buf[pos + 1] == '\n') {
				pos++;
			}
			flagNewLine = sl_true;
		} else if (SLIB_CHAR_IS_SPACE_TAB(ch)) {
			posWordStart = pos + 1;
		} else {
			lineMeasure.setLength(pos + 1 - posLine);
			UISize size = font->getTextSize(lineMeasure);
			if (size.x > maxWidth) {
				sl_bool flagOneWord = sl_true;
				for (sl_size k = posLine; k < posWordStart; k++) {
					if (!(SLIB_CHAR_IS_SPACE_TAB(buf[k]))) {
						flagOneWord = sl_false;
						break;
					}
				}
				if (flagOneWord) {
					if (pos >= posLine + 2) {
						pos--;
					}
					line = String16(buf + posLine, pos - posLine);
				} else {
					posWordStart--;
					pos = posWordStart;
					while (posWordStart > posLine) {
						if (!(SLIB_CHAR_IS_SPACE_TAB(buf[posWordStart-1]))) {
							break;
						}
						posWordStart--;
					}
					line = String16(buf + posLine, posWordStart - posLine);
				}
				flagNewLine = sl_true;
			}
		}
	}
	if (flagNewLine) {
		lines.add_NoLock(line);
	}
	if (pos > posLine) {
		lines.add_NoLock(String16(buf + posLine, pos - posLine));
	}
	
	m_textLines = lines;
	
	m_currentTextBoundWidth = maxWidth;
	m_currentTextFont = font;
	
}

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
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
