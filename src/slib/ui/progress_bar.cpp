#include "../../../inc/slib/ui/progress_bar.h"

#include "../../../inc/slib/core/math.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ProgressBar, View)

class _ProgressBar_Bar : public Drawable
{
public:
	Ref<Brush> m_brush;
	
public:
	_ProgressBar_Bar(const Color& color)
	{
		m_brush = Brush::createSolidBrush(color);
	}
	
public:
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst)
	{
		canvas->fillRectangle(rectDst, m_brush);
	}
};

class _ProgressBar_Static
{
public:
	Ref<Drawable> defaultBar;
	
	_ProgressBar_Static()
	{
		defaultBar = new _ProgressBar_Bar(Color(150, 150, 150));
	}
};

SLIB_SAFE_STATIC_GETTER(_ProgressBar_Static, _ProgressBar_getStatic)

ProgressBar::ProgressBar()
{
	m_orientation = LayoutOrientation::Horizontal;
	m_gravity = Alignment::TopLeft;
	m_value = 0;
	m_value_min = 0;
	m_value_max = 1;
	
	_ProgressBar_Static* s = _ProgressBar_getStatic();
	if (s) {
		m_bar = s->defaultBar;
	}

	setBackgroundColor(Color(220, 220, 220));
	
}

LayoutOrientation ProgressBar::getOrientation()
{
	return m_orientation;
}

void ProgressBar::setOrientation(LayoutOrientation orientation, sl_bool flagRedraw)
{
	m_orientation = orientation;
	if (flagRedraw) {
		invalidate();
	}
}

Alignment ProgressBar::getGravity()
{
	return m_gravity;
}

void ProgressBar::setGravity(Alignment gravity, sl_bool flagRedraw)
{
	m_gravity = gravity;
	if (flagRedraw) {
		invalidate();
	}
}

sl_progress_bar_value ProgressBar::getValue()
{
	return m_value;
}

void ProgressBar::setValue(sl_progress_bar_value value, sl_bool flagRedraw)
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
	if (flagRedraw) {
		invalidate();
	}
}

sl_progress_bar_value ProgressBar::getMinimumValue()
{
	return m_value_min;
}

void ProgressBar::setMinimumValue(sl_progress_bar_value value, sl_bool flagRedraw)
{
	m_value_min = value;
	setValue(m_value, sl_false);
	if (flagRedraw) {
		invalidate();
	}
}

sl_progress_bar_value ProgressBar::getMaximumValue()
{
	return m_value_max;
}

void ProgressBar::setMaximumValue(sl_progress_bar_value value, sl_bool flagRedraw)
{
	m_value_max = value;
	setValue(m_value, sl_false);
	if (flagRedraw) {
		invalidate();
	}
}

sl_progress_bar_value ProgressBar::getRange()
{
	sl_scroll_pos range = m_value_max - m_value_min;
	if (range < 0) {
		range = 0;
	}
	return range;
}

void ProgressBar::setRange(sl_progress_bar_value range, sl_bool flagRedraw)
{
	setMaximumValue(m_value_min + range, flagRedraw);
}

Ref<Drawable> ProgressBar::getBar()
{
	return m_bar;
}

void ProgressBar::setBar(const Ref<Drawable>& drawable, sl_bool flagRedraw)
{
	m_bar = drawable;
	if (flagRedraw) {
		invalidate();
	}
}

UIRect ProgressBar::getBarRegion()
{
	sl_ui_pos paddingLeft = getPaddingLeft();
	sl_ui_pos paddingTop = getPaddingTop();
	sl_ui_pos width = getWidth() - paddingLeft - getPaddingRight();
	sl_ui_pos height = getHeight() - paddingTop - getPaddingBottom();
	if (width <= 0 || height <= 0) {
		return UIRect::zero();
	}
	Alignment gravity = m_gravity;
	LayoutOrientation orientation = m_orientation;
	if (orientation == LayoutOrientation::Horizontal) {
		sl_ui_pos barPos, barLen;
		sl_progress_bar_value range = m_value_max - m_value_min;
		if (range > SLIB_EPSILON) {
			barLen = (sl_ui_pos)(width * (m_value - m_value_min) / range);
		} else {
			barLen = 0;
		}
		Alignment align = gravity & Alignment::HorizontalMask;
		if (align == Alignment::Left) {
			barPos = 0;
		} else if (align == Alignment::Right) {
			barPos = width - barLen;
		} else {
			barPos = (width - barLen) / 2;
		}
		return UIRect(paddingLeft + barPos, paddingTop, paddingLeft + barPos + barLen, paddingTop + height);
	} else {
		sl_ui_pos barPos, barLen;
		sl_progress_bar_value range = m_value_max - m_value_min;
		if (range > SLIB_EPSILON) {
			barLen = (sl_ui_pos)(height * (m_value - m_value_min) / range);
		} else {
			barLen = 0;
		}
		Alignment align = gravity & Alignment::VerticalMask;
		if (align == Alignment::Top) {
			barPos = 0;
		} else if (align == Alignment::Bottom) {
			barPos = height - barLen;
		} else {
			barPos = (height - barLen) / 2;
		}
		return UIRect(paddingLeft, paddingTop + barPos, paddingLeft + width, paddingTop + barPos + barLen);
	}
}

void ProgressBar::onDraw(Canvas* canvas)
{
	drawBackground(canvas, getBackgroundColor(), getBackground());
	canvas->draw(getBarRegion(), m_bar);
}

SLIB_UI_NAMESPACE_END
