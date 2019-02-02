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

#include "slib/ui/progress_bar.h"

#include "slib/core/math.h"
#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(ProgressBar, View)
	
	class _priv_ProgressBar_Static
	{
	public:
		Ref<Drawable> defaultTrack;
		Ref<Drawable> defaultProgress;
		Ref<Drawable> defaultProgress2;
		
		_priv_ProgressBar_Static()
		{
			defaultTrack = ColorDrawable::create(Color(220, 220, 220));
			defaultProgress = ColorDrawable::create(Color(150, 150, 150));
			defaultProgress2 = ColorDrawable::create(Color(200, 200, 200));
		}
	};
	
	SLIB_SAFE_STATIC_GETTER(_priv_ProgressBar_Static, _priv_ProgressBar_getStatic)
	
	ProgressBar::ProgressBar(LayoutOrientation orientation)
	{
		
		SLIB_REFERABLE_CONSTRUCTOR
		
		setSavingCanvasState(sl_false);
		
		m_orientation = orientation;
		m_value_min = 0;
		m_value_max = 1;
		m_value = 0;
		m_value2 = 0;
		m_flagDualValues = sl_false;
		m_flagDiscrete = sl_false;
		m_step = 0;
		m_flagReversed = sl_false;
		
		_priv_ProgressBar_Static* s = _priv_ProgressBar_getStatic();
		if (s) {
			m_track = s->defaultTrack;
			m_progress = s->defaultProgress;
			m_progress2 = s->defaultProgress2;
		}
	}
	
	ProgressBar::~ProgressBar()
	{
	}

	LayoutOrientation ProgressBar::getOrientation()
	{
		return m_orientation;
	}
	
	void ProgressBar::setOrientation(LayoutOrientation orientation, UIUpdateMode mode)
	{
		m_orientation = orientation;
		invalidate(mode);
	}
	
	sl_bool ProgressBar::isVertical()
	{
		return m_orientation == LayoutOrientation::Vertical;
	}
	
	void ProgressBar::setVertical(UIUpdateMode mode)
	{
		setOrientation(LayoutOrientation::Vertical, mode);
	}
	
	sl_bool ProgressBar::isHorizontal()
	{
		return m_orientation == LayoutOrientation::Horizontal;
	}
	
	void ProgressBar::setHorizontal(UIUpdateMode mode)
	{
		setOrientation(LayoutOrientation::Horizontal, mode);
	}
	
	float ProgressBar::getMinimumValue()
	{
		return m_value_min;
	}
	
	void ProgressBar::setMinimumValue(float value, UIUpdateMode mode)
	{
		m_value_min = value;
		setValue(m_value, UIUpdateMode::None);
		invalidate(mode);
	}
	
	float ProgressBar::getMaximumValue()
	{
		return m_value_max;
	}
	
	void ProgressBar::setMaximumValue(float value, UIUpdateMode mode)
	{
		m_value_max = value;
		setValue(m_value, UIUpdateMode::None);
		invalidate(mode);
	}
	
	float ProgressBar::getValue()
	{
		return m_value;
	}
	
	void ProgressBar::setValue(float value, UIUpdateMode mode)
	{
		if (tryChangeValue(value, m_value2, sl_false)) {
			m_value = value;
			invalidate(mode);
		}
	}
	
	float ProgressBar::getSecondaryValue()
	{
		return m_value2;
	}
	
	void ProgressBar::setSecondaryValue(float value, UIUpdateMode mode)
	{
		if (tryChangeValue(m_value, value, sl_true)) {
			m_value2 = value;
			invalidate(mode);
		}
	}
	
	sl_bool ProgressBar::isDualValues()
	{
		return m_flagDualValues;
	}
	
	void ProgressBar::setDualValues(sl_bool flagDualValues, UIUpdateMode mode)
	{
		m_flagDualValues = flagDualValues;
		tryChangeValue(m_value, m_value2, sl_false);
		invalidate(mode);
	}
	
	sl_bool ProgressBar::isDiscrete()
	{
		return m_flagDiscrete;
	}
	
	void ProgressBar::setDiscrete(sl_bool flagDiscrete, UIUpdateMode mode)
	{
		m_flagDiscrete = flagDiscrete;
		setSecondaryValue(m_value2, UIUpdateMode::None);
		setValue(m_value, mode);
	}
	
	float ProgressBar::getStep()
	{
		return m_step;
	}
	
	void ProgressBar::setStep(float step, UIUpdateMode mode)
	{
		m_step = step;
		setSecondaryValue(m_value2, UIUpdateMode::None);
		setValue(m_value, mode);
	}
	
	sl_bool ProgressBar::isReversed()
	{
		return m_flagReversed;
	}
	
	void ProgressBar::setReversed(sl_bool flagReversed, UIUpdateMode mode)
	{
		m_flagReversed = flagReversed;
		invalidate(mode);
	}
	
	Ref<Drawable> ProgressBar::getTrackDrawable()
	{
		return m_track;
	}
	
	void ProgressBar::setTrackDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		m_track = drawable;
		invalidate(mode);
	}
	
	void ProgressBar::setTrackColor(const Color& color, UIUpdateMode mode)
	{
		setTrackDrawable(ColorDrawable::create(color), mode);
	}
	
	Ref<Drawable> ProgressBar::getProgressDrawable()
	{
		return m_progress;
	}
	
	void ProgressBar::setProgressDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		m_progress = drawable;
		invalidate(mode);
	}
	
	void ProgressBar::setProgressColor(const Color &color, UIUpdateMode mode)
	{
		setProgressDrawable(ColorDrawable::create(color), mode);
	}
	
	Ref<Drawable> ProgressBar::getSecondaryProgressDrawable()
	{
		return m_progress2;
	}
	
	void ProgressBar::setSecondaryProgressDrawable(const Ref<Drawable>& drawable, UIUpdateMode mode)
	{
		m_progress2 = drawable;
		invalidate(mode);
	}
	
	void ProgressBar::setSecondaryProgressColor(const Color &color, UIUpdateMode mode)
	{
		setSecondaryProgressDrawable(ColorDrawable::create(color), mode);
	}
	
	void ProgressBar::onDraw(Canvas* canvas)
	{
		Ref<Drawable> track = m_track;
		if (track.isNotNull()) {
			canvas->draw(getBoundsInnerPadding(), track);
		}
		Ref<Drawable> progress = m_progress;
		Ref<Drawable> progress2;
		if (m_flagDualValues) {
			progress2 = m_progress2;
		}
		if (progress.isNotNull() || progress2.isNotNull()) {
			UIRect rc, rc2;
			getProgressRegions(rc, rc2);
			if (progress2.isNotNull() && rc2.isValidSize()) {
				canvas->draw(rc2, progress2);
			}
			if (progress.isNotNull() && rc.isValidSize()) {
				canvas->draw(rc, progress);
			}
		}
	}
	
	void ProgressBar::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		Ref<Font> font = getFont();

		if (flagHorizontal) {
			sl_ui_pos width = getPaddingLeft() + getPaddingRight();
			if (font.isNotNull()) {
				sl_ui_pos t = (sl_ui_pos)(font->getFontHeight());
				if (t > 0) {
					width += t * 10;
				}
			}
			if (width < 0) {
				width = 0;
			}
			setLayoutWidth(width);
		}
		if (flagVertical) {
			sl_ui_pos height = getPaddingTop() + getPaddingBottom();
			if (font.isNotNull()) {
				sl_ui_pos t = (sl_ui_pos)(font->getFontHeight());
				if (t > 0) {
					height += t;
				}
			}
			if (height < 0) {
				height = 0;
			}
			setLayoutHeight(height);
		}
	}
	
	float ProgressBar::refineValue(float value)
	{
		if (m_flagDiscrete) {
			float step = refineStep();
			if (step > SLIB_EPSILON) {
				value = (float)((int)((value - m_value_min) / step) * step + m_value_min);
			}
		}
		if (value > m_value_max) {
			value = m_value_max;
		}
		if (value < m_value_min) {
			value = m_value_min;
		}
		return value;
	}
	
	int ProgressBar::tryChangeValue(float& value, float& value2, sl_bool flagChange2)
	{
		int ret = 0;
		float newValue = refineValue(value);
		if (m_flagDualValues) {
			float newValue2 = refineValue(value2);
			if (flagChange2) {
				if (newValue > newValue2) {
					newValue = newValue2;
				}
			} else {
				if (newValue > newValue2) {
					newValue2 = newValue;
				}
			}
			if (!(Math::isAlmostZero(newValue2 - m_value2))) {
				ret |= 2;
			}
			value2 = newValue2;
		}
		if (!(Math::isAlmostZero(newValue - m_value))) {
			ret |= 1;
		}
		value = newValue;
		return ret;
	}
	
	float ProgressBar::refineStep()
	{
		sl_real step = m_step;
		if (step > SLIB_EPSILON) {
			return step;
		}
		float range = m_value_max - m_value_min;
		if (m_flagDiscrete && range > 1) {
			return 1;
		} else {
			step = range / 20;
		}
		return step;
	}
	
	sl_ui_pos ProgressBar::getPositionFromValue(float value)
	{
		sl_ui_pos paddingLeft = getPaddingLeft();
		sl_ui_pos paddingTop = getPaddingTop();
		sl_ui_pos width = getWidth() - paddingLeft - getPaddingRight();
		sl_ui_pos height = getHeight() - paddingTop - getPaddingBottom();
		if (width <= 0 || height <= 0) {
			return paddingLeft;
		}
		float range = m_value_max - m_value_min;
		if (range < SLIB_EPSILON) {
			return paddingLeft;
		}
		sl_ui_pos len, padding;
		if (m_orientation == LayoutOrientation::Horizontal) {
			len = width;
			padding = paddingLeft;
		} else {
			len = height;
			padding = paddingTop;
		}
		sl_ui_pos pos = (sl_ui_pos)(len * (value - m_value_min) / range);
		if (m_flagReversed) {
			return padding + len - pos;
		} else {
			return pos + padding;
		}
	}
	
	void ProgressBar::getProgressRegions(UIRect& outProgress, UIRect& outSecondaryProgress)
	{
		sl_ui_pos pos1 = getPositionFromValue(m_value);
		sl_ui_pos pos2 = 0;
		if (m_flagDualValues && m_value2 > m_value) {
			pos2 = getPositionFromValue(m_value2);
		}
		if (m_orientation == LayoutOrientation::Horizontal) {
			if (m_flagReversed) {
				outProgress.left = pos1;
				outProgress.right = getWidth() - getPaddingRight();
				outSecondaryProgress.right = outProgress.left;
				outSecondaryProgress.left = pos2;
			} else {
				outProgress.left = getPaddingLeft();
				outProgress.right = pos1;
				outSecondaryProgress.left = outProgress.right;
				outSecondaryProgress.right = pos2;
			}
			outProgress.top = getPaddingTop();
			outProgress.bottom = getHeight() - getPaddingBottom();
			outSecondaryProgress.top = outProgress.top;
			outSecondaryProgress.bottom = outProgress.bottom;
		} else {
			if (m_flagReversed) {
				outProgress.top = pos1;
				outProgress.bottom = getHeight() - getPaddingBottom();
				outSecondaryProgress.top = pos2;
				outSecondaryProgress.bottom = outProgress.top;
			} else {
				outProgress.top = getPaddingTop();
				outProgress.bottom = pos1;
				outSecondaryProgress.top = outProgress.bottom;
				outSecondaryProgress.bottom = pos2;
			}
			outProgress.left = getPaddingLeft();
			outProgress.right = getWidth() - getPaddingRight();
			outSecondaryProgress.left = outProgress.left;
			outSecondaryProgress.right = outProgress.right;
		}
	}
	
}
