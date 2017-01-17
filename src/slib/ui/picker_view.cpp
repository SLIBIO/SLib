#include "../../../inc/slib/ui/picker_view.h"

#include "../../../inc/slib/core/safe_static.h"
#include "../../../inc/slib/core/math.h"
#include "../../../inc/slib/ui/resource.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(PickerView, View)

PickerView::PickerView()
{
	setCreatingNativeWidget(sl_true);
	setUsingFont(sl_true);
	
	m_indexSelected = 0;

	m_textColor = Color::Black;
	
	m_linesHalfCount = 2;
	m_flagCircular = sl_false;
	m_yOffset = 0;
	m_timeCallbackBefore.setZero();
}

sl_uint32 PickerView::getItemsCount()
{
	return (sl_uint32)(m_titles.getCount());
}

void PickerView::setItemsCount(sl_uint32 n, UIUpdateMode mode)
{
	m_values.setCount(n);
	m_titles.setCount(n);
	if (isNativeWidget()) {
		_refreshItemsCount_NW();
		if (m_indexSelected >= n) {
			selectIndex(0, UIUpdateMode::NoRedraw);
		}
	} else {
		if (m_indexSelected >= n) {
			selectIndex(0, UIUpdateMode::NoRedraw);
		}
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void PickerView::removeAllItems(UIUpdateMode mode)
{
	setItemsCount(0, mode);
}

String PickerView::getItemValue(sl_uint32 index)
{
	return m_values.getValueAt(index);
}

void PickerView::setItemValue(sl_uint32 index, const String& value)
{
	m_values.setAt(index, value);
}

List<String> PickerView::getValues()
{
	return m_values;
}

void PickerView::setValues(const List<String>& list)
{
	m_values = list;
}

String PickerView::getItemTitle(sl_uint32 index)
{
	return m_titles.getValueAt(index);
}

void PickerView::setItemTitle(sl_uint32 index, const String& title, UIUpdateMode mode)
{
	if (index < m_titles.getCount()) {
		m_titles.setAt(index, title);
		if (isNativeWidget()) {
			_setItemTitle_NW(index, title);
		} else {
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}
}

List<String> PickerView::getTitles()
{
	return m_titles;
}

void PickerView::setTitles(const List<String>& list, UIUpdateMode mode)
{
	m_titles = list;
	if (isNativeWidget()) {
		_refreshItemsContent_NW();
		sl_uint32 n = (sl_uint32)(m_titles.getCount());
		if (m_indexSelected >= n) {
			selectIndex(0, UIUpdateMode::NoRedraw);
		}
	} else {
		sl_uint32 n = (sl_uint32)(m_titles.getCount());
		if (m_indexSelected >= n) {
			selectIndex(0, UIUpdateMode::NoRedraw);
		}
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void PickerView::selectIndex(sl_uint32 index, UIUpdateMode mode)
{
	if (index < m_titles.getCount()) {
		m_indexSelected = index;
		if (isNativeWidget()) {
			_select_NW(index);
		} else {
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	} else {
		if (index == 0) {
			m_indexSelected = 0;
		}
	}
}

void PickerView::selectValue(const String& value, UIUpdateMode mode)
{
	sl_int32 m = (sl_int32)(m_values.indexOf(value));
	if (m > 0) {
		selectIndex(m, mode);
	}
}

sl_uint32 PickerView::getSelectedIndex()
{
	if (isNativeWidget()) {
		_getSelectedIndex_NW();
	}
	return m_indexSelected;
}

String PickerView::getSelectedValue()
{
	return m_values.getValueAt(m_indexSelected);
}

String PickerView::getSelectedTitle()
{
	return m_titles.getValueAt(m_indexSelected);
}

Color PickerView::getTextColor()
{
	return m_textColor;
}

void PickerView::setTextColor(const Color& color, UIUpdateMode mode)
{
	m_textColor = color;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void PickerView::onDraw(Canvas* canvas)
{
	Ref<Font> font = getFont();
	if (font.isNull()) {
		return;
	}
	sl_int32 nLinesHalf = m_linesHalfCount;
	sl_real lineHeight = (sl_real)(_getLineHeight());
	sl_real height = (nLinesHalf * 2 + 1) * lineHeight;
	
	Rectangle rect = getBoundsInnerPadding();
	sl_real yStart = rect.top + (rect.getHeight() - height) / 2;
	
	ListLocker<String> titles(m_titles);
	sl_int32 i;
	
	{
		CanvasStateScope scope(canvas);
		canvas->clipToRectangle(rect.left, yStart, rect.right - rect.left, nLinesHalf * lineHeight);
		rect.top = yStart - lineHeight + m_yOffset;
		for (i = -nLinesHalf - 1; i <= 0; i++) {
			rect.bottom = rect.top + lineHeight;
			sl_int32 index;
			if (m_flagCircular) {
				index = _getCircularIndex(m_indexSelected + i);
			} else {
				index = m_indexSelected + i;
			}
			if (index >= 0 && index < (sl_int32)(titles.count)) {
				sl_int32 alpha = 50 + 100 * (nLinesHalf + 1 - Math::abs(i)) / (nLinesHalf + 1);
				Color c = m_textColor;
				c.a = (sl_uint8)((sl_int32)(c.a) * alpha / 256);
				canvas->drawText(titles[index], rect, font, c, Alignment::Center);
			}
			rect.top = rect.bottom;
		}
	}
	
	{
		CanvasStateScope scope(canvas);
		canvas->clipToRectangle(rect.left, yStart + nLinesHalf * lineHeight + lineHeight, rect.right - rect.left, lineHeight * nLinesHalf);
		rect.top = yStart + nLinesHalf * lineHeight + m_yOffset;
		for (i = 0; i <= nLinesHalf + 1; i++) {
			rect.bottom = rect.top + lineHeight;
			sl_int32 index;
			if (m_flagCircular) {
				index = _getCircularIndex(m_indexSelected + i);
			} else {
				index = m_indexSelected + i;
			}
			if (index >= 0 && index < (sl_int32)(titles.count)) {
				sl_int32 alpha = 50 + 100 * (nLinesHalf + 1 - Math::abs(i)) / (nLinesHalf + 1);
				Color c = m_textColor;
				c.a = (sl_uint8)((sl_int32)(c.a) * alpha / 256);
				canvas->drawText(titles[index], rect, font, c, Alignment::Center);
			}
			rect.top = rect.bottom;
		}
	}
	
	{
		CanvasStateScope scope(canvas);
		canvas->clipToRectangle(rect.left, yStart + nLinesHalf * lineHeight, rect.right - rect.left, lineHeight);
		rect.top = yStart + nLinesHalf * lineHeight - lineHeight + m_yOffset;
		Color c = m_textColor;
		for (i = -1; i <= 1; i++) {
			rect.bottom = rect.top + lineHeight;
			sl_int32 index;
			if (m_flagCircular) {
				index = _getCircularIndex(m_indexSelected + i);
			} else {
				index = m_indexSelected + i;
			}
			if (index >= 0 && index < (sl_int32)(titles.count)) {
				canvas->drawText(titles[index], rect, font, c, Alignment::Center);
			}
			rect.top = rect.bottom;
		}
	}
	
	Color c = m_textColor;
	c.a /= 2;
	/*
	Ref<Pen> pen = Pen::createSolidPen(1, c);
	canvas->drawLine(rect.left, yStart + lineHeight * nLinesHalf, rect.right, yStart + lineHeight * nLinesHalf, pen);
	canvas->drawLine(rect.left, yStart + lineHeight * nLinesHalf + lineHeight, rect.right, yStart + lineHeight * nLinesHalf + lineHeight, pen);
	*/
}

void PickerView::onMouseEvent(UIEvent* ev)
{
	UIAction action = ev->getAction();
	Point pt = ev->getPoint();

	Time time = Time::now();
	sl_real y = pt.y;

	if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
		_stopFlow();
		m_timeTouchBefore = time;
		m_speedBefore = 0;
		m_yTouchBefore = y;
		invalidate();
	} else if (action == UIAction::LeftButtonDrag || action == UIAction::TouchMove) {
		_stopFlow();
		sl_real offset = y - m_yTouchBefore;
		m_speedBefore = (sl_real)(offset / (time - m_timeTouchBefore).getMillisecondsCountf());
		_flow((sl_ui_pos)offset);
		m_timeTouchBefore = time;
		m_yTouchBefore = y;
		invalidate();
	} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd || action == UIAction::TouchCancel) {
		sl_real timeOffset = (sl_real)((time - m_timeTouchBefore).getMillisecondsCountf());
		if (timeOffset > 300) {
			m_speedBefore = 0;
		}
		_startFlow(m_speedBefore);
		invalidate();
	}
}

void PickerView::onSelectItem(sl_uint32 index)
{
}

void PickerView::dispatchSelectItem(sl_uint32 index)
{
	onSelectItem(index);
	PtrLocker<IPickerViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectItem(this, index);
	}
	getOnSelectItem()(index);
}

void PickerView::_selectIndexInner(sl_int32 index)
{
	if (m_flagCircular) {
		index = _getCircularIndex(index);
	} else {
		sl_int32 n = (sl_uint32)(m_values.getCount());
		if (n <= 0) {
			return;
		}
		if (index >= n) {
			index = n - 1;
		} else if (index < 0) {
			index = 0;
		}
	}
	if (m_indexSelected != index) {
		m_indexSelected = index;
		dispatchSelectItem(index);
	}
}

sl_uint32 PickerView::_getCircularIndex(sl_int32 index)
{
	sl_int32 n = (sl_uint32)(m_values.getCount());
	if (n <= 0) {
		return 0;
	}
	index = index % n;
	if (index < 0) {
		index += n;
	}
	return index;
}

sl_ui_len PickerView::_getLineHeight()
{
	Ref<Font> font = getFont();
	if (font.isNotNull()) {
		return (sl_ui_len)(font->getFontHeight() * 1.2f);
	}
	return 10;
}

void PickerView::_flow(sl_ui_pos offset)
{
	sl_int32 k = (sl_int32)(m_yOffset + offset);
	sl_int32 lineHeight = (sl_int32)(_getLineHeight());
	if (lineHeight == 0) {
		return;
	}
	sl_int32 index = m_indexSelected;
	if (k >= 0) {
		sl_int32 n = k / lineHeight;
		sl_int32 m = k - n * lineHeight;
		if (m > lineHeight / 2) {
			n += 1;
		}
		_selectIndexInner(index - n);
	} else {
		k = -k;
		sl_int32 n = k / lineHeight;
		sl_int32 m = k - n * lineHeight;
		if (m > lineHeight / 2) {
			n += 1;
		}
		_selectIndexInner(index + n);
	}
	m_yOffset = (sl_ui_pos)(m_yOffset + offset - (index - m_indexSelected) * lineHeight);
	if (m_yOffset > lineHeight) {
		m_yOffset = lineHeight;
		m_speedFlow = 0;
	}
	if (m_yOffset < -lineHeight) {
		m_yOffset = -lineHeight;
		m_speedFlow = 0;
	}
}

void PickerView::_startFlow(sl_real speed)
{
	m_speedFlow = speed;
	ObjectLocker lock(this);
	if (m_timer.isNull()) {
		m_timeCallbackBefore.setZero();
		m_timer = Dispatch::addTimer(SLIB_FUNCTION_WEAKREF(PickerView, _timerCallback, this), 10);
	}
}

void PickerView::_stopFlow()
{
	ObjectLocker lock(this);
	if (m_timer.isNotNull()) {
		m_timer->stop();
		Dispatch::removeTimer(m_timer);
		m_timer.setNull();
	}
}

void PickerView::_timerCallback()
{
	ObjectLocker lock(this);
	if (m_timer.isNull()) {
		return;
	}
	Time time = Time::now();
	if (m_timeCallbackBefore.isZero()) {
		m_timeCallbackBefore = time;
		return;
	}
	float T = UIResource::getScreenMinimum() / 2000.0f;
	if (Math::abs(m_speedFlow) <= T) {
		if (Math::abs(m_yOffset) >= T) {
			if (m_yOffset > 0) {
				m_speedFlow = -T;
			} else {
				m_speedFlow = T;
			}
			sl_ui_pos f = (sl_ui_pos)(m_speedFlow * (time - m_timeCallbackBefore).getMillisecondsCountf());
			if (Math::abs(f) > Math::abs(m_yOffset)) {
				_stopFlow();
				m_yOffset = 0;
				m_speedFlow = 0;
			} else {
				_flow(f);
			}
		} else {
			_stopFlow();
			m_yOffset = 0;
			m_speedFlow = 0;
		}
	} else {
		_flow((sl_ui_pos)(m_speedFlow * (time - m_timeCallbackBefore).getMillisecondsCountf()));
	}
	invalidate();
	m_speedFlow *= 0.97f;
	m_timeCallbackBefore = time;
}

#if !(defined(SLIB_PLATFORM_IS_IOS))

Ref<ViewInstance> PickerView::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
}

void PickerView::_getSelectedIndex_NW()
{
}

void PickerView::_select_NW(sl_uint32 index)
{
}

void PickerView::_refreshItemsCount_NW()
{
}

void PickerView::_refreshItemsContent_NW()
{
}

void PickerView::_setItemTitle_NW(sl_uint32 index, const String& title)
{
}

void PickerView::_setFont_NW(const Ref<Font>& font)
{
}

#endif

SLIB_UI_NAMESPACE_END
