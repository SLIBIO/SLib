#include "../../../inc/slib/ui/slider_bar.h"

#include "../../../inc/slib/core/math.h"
#include "../../../inc/slib/core/log.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(SliderBar, View)

class _SliderBar_Static
{
public:
	Ref<Drawable> defaultHorizontalBackground;
	Ref<Drawable> defaultVerticalBackground;
	Ref<Drawable> defaultBar;
	Ref<Drawable> defaultClickedBar;
	Ref<Drawable> defaultHoverBar;
	
	_SliderBar_Static()
	{
		defaultHorizontalBackground = HorizontalLineDrawable::create(Pen::createSolidPen(1, Color::Black));
		defaultVerticalBackground = VerticalLineDrawable::create(Pen::createSolidPen(1, Color::Black));
		defaultBar = EllipseDrawable::create(Ref<Pen>::null(), Brush::createSolidBrush(Color(50, 50, 50, 255)));
		defaultClickedBar = EllipseDrawable::create(Ref<Pen>::null(), Brush::createSolidBrush(Color(0, 100, 200, 255)));
		defaultHoverBar = EllipseDrawable::create(Ref<Pen>::null(), Brush::createSolidBrush(Color(0, 200, 100, 255)));
	}
};

SLIB_SAFE_STATIC_GETTER(_SliderBar_Static, _SliderBar_getStatic)

SliderBar::SliderBar(LayoutOrientation orientation)
{
	m_orientation = orientation;
	m_value = 0;
	m_value_min = 0;
	m_value_max = 1;
	
	_SliderBar_Static* s = _SliderBar_getStatic();
	if (s) {
		m_bar = s->defaultBar;
		m_hoverBar = s->defaultHoverBar;
		m_clickedBar = s->defaultClickedBar;
	}
	m_barLengthRatio = 1;
	m_line = 0;
	
	m_flagHoverBar = sl_false;
	
	setFocusable(sl_true);
}

LayoutOrientation SliderBar::getOrientation()
{
	return m_orientation;
}

void SliderBar::setOrientation(LayoutOrientation orientation, sl_bool flagRedraw)
{
	m_orientation = orientation;
	if (flagRedraw) {
		invalidate();
	}
}

sl_bool SliderBar::isVertical()
{
	return m_orientation == LayoutOrientation::Vertical;
}

void SliderBar::setVertical(sl_bool flagRedraw)
{
	setOrientation(LayoutOrientation::Vertical, flagRedraw);
}

sl_bool SliderBar::isHorizontal()
{
	return m_orientation == LayoutOrientation::Horizontal;
}

void SliderBar::setHorizontal(sl_bool flagRedraw)
{
	setOrientation(LayoutOrientation::Horizontal, flagRedraw);
}

sl_slider_bar_value SliderBar::getValue()
{
	return m_value;
}

void SliderBar::setValue(sl_slider_bar_value value, sl_bool flagRedraw)
{
	if (value > m_value_max) {
		value = m_value_max;
	}
	if (value < m_value_min) {
		value = m_value_min;
	}
	if (Math::isAlmostZero(value - m_value)) {
		m_value = value;
		return;
	}
	m_value = value;
	dispatchValueChanged(value);
	if (flagRedraw) {
		invalidate();
	}
}

sl_slider_bar_value SliderBar::getMinimumValue()
{
	return m_value_min;
}

void SliderBar::setMinimumValue(sl_slider_bar_value value, sl_bool flagRedraw)
{
	m_value_min = value;
	setValue(m_value, sl_false);
	if (flagRedraw) {
		invalidate();
	}
}

sl_slider_bar_value SliderBar::getMaximumValue()
{
	return m_value_max;
}

void SliderBar::setMaximumValue(sl_slider_bar_value value, sl_bool flagRedraw)
{
	m_value_max = value;
	setValue(m_value, sl_false);
	if (flagRedraw) {
		invalidate();
	}
}

sl_slider_bar_value SliderBar::getRange()
{
	sl_slider_bar_value range = m_value_max - m_value_min;
	if (range < 0) {
		range = 0;
	}
	return range;
}

void SliderBar::setRange(sl_slider_bar_value range, sl_bool flagRedraw)
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

Ref<Drawable> SliderBar::getBar()
{
	return m_bar;
}

void SliderBar::setBar(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_bar = drawable;
	if (flagRedraw) {
		invalidate();
	}
}

Ref<Drawable> SliderBar::getClickedBar()
{
	return m_clickedBar;
}

void SliderBar::setClickedBar(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_clickedBar = drawable;
	if (flagRedraw) {
		invalidate();
	}
}

Ref<Drawable> SliderBar::getHoverBar()
{
	return m_hoverBar;
}

void SliderBar::setHoverBar(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_hoverBar = drawable;
	if (flagRedraw) {
		invalidate();
	}
}

sl_real SliderBar::getBarLengthRatio()
{
	return m_barLengthRatio;
}

void SliderBar::setBarLengthRatio(sl_real ratio, sl_bool flagRedraw)
{
	m_barLengthRatio = ratio;
	if (flagRedraw) {
		invalidate();
	}
}

sl_slider_bar_value SliderBar::getLine()
{
	return m_line;
}

void SliderBar::setLine(sl_slider_bar_value line)
{
	m_line = line;
}


#define CHECK_STATUS(...) \
	sl_scroll_pos value = m_value; \
	sl_scroll_pos range_max = m_value_max; \
	sl_scroll_pos range_min = m_value_min; \
	sl_scroll_pos range = range_max - range_min; \
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
	sl_ui_pos paddingStartDepth, paddingEndDepth, paddingStartLength, paddingEndLength; \
	if (flagVertical) { \
		paddingStartDepth = getPaddingLeft(); \
		paddingEndDepth = getPaddingRight(); \
		depth = width - paddingStartDepth - paddingEndDepth; \
		paddingStartLength = getPaddingTop(); \
		paddingEndLength = getPaddingBottom(); \
		length = height - paddingStartLength - paddingEndLength; \
	} else { \
		paddingStartDepth = getPaddingTop(); \
		paddingEndDepth = getPaddingBottom(); \
		depth = height - paddingStartDepth - paddingEndDepth; \
		paddingStartLength = getPaddingLeft(); \
		paddingEndLength = getPaddingRight(); \
		length = width - paddingStartLength - paddingEndLength; \
	} \
	sl_slider_bar_value bar_len = m_barLengthRatio * (sl_scroll_pos)depth; \
	sl_scroll_pos ratioValuePos; \
	if (bar_len < length) { \
		ratioValuePos = range / (sl_scroll_pos)(length - bar_len); \
	} else { \
		ratioValuePos = 0; \
	} \
	SLIB_UNUSED(ratioValuePos) \
	SLIB_UNUSED(value)

sl_slider_bar_value SliderBar::getValueFromBarPosition(sl_ui_pos pos)
{
	CHECK_STATUS(sl_false)
	return ((sl_slider_bar_value)(pos - paddingStartLength - bar_len / 2) * ratioValuePos) + m_value_min;
}

sl_bool SliderBar::isValid()
{
	CHECK_STATUS(sl_false)
	return sl_true;
}

sl_bool SliderBar::getBarPositionRange(sl_ui_pos& pos_begin, sl_ui_pos& pos_end)
{
	CHECK_STATUS(sl_false)
	pos_begin = (sl_ui_pos)((value - range_min) * (sl_scroll_pos)(length - bar_len) / range);
	pos_end = pos_begin + bar_len;
	if (pos_end > (sl_ui_pos)(length)) {
		pos_end = length;
	}
	if (pos_begin > (sl_ui_pos)(pos_end - bar_len)) {
		pos_begin = pos_end - bar_len;
	}
	if (pos_begin < 0) {
		pos_begin = 0;
	}
	if (pos_end < (sl_ui_pos)(pos_begin + bar_len)) {
		pos_end = pos_begin + bar_len;
	}
	pos_begin += paddingStartLength;
	pos_end += paddingStartLength;
	return sl_true;
}

sl_bool SliderBar::getBarRegion(UIRect& region)
{
	sl_ui_pos paddingLeft = getPaddingLeft();
	sl_ui_pos paddingTop = getPaddingTop();
	sl_ui_pos paddingRight = getPaddingRight();
	sl_ui_pos paddingBottom = getPaddingBottom();
	
	sl_ui_pos pos_begin, pos_end;
	if (getBarPositionRange(pos_begin, pos_end)) {
		if (isVertical()) {
			region.left = paddingLeft;
			region.right = getWidth() - paddingRight;
			region.top = pos_begin;
			region.bottom = pos_end;
		} else {
			region.top = paddingTop;
			region.bottom = getHeight() - paddingBottom;
			region.left = pos_begin;
			region.right = pos_end;
		}
		region.fixSizeError();
		return sl_true;
	}
	return sl_false;
}

void SliderBar::onDraw(Canvas* canvas)
{
	Ref<Drawable> background = getBackground();
	if (background.isNull()) {
		_SliderBar_Static* s = _SliderBar_getStatic();
		if (s) {
			if (m_orientation == LayoutOrientation::Horizontal) {
				canvas->draw(getBounds(), s->defaultHorizontalBackground);
			} else {
				canvas->draw(getBounds(), s->defaultVerticalBackground);
			}
		}
	}
	UIRect barRegion;
	if (getBarRegion(barRegion)) {
		if (isDownState()) {
			canvas->draw(barRegion, m_clickedBar);
		} else if (m_flagHoverBar) {
			canvas->draw(barRegion, m_hoverBar);
		} else {
			canvas->draw(barRegion, m_bar);
		}
	}
}

void SliderBar::onMouseEvent(UIEvent* ev)
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
		if (action == UIAction::MouseMove || action == UIAction::MouseEnter) {
			if (pos >= pos_begin && pos <= pos_end) {
				if (!m_flagHoverBar) {
					m_flagHoverBar = sl_true;
					invalidate();
				}
			} else {
				if (m_flagHoverBar) {
					m_flagHoverBar = sl_false;
					invalidate();
				}
			}
			return;
		} else if (action == UIAction::MouseLeave) {
			m_flagHoverBar = sl_false;
			invalidate();
			return;
		} else if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
			m_posDown = pos;
			if (pos < pos_begin) {
				m_valueDown = getValueFromBarPosition(pos);
				setValue(m_valueDown);
			} else if (pos <= pos_end) {
				m_valueDown = value;
			} else {
				m_valueDown = getValueFromBarPosition(pos);
				setValue(m_valueDown);
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

void SliderBar::onMouseWheelEvent(UIEvent* ev)
{
	ev->preventDefault();
	CHECK_STATUS()
	ev->setPreventedDefault(sl_false);
	
	sl_slider_bar_value line = m_line;
	if (line < SLIB_EPSILON) {
		line = range * 0.05f;
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

void SliderBar::onValueChanged(sl_slider_bar_value value)
{
}

void SliderBar::dispatchValueChanged(sl_slider_bar_value value)
{
	onValueChanged(value);
	PtrLocker<ISliderBarListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onValueChanged(this, value);
	}
}

void SliderBar::onKeyEvent(UIEvent* ev)
{
	ev->preventDefault();
	CHECK_STATUS()
	ev->setPreventedDefault(sl_false);
	
	sl_slider_bar_value line = m_line;
	if (line < SLIB_EPSILON) {
		line = range / 20;
	}
	
	Keycode key = ev->getKeycode();
	if (key == Keycode::Left) {
		setValue(value - line);
	} else if (key == Keycode::Right) {
		setValue(value + line);
	} else if (key == Keycode::Up) {
		setValue(value - line);
	} else if (key == Keycode::Down) {
		setValue(value + line);
	}
	ev->stopPropagation();
}

SLIB_UI_NAMESPACE_END
