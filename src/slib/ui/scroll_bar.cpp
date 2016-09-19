#include "../../../inc/slib/ui/scroll_bar.h"

#include "../../../inc/slib/core/math.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ScrollBar, View)

class _ScrollBar_Bar : public Drawable
{
public:
	Ref<Brush> m_brush;
	
public:
	_ScrollBar_Bar(const Color& color)
	{
		m_brush = Brush::createSolidBrush(color);
	}
	
public:
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst)
	{
		sl_bool flagAntiAlias = canvas->isAntiAlias();
		canvas->setAntiAlias(sl_true);
		sl_real r = Math::min(rectDst.getWidth(), rectDst.getHeight()) * 0.5f;
		sl_real padding = 2;
		r -= padding;
		canvas->drawRoundRect(Rectangle(rectDst.left + padding, rectDst.top + padding, rectDst.right - padding, rectDst.bottom - padding), Size(r, r), Ref<Pen>::null(), m_brush);
		canvas->setAntiAlias(flagAntiAlias);
	}
};

class _ScrollBar_Static
{
public:
	Ref<Drawable> defaultBar;
	Ref<Drawable> defaultClickedBar;

	_ScrollBar_Static()
	{
		defaultBar = new _ScrollBar_Bar(Color(0, 0, 0, 110));
		defaultClickedBar = new _ScrollBar_Bar(Color(0, 0, 0, 160));
	}
};

SLIB_SAFE_STATIC_GETTER(_ScrollBar_Static, _ScrollBar_getStatic)

ScrollBar::ScrollBar(LayoutOrientation orientation)
{
	m_orientation = orientation;
	m_value = 0;
	m_page = 0;
	m_line = 0;
	m_value_min = 0;
	m_value_max = 1;
	
	m_valueDown = 0;
	m_posDown = 0;

	_ScrollBar_Static* s = _ScrollBar_getStatic();
	if (s) {
		m_bar = s->defaultBar;
		m_clickedBar = s->defaultClickedBar;
	}
	m_bar_len_ratio_min = 2;
}

LayoutOrientation ScrollBar::getOrientation()
{
	return m_orientation;
}

void ScrollBar::setOrientation(LayoutOrientation orientation, sl_bool flagRedraw)
{
	m_orientation = orientation;
	if (flagRedraw) {
		invalidate();
	}
}

sl_bool ScrollBar::isVertical()
{
	return m_orientation == LayoutOrientation::Vertical;
}

void ScrollBar::setVertical(sl_bool flagRedraw)
{
	setOrientation(LayoutOrientation::Vertical, flagRedraw);
}

sl_bool ScrollBar::isHorizontal()
{
	return m_orientation == LayoutOrientation::Horizontal;
}

void ScrollBar::setHorizontal(sl_bool flagRedraw)
{
	setOrientation(LayoutOrientation::Horizontal, flagRedraw);
}

sl_scroll_pos ScrollBar::getValue()
{
	return m_value;
}

void ScrollBar::setValue(sl_scroll_pos value, sl_bool flagRedraw)
{
	sl_scroll_pos _max = m_value_max - m_page;
	if (value > _max) {
		value = _max;
	}
	if (value < m_value_min) {
		value = m_value_min;
	}
	if (Math::isAlmostZero(value - m_value)) {
		m_value = value;
		return;
	}
	m_value = value;
	dispatchScroll(value);
	if (flagRedraw) {
		invalidate();
	}
}

void ScrollBar::setValueOfOutRange(sl_scroll_pos value, sl_bool flagRedraw)
{
	m_value = value;
	if (flagRedraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getPage()
{
	return m_page;
}

void ScrollBar::setPage(sl_scroll_pos page, sl_bool flagRedraw)
{
	m_page = page;
	if (flagRedraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getLine()
{
	return m_line;
}

void ScrollBar::setLine(sl_scroll_pos line, sl_bool flagRedraw)
{
	m_line = line;
	if (flagRedraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getMinimumValue()
{
	return m_value_min;
}

void ScrollBar::setMinimumValue(sl_scroll_pos value, sl_bool flagRedraw)
{
	m_value_min = value;
	setValue(m_value, sl_false);
	if (flagRedraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getMaximumValue()
{
	return m_value_max;
}

void ScrollBar::setMaximumValue(sl_scroll_pos value, sl_bool flagRedraw)
{
	m_value_max = value;
	setValue(m_value, sl_false);
	if (flagRedraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getRange()
{
	sl_scroll_pos range = m_value_max - m_value_min - m_page;
	if (range < 0) {
		range = 0;
	}
	return range;
}

void ScrollBar::setRange(sl_scroll_pos range, sl_bool flagRedraw)
{
	if (range < 0) {
		range = 0;
	}
	m_value_max = m_value_min + range;
	setValue(m_value, sl_false);
	if (flagRedraw) {
		invalidate();
	}
}

Ref<Drawable> ScrollBar::getBar()
{
	return m_bar;
}

void ScrollBar::setBar(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_bar = drawable;
	if (flagRedraw) {
		invalidate();
	}
}

Ref<Drawable> ScrollBar::getClickedBar()
{
	return m_clickedBar;
}

void ScrollBar::setClickedBar(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_clickedBar = drawable;
	if (flagRedraw) {
		invalidate();
	}
}

Ref<Drawable> ScrollBar::getHoverBackground()
{
	return m_hoverBackground;
}

void ScrollBar::setHoverBackground(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_hoverBackground = drawable;
	if (flagRedraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getMinimumBarLengthRatio()
{
	return m_bar_len_ratio_min;
}

void ScrollBar::setMinimumBarLengthRatio(sl_scroll_pos ratio)
{
	if (ratio < 0) {
		ratio = 0;
	}
	m_bar_len_ratio_min = ratio;
}

#define CHECK_STATUS(...) \
	sl_scroll_pos value = m_value; \
	sl_scroll_pos page = m_page; \
	sl_scroll_pos range_max = m_value_max; \
	sl_scroll_pos range_min = m_value_min; \
	sl_scroll_pos range = range_max - range_min; \
	if (page < 0) { \
		return __VA_ARGS__; \
	} \
	if (range - page < SLIB_EPSILON) { \
		return __VA_ARGS__; \
	} \
	sl_bool flagVertical = m_orientation == LayoutOrientation::Vertical; \
	sl_ui_len width = getWidth(); \
	if (width < 1) { \
		return __VA_ARGS__; \
	} \
	sl_ui_len height = getHeight(); \
	if (height < 1) { \
		return __VA_ARGS__; \
	} \
	sl_ui_len depth, length; \
	if (flagVertical) { \
		depth = width; \
		length = height; \
	} else { \
		depth = height; \
		length = width; \
	} \
	sl_scroll_pos f_min_bar_len = m_bar_len_ratio_min * (sl_scroll_pos)depth; \
	if (f_min_bar_len >= (sl_scroll_pos)length) { \
		return __VA_ARGS__; \
	} \
	sl_ui_len min_bar_len = (sl_ui_len)(f_min_bar_len); \
	sl_scroll_pos f_bar_len = page * (sl_scroll_pos)length / range; \
	if (f_bar_len < 0 || f_bar_len - (sl_scroll_pos)length > (sl_scroll_pos)SLIB_EPSILON) { \
		return __VA_ARGS__; \
	} \
	sl_ui_len bar_len = (sl_ui_len)(f_bar_len); \
	if (bar_len < min_bar_len) { \
		bar_len = min_bar_len; \
	} \
	if (bar_len > length) { \
		bar_len = length; \
	} \
	sl_scroll_pos ratioValuePos; \
	if (bar_len < length) { \
		ratioValuePos = (range - page) / (sl_scroll_pos)(length - bar_len); \
	} else { \
		ratioValuePos = 0; \
	} \
	SLIB_UNUSED(ratioValuePos) \
	SLIB_UNUSED(value)

sl_bool ScrollBar::getBarPositionRange(sl_ui_pos& pos_begin, sl_ui_pos& pos_end)
{
	CHECK_STATUS(sl_false)
	pos_begin = (sl_ui_pos)((value - range_min) * (sl_scroll_pos)(length - bar_len) / (range - page));
	pos_end = pos_begin + bar_len;
	if (pos_end > (sl_ui_pos)length) {
		pos_end = length;
	}
	if (pos_begin > pos_end - (sl_ui_pos)min_bar_len) {
		pos_begin = pos_end - min_bar_len;
	}
	if (pos_begin < 0) {
		pos_begin = 0;
	}
	if (pos_end < pos_begin + (sl_ui_pos)min_bar_len) {
		pos_end = pos_begin + min_bar_len;
	}
	return sl_true;
}

sl_bool ScrollBar::getBarRegion(UIRect& region)
{
	sl_ui_pos pos_begin, pos_end;
	if (getBarPositionRange(pos_begin, pos_end)) {
		if (isVertical()) {
			region.left = 0;
			region.right = getWidth();
			region.top = pos_begin;
			region.bottom = pos_end;
		} else {
			region.top = 0;
			region.bottom = getHeight();
			region.left = pos_begin;
			region.right = pos_end;
		}
		region.fixSizeError();
		return sl_true;
	}
	return sl_false;
}

sl_scroll_pos ScrollBar::getValueFromBarPosition(sl_ui_pos pos)
{
	CHECK_STATUS(range_min)
	return ((sl_scroll_pos)(pos - bar_len / 2) * ratioValuePos) + m_value_min;
}

sl_bool ScrollBar::isValid()
{
	CHECK_STATUS(sl_false)
	return sl_true;
}

void ScrollBar::onDraw(Canvas* canvas)
{
	UIRect barRegion;
	if (getBarRegion(barRegion)) {
		if (isHoverState()) {
			drawBackground(canvas, getBackgroundColor(), m_hoverBackground);
		} else {
			drawBackground(canvas, getBackgroundColor(), getBackground());
		}
		if (isDownState()) {
			canvas->draw(barRegion, m_clickedBar);
		} else {
			canvas->draw(barRegion, m_bar);
		}
	}
}

void ScrollBar::onMouseEvent(UIEvent* ev)
{
	ev->preventDefault();
	CHECK_STATUS()
	ev->setPreventedDefault(sl_false);
	
	UIAction action = ev->getAction();
	sl_ui_pos pos;
	if (isVertical()) {
		pos = (sl_ui_pos)(ev->getY());
	} else {
		pos = (sl_ui_pos)(ev->getX());
	}
	
	sl_ui_pos pos_begin, pos_end;
	if (getBarPositionRange(pos_begin, pos_end)) {
		if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
			m_posDown = pos;
			if (pos < pos_begin) {
				m_valueDown = getValueFromBarPosition(pos);
				if (page > 0) {
					setValue(value - page);
				} else {
					setValue(m_valueDown);
				}
			} else if (pos <= pos_end) {
				m_valueDown = value;
			} else {
				m_valueDown = getValueFromBarPosition(pos);
				if (page > 0) {
					setValue(value + page);
				} else {
					setValue(m_valueDown);
				}
			}
		} else if (action == UIAction::LeftButtonDrag || action == UIAction::TouchMove) {
			if (isDownState()) {
				setValue(m_valueDown + (sl_scroll_pos)(pos - m_posDown) * ratioValuePos);
			}
		} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd || action == UIAction::TouchCancel) {
			if (isDownState()) {
				if (m_posDown != pos) {
					setValue(m_valueDown + (sl_scroll_pos)(pos - m_posDown) * ratioValuePos);
				}
			}
		}
	}
	
	ev->stopPropagation();
	
}

void ScrollBar::onMouseWheelEvent(UIEvent* ev)
{
	ev->preventDefault();
	CHECK_STATUS()
	ev->setPreventedDefault(sl_false);
	
	sl_scroll_pos line = m_line;
	if (line < SLIB_EPSILON) {
		if (page > 0) {
			line = page / 20;
		} else {
			line = range / 20;
		}
	}
	sl_real delta;
	if (flagVertical) {
		delta = ev->getDeltaY();
	} else {
		delta = ev->getDeltaX();
	}
	if (delta > SLIB_EPSILON) {
		setValue(value - line);
	} else if (delta < -SLIB_EPSILON) {
		setValue(value + line);
	}
	
	ev->stopPropagation();
}

void ScrollBar::onScroll(sl_scroll_pos value)
{
}

void ScrollBar::dispatchScroll(sl_scroll_pos value)
{
	onScroll(value);
	PtrLocker<IScrollBarListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onScroll(this, value);
	}
}

SLIB_UI_NAMESPACE_END
