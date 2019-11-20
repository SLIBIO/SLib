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

#include "slib/ui/picker_view.h"

#include "slib/ui/resource.h"
#include "slib/ui/core.h"
#include "slib/core/safe_static.h"
#include "slib/core/math.h"
#include "slib/core/timer.h"

#define ANIMATE_FRAME_MS 15

#if defined(SLIB_UI_IS_IOS)
#	define HAS_NATIVE_WIDGET_IMPL
#endif

namespace slib
{

	SLIB_DEFINE_OBJECT(PickerView, View)
	
	PickerView::PickerView()
	{		
#ifdef HAS_NATIVE_WIDGET_IMPL
		setCreatingNativeWidget(sl_true);
#endif
		setUsingFont(sl_true);
		setClipping(sl_true, UIUpdateMode::Init);
		
		m_indexSelected = 0;
		
		m_textColor = Color::Black;
		
		m_linesHalfCount = 2;
		m_flagCircular = sl_false;
		m_yOffset = 0;

		m_speedFlow = 0;
	}
	
	PickerView::~PickerView()
	{
	}

	sl_uint32 PickerView::getItemsCount()
	{
		return (sl_uint32)(m_titles.getCount());
	}
	
	void PickerView::setItemsCount(sl_uint32 n, UIUpdateMode mode)
	{
		Ptr<IPickerViewInstance> instance = getPickerViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&PickerView::setItemsCount, n, mode)
		}
		m_values.setCount(n);
		m_titles.setCount(n);
		if (instance.isNotNull()) {
			instance->refreshItemsCount(this);
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
		} else {
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
			invalidate(mode);
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
		Ptr<IPickerViewInstance> instance = getPickerViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&PickerView::setItemTitle, index, title, mode)
		}
		if (index < m_titles.getCount()) {
			m_titles.setAt(index, title);
			if (instance.isNotNull()) {
				instance->setItemTitle(this, index, title);
			} else {
				invalidate(mode);
			}
		}
	}
	
	List<String> PickerView::getTitles()
	{
		return m_titles;
	}
	
	void PickerView::setTitles(const List<String>& list, UIUpdateMode mode)
	{
		Ptr<IPickerViewInstance> instance = getPickerViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&PickerView::setTitles, list, mode)
		}
		m_titles = list;
		if (instance.isNotNull()) {
			instance->refreshItemsContent(this);
			sl_uint32 n = (sl_uint32)(m_titles.getCount());
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
		} else {
			sl_uint32 n = (sl_uint32)(m_titles.getCount());
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
			invalidate(mode);
		}
	}
	
	void PickerView::selectIndex(sl_uint32 index, UIUpdateMode mode)
	{
		Ptr<IPickerViewInstance> instance = getPickerViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&PickerView::selectIndex, index, mode)
		}
		if (index < m_titles.getCount()) {
			m_indexSelected = index;
			if (instance.isNotNull()) {
				instance->select(this, index);
			} else {
				invalidate(mode);
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
		invalidate(mode);
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
		
	}
	
	void PickerView::onMouseEvent(UIEvent* ev)
	{
		UIAction action = ev->getAction();
				
		if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
			_stopFlow();
			m_motionTracker.clearMovements();
			m_motionTracker.addMovement(ev->getPoint());
			invalidate();
		} else if (action == UIAction::LeftButtonDrag || action == UIAction::TouchMove) {
			_stopFlow();
			Point ptLast;
			if (m_motionTracker.getLastPosition(&ptLast)) {
				sl_real offset = ev->getY() - ptLast.y;
				_flow((sl_ui_pos)offset);
				invalidate();
			}
			m_motionTracker.addMovement(ev->getPoint());
		} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd || action == UIAction::TouchCancel) {
			m_motionTracker.addMovement(ev->getPoint());
			sl_real speed = 0;
			m_motionTracker.getVelocity(sl_null, &speed);
			m_motionTracker.clearMovements();
			_startFlow(speed);
			invalidate();
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(PickerView, SelectItem, sl_uint32 index)

	void PickerView::dispatchSelectItem(sl_uint32 index)
	{
		SLIB_INVOKE_EVENT_HANDLER(SelectItem, index)
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
		if (m_indexSelected != (sl_uint32)index) {
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
		m_timeFlowFrameBefore = Time::now();
		m_timerFlow = startTimer(SLIB_FUNCTION_WEAKREF(PickerView, _animationCallback, this), ANIMATE_FRAME_MS);
	}
	
	void PickerView::_stopFlow()
	{
		m_timerFlow.setNull();
	}
	
	void PickerView::_animationCallback(Timer* timer)
	{
		Time time = Time::now();
		sl_real ellapsed = (sl_real)((time - m_timeFlowFrameBefore).getSecondsCountf());
		m_timeFlowFrameBefore = time;
		
		float T = UIResource::getScreenMinimum() /
#ifdef SLIB_PLATFORM_IS_MOBILE
			2.0f;
#else
			4.0f;
#endif
		if (Math::abs(m_speedFlow) <= T) {
			if (Math::abs(m_yOffset) >= 1) {
				if (m_yOffset > 0) {
					m_speedFlow = -T;
				} else {
					m_speedFlow = T;
				}
				sl_ui_pos f = (sl_ui_pos)(m_speedFlow * ellapsed);
				if (Math::abs(f) > Math::abs(m_yOffset)) {
					_stopFlow();
					m_yOffset = 0;
					invalidate();
					return;
				} else {
					_flow(f);
				}
			} else {
				_stopFlow();
				m_yOffset = 0;
				invalidate();
				return;
			}
		} else {
			_flow((sl_ui_pos)(m_speedFlow * ellapsed));
		}
		
		invalidate();
		
		m_speedFlow *= 0.97f;
		
	}
	
#if !defined(HAS_NATIVE_WIDGET_IMPL)
	Ref<ViewInstance> PickerView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
	
	Ptr<IPickerViewInstance> PickerView::getPickerViewInstance()
	{
		return sl_null;
	}
#endif

}
