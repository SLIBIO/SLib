#include "../../../inc/slib/ui/scroll_bar.h"

#include "../../../inc/slib/core/math.h"
#include "../../../inc/slib/core/safe_static.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ScrollBar, View)

class _ScrollBar_Static
{
public:
	Ref<Drawable> defaultThumb;
	Ref<Drawable> defaultPressedThumb;
	Ref<Drawable> defaultHoverThumb;
	
	Ref<Drawable> defaultHoverTrack;
	Ref<Drawable> defaultPressedTrack;

	_ScrollBar_Static()
	{
		defaultThumb = ColorDrawable::create(Color(0, 0, 0, 150));
		defaultPressedThumb = ColorDrawable::create(Color(0, 0, 0, 200));
		defaultHoverThumb = ColorDrawable::create(Color(0, 50, 255, 110));
		
		defaultHoverTrack = ColorDrawable::create(Color(255, 255, 255, 50));
		defaultPressedTrack = ColorDrawable::create(Color(255, 255, 255, 100));
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
		m_thumb = s->defaultThumb;
		m_pressedThumb = s->defaultPressedThumb;
		m_hoverThumb = s->defaultHoverThumb;
		
		m_hoverTrack = s->defaultHoverTrack;
		m_pressedTrack = s->defaultPressedTrack;
	}
	
	m_thumb_len_ratio_min = 2;
	
	m_flagHoverThumb = sl_false;
	
}

LayoutOrientation ScrollBar::getOrientation()
{
	return m_orientation;
}

void ScrollBar::setOrientation(LayoutOrientation orientation, UIUpdateMode mode)
{
	m_orientation = orientation;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

sl_bool ScrollBar::isVertical()
{
	return m_orientation == LayoutOrientation::Vertical;
}

void ScrollBar::setVertical(UIUpdateMode mode)
{
	setOrientation(LayoutOrientation::Vertical, mode);
}

sl_bool ScrollBar::isHorizontal()
{
	return m_orientation == LayoutOrientation::Horizontal;
}

void ScrollBar::setHorizontal(UIUpdateMode mode)
{
	setOrientation(LayoutOrientation::Horizontal, mode);
}

sl_scroll_pos ScrollBar::getValue()
{
	return m_value;
}

void ScrollBar::setValue(sl_scroll_pos value, UIUpdateMode mode)
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
	dispatchChange(value);
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void ScrollBar::setValueOfOutRange(sl_scroll_pos value, UIUpdateMode mode)
{
	m_value = value;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getPage()
{
	return m_page;
}

void ScrollBar::setPage(sl_scroll_pos page, UIUpdateMode mode)
{
	m_page = page;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getLine()
{
	return m_line;
}

void ScrollBar::setLine(sl_scroll_pos line, UIUpdateMode mode)
{
	m_line = line;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getMinimumValue()
{
	return m_value_min;
}

void ScrollBar::setMinimumValue(sl_scroll_pos value, UIUpdateMode mode)
{
	m_value_min = value;
	setValue(m_value, UIUpdateMode::NoRedraw);
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

sl_scroll_pos ScrollBar::getMaximumValue()
{
	return m_value_max;
}

void ScrollBar::setMaximumValue(sl_scroll_pos value, UIUpdateMode mode)
{
	m_value_max = value;
	setValue(m_value, UIUpdateMode::NoRedraw);
	if (mode == UIUpdateMode::Redraw) {
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

void ScrollBar::setRange(sl_scroll_pos range, UIUpdateMode mode)
{
	if (range < 0) {
		range = 0;
	}
	m_value_max = m_value_min + range;
	setValue(m_value, UIUpdateMode::NoRedraw);
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

Ref<Drawable> ScrollBar::getThumbDrawable()
{
	return m_thumb;
}

void ScrollBar::setThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	m_thumb = drawable;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void ScrollBar::setThumbColor(const Color& color, UIUpdateMode mode)
{
	setThumbDrawable(ColorDrawable::create(color), mode);
}

Ref<Drawable> ScrollBar::getPressedThumbDrawable()
{
	return m_pressedThumb;
}

void ScrollBar::setPressedThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	m_pressedThumb = drawable;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void ScrollBar::setPressedThumbColor(const Color& color, UIUpdateMode mode)
{
	setPressedThumbDrawable(ColorDrawable::create(color), mode);
}

Ref<Drawable> ScrollBar::getHoverThumbDrawable()
{
	return m_hoverThumb;
}

void ScrollBar::setHoverThumbDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	m_hoverThumb = drawable;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void ScrollBar::setHoverThumbColor(const Color& color, UIUpdateMode mode)
{
	setHoverThumbDrawable(ColorDrawable::create(color), mode);
}

Ref<Drawable> ScrollBar::getTrackDrawable()
{
	return m_track;
}

void ScrollBar::setTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	m_track = drawable;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void ScrollBar::setTrackColor(const Color& color, UIUpdateMode mode)
{
	setTrackDrawable(ColorDrawable::create(color), mode);
}

Ref<Drawable> ScrollBar::getPressedTrackDrawable()
{
	return m_pressedTrack;
}

void ScrollBar::setPressedTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	m_pressedTrack = drawable;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void ScrollBar::setPressedTrackColor(const Color& color, UIUpdateMode mode)
{
	setPressedTrackDrawable(ColorDrawable::create(color), mode);
}

Ref<Drawable> ScrollBar::getHoverTrackDrawable()
{
	return m_hoverTrack;
}

void ScrollBar::setHoverTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
{
	m_hoverTrack = drawable;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void ScrollBar::setHoverTrackColor(const Color& color, UIUpdateMode mode)
{
	setHoverTrackDrawable(ColorDrawable::create(color), mode);
}

float ScrollBar::getMinimumThumbLengthRatio()
{
	return m_thumb_len_ratio_min;
}

void ScrollBar::setMinimumThumbLengthRatio(float ratio)
{
	if (ratio < 0) {
		ratio = 0;
	}
	m_thumb_len_ratio_min = ratio;
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
	sl_ui_pos width = getWidth() - getPaddingLeft() - getPaddingTop(); \
	if (width < 1) { \
		return __VA_ARGS__; \
	} \
	sl_ui_pos height = getHeight() - getPaddingTop() - getPaddingBottom(); \
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
	float f_min_thumb_len = (float)(m_thumb_len_ratio_min * (float)depth); \
	if (f_min_thumb_len < 0 || f_min_thumb_len >= (float)length) { \
		return __VA_ARGS__; \
	} \
	sl_ui_len min_thumb_len = (sl_ui_len)(f_min_thumb_len); \
	float f_thumb_len = (float)(page * (sl_scroll_pos)length / range); \
	if (f_thumb_len < 0 || f_thumb_len - (float)length > SLIB_EPSILON) { \
		return __VA_ARGS__; \
	} \
	sl_ui_len thumb_len = (sl_ui_len)(f_thumb_len); \
	if (thumb_len < min_thumb_len) { \
		thumb_len = min_thumb_len; \
	} \
	if (thumb_len > length) { \
		thumb_len = length; \
	} \
	sl_scroll_pos ratioValuePos; \
	if (thumb_len < length) { \
		ratioValuePos = (range - page) / (sl_scroll_pos)(length - thumb_len); \
	} else { \
		ratioValuePos = 0; \
	} \
	SLIB_UNUSED(ratioValuePos) \
	SLIB_UNUSED(value)

sl_bool ScrollBar::getThumbPositionRange(sl_ui_pos& _pos_begin, sl_ui_pos& _pos_end)
{
	CHECK_STATUS(sl_false)
	sl_ui_pos pos_begin = (sl_ui_pos)((value - range_min) * (sl_scroll_pos)(length - thumb_len) / (range - page));
	sl_ui_pos pos_end = pos_begin + thumb_len;
	if (pos_end > (sl_ui_pos)length) {
		pos_end = length;
	}
	if (pos_begin > pos_end - (sl_ui_pos)min_thumb_len) {
		pos_begin = pos_end - min_thumb_len;
	}
	if (pos_begin < 0) {
		pos_begin = 0;
	}
	if (pos_end < pos_begin + (sl_ui_pos)min_thumb_len) {
		pos_end = pos_begin + min_thumb_len;
	}
	if (flagVertical) {
		sl_ui_pos padding = getPaddingTop();
		pos_begin += padding;
		pos_end += padding;
	} else {
		sl_ui_pos padding = getPaddingLeft();
		pos_begin += padding;
		pos_end += padding;
	}
	_pos_begin = pos_begin;
	_pos_end = pos_end;
	return sl_true;
}

sl_bool ScrollBar::getThumbRegion(UIRect& region)
{
	sl_ui_pos pos_begin, pos_end;
	if (getThumbPositionRange(pos_begin, pos_end)) {
		if (isVertical()) {
			region.left = getPaddingLeft();
			region.right = getWidth() - getPaddingRight();
			region.top = pos_begin;
			region.bottom = pos_end;
		} else {
			region.top = getPaddingTop();
			region.bottom = getHeight() - getPaddingBottom();
			region.left = pos_begin;
			region.right = pos_end;
		}
		region.fixSizeError();
		return sl_true;
	}
	return sl_false;
}

sl_scroll_pos ScrollBar::getValueFromThumbPosition(sl_ui_pos pos)
{
	CHECK_STATUS(range_min)
	if (flagVertical) {
		pos -= getPaddingTop();
	} else {
		pos -= getPaddingLeft();
	}
	return ((sl_scroll_pos)(pos - thumb_len / 2) * ratioValuePos) + m_value_min;
}

sl_bool ScrollBar::isValid()
{
	CHECK_STATUS(sl_false)
	return sl_true;
}

void ScrollBar::onDraw(Canvas* canvas)
{
	Ref<Drawable> track;
	if (isPressedState()) {
		track = m_pressedTrack;
	} else if (isHoverState()) {
		track = m_hoverTrack;
	}
	if (track.isNull()) {
		track = m_track;
	}
	if (track.isNotNull()) {
		canvas->draw(getBoundsInnerPadding(), track);
	}
	UIRect thumbRegion;
	if (getThumbRegion(thumbRegion)) {
		Ref<Drawable> thumb;
		if (isPressedState()) {
			thumb = m_pressedThumb;
		} else if (isHoverState() && m_flagHoverThumb) {
			thumb = m_hoverThumb;
		}
		if (thumb.isNull()) {
			thumb = m_thumb;
		}
		if (thumb.isNotNull()) {
			Color color;
			if (ColorDrawable::check(thumb, &color)) {
				sl_bool flagAntiAlias = canvas->isAntiAlias();
				canvas->setAntiAlias(sl_true);
				sl_real r = Math::min(thumbRegion.getWidth(), thumbRegion.getHeight()) * 0.5f;
				sl_real padding = 2;
				r -= padding;
				canvas->fillRoundRect(Rectangle(thumbRegion.left + padding - 1, thumbRegion.top + padding, thumbRegion.right - padding, thumbRegion.bottom - padding), Size(r, r), color);
				canvas->setAntiAlias(flagAntiAlias);
			} else {
				canvas->draw(thumbRegion, thumb);
			}
		}
	}
}

void ScrollBar::onMouseEvent(UIEvent* ev)
{
	ev->preventDefault();
	CHECK_STATUS()
	ev->setPreventedDefault(sl_false);
	
	sl_ui_pos pos_begin, pos_end;
	if (!(getThumbPositionRange(pos_begin, pos_end))) {
		ev->preventDefault();
		return;
	}

	UIAction action = ev->getAction();
	sl_ui_pos pos;
	if (isVertical()) {
		pos = (sl_ui_pos)(ev->getY());
	} else {
		pos = (sl_ui_pos)(ev->getX());
	}
	
	switch (action) {
		case UIAction::MouseEnter:
		case UIAction::MouseMove:
			{
				UIRect region;
				if (getThumbRegion(region)) {
					if (region.containsPoint(ev->getPoint())) {
						_setHoverThumb(sl_true);
						return;
					}
				}
				_setHoverThumb(sl_false);
				return;
			}
		case UIAction::MouseLeave:
			_setHoverThumb(sl_false);
			return;
			
		case UIAction::LeftButtonDown:
		case UIAction::TouchBegin:
			{
				m_posDown = pos;
				if (pos < pos_begin) {
					m_valueDown = getValueFromThumbPosition(pos);
					if (page > 0) {
						setValue(value - page);
					} else {
						setValue(m_valueDown);
					}
				} else if (pos <= pos_end) {
					m_valueDown = value;
				} else {
					m_valueDown = getValueFromThumbPosition(pos);
					if (page > 0) {
						setValue(value + page);
					} else {
						setValue(m_valueDown);
					}
				}
				break;
			}
		case UIAction::LeftButtonDrag:
		case UIAction::TouchMove:
			if (isPressedState()) {
				setValue(m_valueDown + (sl_scroll_pos)(pos - m_posDown) * ratioValuePos);
			}
			break;
		case UIAction::LeftButtonUp:
		case UIAction::TouchEnd:
		case UIAction::TouchCancel:
			if (isPressedState()) {
				if (m_posDown != pos) {
					setValue(m_valueDown + (sl_scroll_pos)(pos - m_posDown) * ratioValuePos);
				}
			}
			break;
		default:
			return;
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

void ScrollBar::onChange(sl_scroll_pos value)
{
}

void ScrollBar::dispatchChange(sl_scroll_pos value)
{
	onChange(value);
	PtrLocker<IScrollBarListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onChange(this, value);
	}
	getOnChange()(value);
}

void ScrollBar::_setHoverThumb(sl_bool flag)
{
	if (m_flagHoverThumb != flag) {
		m_flagHoverThumb = flag;
		invalidate();
	}
}

SLIB_UI_NAMESPACE_END
