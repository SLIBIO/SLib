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

#include "slib/ui/switch_view.h"

#include "slib/ui/core.h"

#include "slib/core/math.h"
#include "slib/core/timer.h"
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace switch_view
		{

			class StaticContext
			{
			public:
				Color textColorOff;
				Color textColorOn;
				
				Ref<Drawable> thumbOff;
				Ref<Drawable> trackOff;
				Ref<Drawable> pressedThumbOff;
				Ref<Drawable> pressedTrackOff;
				Ref<Drawable> hoverThumbOff;
				Ref<Drawable> hoverTrackOff;
				Ref<Drawable> thumbOn;
				Ref<Drawable> trackOn;
				Ref<Drawable> pressedThumbOn;
				Ref<Drawable> pressedTrackOn;
				Ref<Drawable> hoverThumbOn;
				Ref<Drawable> hoverTrackOn;

				StaticContext(sl_bool flagLabel)
				{
					if (flagLabel) {
						textColorOff = Color::White;
						textColorOn = Color::Black;
						thumbOff = ColorDrawable::create(Color(255, 255, 255));
						trackOff = ColorDrawable::create(Color(130, 130, 130));
						pressedThumbOff = ColorDrawable::create(Color(255, 255, 255));
						pressedTrackOff = ColorDrawable::create(Color(100, 100, 100));
						hoverThumbOff = ColorDrawable::create(Color(255, 255, 255));
						hoverTrackOff = ColorDrawable::create(Color(120, 120, 120));
						thumbOn = thumbOff;
						trackOn = trackOff;
						pressedThumbOn = pressedThumbOff;
						pressedTrackOn = pressedTrackOff;
						hoverThumbOn = hoverThumbOff;
						hoverTrackOn = hoverTrackOff;
					} else {
						textColorOff = Color::Black;
						textColorOn = Color::Black;
						thumbOff = ColorDrawable::create(Color(255, 255, 255));
						trackOff = ColorDrawable::create(Color(120, 120, 120));
						pressedThumbOff = ColorDrawable::create(Color(255, 255, 255));
						pressedTrackOff = ColorDrawable::create(Color(0, 70, 210));
						hoverThumbOff = ColorDrawable::create(Color(255, 255, 255));
						hoverTrackOff = ColorDrawable::create(Color(90, 90, 90));
						thumbOn = ColorDrawable::create(Color(255, 255, 255));
						trackOn = ColorDrawable::create(Color(0, 80, 230));
						pressedThumbOn = ColorDrawable::create(Color(255, 255, 255));
						pressedTrackOn = ColorDrawable::create(Color(0, 70, 210));
						hoverThumbOn = ColorDrawable::create(Color(255, 255, 255));
						hoverTrackOn = ColorDrawable::create(Color(30, 90, 210));
					}
				}
			};
			
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticContext, sl_false)
			SLIB_SAFE_STATIC_GETTER(StaticContext, GetStaticLabelContext, sl_true)

		}
	}

	using namespace priv::switch_view;
	
	SLIB_DEFINE_OBJECT(SwitchView, View)

	SwitchView::SwitchView():
		m_flagDefaultTextOff(sl_true),
		m_flagDefaultTextOn(sl_true),
		m_flagDefaultTextColorOff(sl_true),
		m_flagDefaultTextColorOn(sl_true),
		m_flagDefaultThumbOff(sl_true),
		m_flagDefaultThumbOn(sl_true),
		m_flagDefaultTrackOff(sl_true),
		m_flagDefaultTrackOn(sl_true),
		m_flagDefaultPressedThumbOff(sl_true),
		m_flagDefaultPressedThumbOn(sl_true),
		m_flagDefaultPressedTrackOff(sl_true),
		m_flagDefaultPressedTrackOn(sl_true),
		m_flagDefaultHoverThumbOff(sl_true),
		m_flagDefaultHoverThumbOn(sl_true),
		m_flagDefaultHoverTrackOff(sl_true),
		m_flagDefaultHoverTrackOn(sl_true),

		m_value(sl_false),
		m_flagTextInButton(sl_false),

		m_posThumb(0)
	{
	}
	
	SwitchView::~SwitchView()
	{
	}
	
	sl_bool SwitchView::getValue()
	{
		return m_value;
	}

	sl_bool SwitchView::isLeft()
	{
		return !m_value;
	}

	void SwitchView::switchToLeft(UIUpdateMode mode)
	{
		setValue(sl_false, mode);
	}

	sl_bool SwitchView::isRight()
	{
		return m_value;
	}
	
	void SwitchView::switchToRight(UIUpdateMode mode)
	{
		setValue(sl_true, mode);
	}

	void SwitchView::setValue(sl_bool value, UIUpdateMode mode)
	{
		{
			ObjectLocker lock(this);
			m_value = value;
			sl_real f = value ? 1.0f : 0.0f;
			if (SLIB_UI_UPDATE_MODE_IS_ANIMATE(mode) && !(Math::isAlmostZero(f - m_posThumb))) {
				if (m_timer.isNull()) {
					m_timer = startTimer(SLIB_FUNCTION_WEAKREF(SwitchView, _onTimerAnimation, this), 10);
				}
				return;
			}
			m_timer.setNull();
			m_posThumb = f;
		}
		invalidate(mode);
	}

	sl_bool SwitchView::isTextInButton()
	{
		return m_flagTextInButton;
	}

	void SwitchView::setTextInButton(sl_bool flag, UIUpdateMode mode)
	{
		m_flagTextInButton = flag;
		invalidate(mode);
	}

#define SWITCH_VIEW_GET_SET_1(BIG_NAME, SMALL_NAME, TYPE) \
	TYPE SwitchView::get##BIG_NAME() \
	{ \
		return m_##SMALL_NAME; \
	} \
	void SwitchView::set##BIG_NAME(const TYPE& v, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME = v; \
		m_flagDefault##BIG_NAME = sl_false; \
		invalidate(mode); \
	}

#define SWITCH_VIEW_GET_SET(BIG_NAME, SMALL_NAME, TYPE) \
	SWITCH_VIEW_GET_SET_1(BIG_NAME##Off, SMALL_NAME##Off, TYPE) \
	SWITCH_VIEW_GET_SET_1(BIG_NAME##On, SMALL_NAME##On, TYPE) \
	TYPE SwitchView::get##BIG_NAME() \
	{ \
		if (m_value) { \
			return m_##SMALL_NAME##On; \
		} else { \
			return m_##SMALL_NAME##Off; \
		} \
	} \
	void SwitchView::set##BIG_NAME(const TYPE& v, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME##Off = v; \
		m_##SMALL_NAME##On = v; \
		m_flagDefault##BIG_NAME##Off = sl_false; \
		m_flagDefault##BIG_NAME##On = sl_false; \
		invalidate(mode); \
	} \

	SWITCH_VIEW_GET_SET(Text, text, String)
	SWITCH_VIEW_GET_SET(TextColor, textColor, Color)

#define SWITCH_VIEW_GET_SET_DRAWABLE(BIG_NAME, SMALL_NAME) \
	SWITCH_VIEW_GET_SET(BIG_NAME, SMALL_NAME, Ref<Drawable>) \
	void SwitchView::set##BIG_NAME##ColorOff(const Color& color, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME##Off = ColorDrawable::create(color); \
		m_flagDefault##BIG_NAME##Off = sl_false; \
		invalidate(mode); \
	} \
	void SwitchView::set##BIG_NAME##ColorOn(const Color& color, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME##On = ColorDrawable::create(color); \
		m_flagDefault##BIG_NAME##On = sl_false; \
		invalidate(mode); \
	} \
	void SwitchView::set##BIG_NAME##Color(const Color& color, UIUpdateMode mode) \
	{ \
		m_##SMALL_NAME##Off = ColorDrawable::create(color); \
		m_##SMALL_NAME##On = ColorDrawable::create(color); \
		m_flagDefault##BIG_NAME##Off = sl_false; \
		m_flagDefault##BIG_NAME##On = sl_false; \
		invalidate(mode); \
	}

	SWITCH_VIEW_GET_SET_DRAWABLE(Thumb, thumb)
	SWITCH_VIEW_GET_SET_DRAWABLE(Track, track)
	SWITCH_VIEW_GET_SET_DRAWABLE(PressedThumb, pressedThumb)
	SWITCH_VIEW_GET_SET_DRAWABLE(PressedTrack, pressedTrack)
	SWITCH_VIEW_GET_SET_DRAWABLE(HoverThumb, hoverThumb)
	SWITCH_VIEW_GET_SET_DRAWABLE(HoverTrack, hoverTrack)
	
	String SwitchView::getLeftText()
	{
		return m_textOff;
	}

	void SwitchView::setLeftText(const String& text, UIUpdateMode mode)
	{
		setTextOff(text, mode);
	}

	String SwitchView::getRightText()
	{
		return m_textOn;
	}

	void SwitchView::setRightText(const String& text, UIUpdateMode mode)
	{
		setTextOn(text, mode);
	}
	
	void SwitchView::setPressedState(sl_bool flagState, UIUpdateMode mode)
	{
		View::setPressedState(flagState, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None);
		if (flagState) {
			invalidate(mode);
		} else {
			setValue(m_value);
		}
	}

	void SwitchView::setHoverState(sl_bool flagState, UIUpdateMode mode)
	{
		View::setHoverState(flagState, SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None);
		invalidate(mode);
	}

	SLIB_DEFINE_EVENT_HANDLER(SwitchView, Change, sl_bool newValue)
	
	void SwitchView::dispatchChange(sl_bool newValue)
	{
		SLIB_INVOKE_EVENT_HANDLER(Change, newValue)
	}
	
	void SwitchView::onDraw(Canvas* canvas)
	{
		StaticContext* s = GetStaticContext();
		if (m_flagTextInButton) {
			s = GetStaticLabelContext();
		} else {
			s = GetStaticContext();
		}
		if (!s) {
			return;
		}
		UIRect rect;
		if (!(calculateSwitchRegion(rect))) {
			return;
		}
		Ref<Drawable> track;
		Ref<Drawable> thumb;
		if (isPressedState()) {
			if (m_value) {
				if (m_flagDefaultPressedTrackOn) {
					if (m_flagDefaultTrackOn) {
						track = s->pressedTrackOn;
					} else {
						track = m_trackOn;
					}
				} else {
					track = m_pressedTrackOn;
				}
				if (m_flagDefaultPressedThumbOn) {
					if (m_flagDefaultThumbOn) {
						thumb = s->pressedThumbOn;
					} else {
						thumb = m_thumbOn;
					}
				} else {
					thumb = m_pressedThumbOn;
				}
			} else {
				if (m_flagDefaultPressedTrackOff) {
					if (m_flagDefaultTrackOff) {
						track = s->pressedTrackOff;
					} else {
						track = m_trackOff;
					}
				} else {
					track = m_pressedTrackOff;
				}
				if (m_flagDefaultPressedThumbOff) {
					if (m_flagDefaultThumbOff) {
						thumb = s->pressedThumbOff;
					} else {
						thumb = m_thumbOff;
					}
				} else {
					thumb = m_pressedThumbOff;
				}
			}
		} else if (isHoverState()) {
			if (m_value) {
				if (m_flagDefaultHoverTrackOn) {
					if (m_flagDefaultTrackOn) {
						track = s->hoverTrackOn;
					} else {
						track = m_trackOn;
					}
				} else {
					track = m_hoverTrackOn;
				}
				if (m_flagDefaultHoverThumbOn) {
					if (m_flagDefaultThumbOn) {
						thumb = s->hoverThumbOn;
					} else {
						thumb = m_thumbOn;
					}
				} else {
					thumb = m_hoverThumbOn;
				}
			} else {
				if (m_flagDefaultHoverTrackOff) {
					if (m_flagDefaultTrackOff) {
						track = s->hoverTrackOff;
					} else {
						track = m_trackOff;
					}
				} else {
					track = m_hoverTrackOff;
				}
				if (m_flagDefaultHoverThumbOff) {
					if (m_flagDefaultThumbOff) {
						thumb = s->hoverThumbOff;
					} else {
						thumb = m_thumbOff;
					}
				} else {
					thumb = m_hoverThumbOff;
				}
			}
		} else {
			if (m_value) {
				if (m_flagDefaultTrackOn) {
					track = s->trackOn;
				} else {
					track = m_trackOn;
				}
				if (m_flagDefaultThumbOn) {
					thumb = s->thumbOn;
				} else {
					thumb = m_thumbOn;
				}
			} else {
				if (m_flagDefaultTrackOff) {
					track = s->trackOff;
				} else {
					track = m_trackOff;
				}
				if (m_flagDefaultThumbOff) {
					thumb = s->thumbOff;
				} else {
					thumb = m_thumbOff;
				}
			}
		}
		
		sl_ui_len widthTrack = rect.getWidth();
		sl_ui_len widthThumb = widthTrack / 2;
		
		drawTrack(canvas, track, rect);
		
		if (thumb.isNotNull()) {
			sl_real f = m_posThumb;
			if (f < 0) {
				f = 0;
			} else if (f > 1) {
				f = 1;
			}
			UIRect rectThumb = rect;
			rectThumb.left += (sl_ui_pos)(f * (widthTrack - widthThumb));
			rectThumb.setWidth(widthThumb);
			drawThumb(canvas, thumb, rectThumb);
		}
		if (m_flagTextInButton) {
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				Color colorOn, colorOff;
				if (m_flagDefaultTextColorOn) {
					colorOn = s->textColorOn;
				} else {
					colorOn = m_textColorOn;
				}
				if (m_flagDefaultTextColorOff) {
					colorOff = s->textColorOff;
				} else {
					colorOff = m_textColorOff;
				}
				UIRect rectThumb = rect;
				rectThumb.setWidth(widthThumb);
				canvas->drawText(m_textOff, rectThumb, font, m_value && !(isPressedState()) ? colorOff : colorOn, Alignment::MiddleCenter);
				rectThumb.left = rect.left + widthThumb;
				rectThumb.setWidth(widthThumb);
				canvas->drawText(m_textOn, rectThumb, font, m_value || isPressedState() ? colorOn : colorOff, Alignment::MiddleCenter);
			}
		} else {
			String text;
			Color textColor;
			if (m_value) {
				text = m_textOn;
				if (m_flagDefaultTextColorOn) {
					textColor = s->textColorOn;
				} else {
					textColor = m_textColorOn;
				}
			} else {
				text = m_textOff;
				if (m_flagDefaultTextColorOff) {
					textColor = s->textColorOff;
				} else {
					textColor = m_textColorOff;
				}
			}
			if (text.isNotEmpty()) {
				Ref<Font> font = getFont();
				if (font.isNotNull()) {
					canvas->drawText(text, (sl_real)(getPaddingLeft()), (sl_real)(rect.top + (rect.getHeight() - font->getFontHeight()) / 2), font, textColor);
				}
			}
		}
		
	}
	
	void SwitchView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		sl_ui_len paddingWidth = getPaddingLeft() + getPaddingRight();
		sl_ui_len paddingHeight = getPaddingTop() + getPaddingBottom();
		
		if (flagVertical) {
			sl_ui_len h = (sl_ui_len)(getFontSize() * 1.5f);
			setLayoutHeight(h + paddingHeight);
		}
		
		if (flagHorizontal) {
			sl_ui_len heightSwitch = getLayoutHeight() - paddingHeight;
			if (heightSwitch < 0) {
				heightSwitch = 0;
			}
			sl_ui_len widthText = 0;
			if (m_textOn.isNotEmpty() || m_textOff.isNotEmpty()) {
				Ref<Font> font = getFont();
				if (font.isNotNull()) {
					widthText = (sl_ui_len)(Math::max(font->measureText(m_textOn).x, font->measureText(m_textOff).x));
				}
			}
			if (m_flagTextInButton) {
				if (widthText > heightSwitch) {
					setLayoutWidth(widthText * 2 + heightSwitch + paddingWidth);
				} else {
					setLayoutWidth(heightSwitch * 3 + paddingWidth);
				}
			} else {
				if (widthText > 0) {
					setLayoutWidth(widthText + (sl_ui_len)(getFontSize() * 0.5f) + heightSwitch * 2 + paddingWidth);
				} else {
					setLayoutWidth(heightSwitch * 2 + paddingWidth);
				}
			}
		}
	}
	
	void SwitchView::onMouseEvent(UIEvent* ev)
	{
		sl_real dimUnit = Math::ceil(UI::dpToPixel(1));
		if (dimUnit < 1) {
			dimUnit = 1;
		}
		UIAction action = ev->getAction();
		if (action != UIAction::LeftButtonDrag && action != UIAction::TouchMove) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				parent->setLockScroll(sl_false);
			}
		}
		switch (action) {
			case UIAction::LeftButtonDown:
			case UIAction::TouchBegin:
				{
					m_ptMouseDown = ev->getPoint();
					m_posMouseDown = m_posThumb;
					m_flagTapping = sl_true;
					m_tracker.clearMovements();
					View::setPressedState(sl_true);
					
					ObjectLocker lock(this);
					m_timer.setNull();
				}
				break;
			case UIAction::LeftButtonDrag:
			case UIAction::TouchMove:
				if (isPressedState()) {
					Point pt = ev->getPoint();
					m_tracker.addMovement(pt);
					sl_real dx = Math::abs(pt.x - m_ptMouseDown.x);
					if (dx > 5 * dimUnit) {
						cancelPressedStateOfChildren();
						sl_real dy = Math::abs(pt.y - m_ptMouseDown.y);
						if (dy < dx) {
							Ref<View> parent = getParent();
							if (parent.isNotNull()) {
								parent->setLockScroll(sl_true);
							}
						}
					}
					if (m_flagTapping) {
						if ((pt - m_ptMouseDown).getLength2p() > dimUnit * dimUnit * 30) {
							m_flagTapping = sl_false;
						}
					}
					UIRect rect;
					if (calculateSwitchRegion(rect)) {
						sl_real w = (sl_real)(rect.getWidth());
						sl_real f = m_posMouseDown + (pt.x - m_ptMouseDown.x) / w * 2;
						m_posThumb = Math::clamp(f, 0.0f, 1.0f);
						invalidate();
					}
				}
				break;
			case UIAction::LeftButtonUp:
			case UIAction::TouchEnd:
				if (isPressedState()) {
					sl_real v = 0;
					m_tracker.getVelocity(&v, sl_null);
					sl_real t = dimUnit * 10;
					if (v > t) {
						_changeValue(sl_true);
					} else if (v < -t) {
						_changeValue(sl_false);
					} else {
						if (m_flagTapping) {
							_changeValue(!m_value);
						} else {
							if (m_posThumb > 0.5f) {
								_changeValue(sl_true);
							} else {
								_changeValue(sl_false);
							}
						}
					}
				}
				m_tracker.clearMovements();
				break;
			case UIAction::TouchCancel:
				setValue(m_value);
				m_tracker.clearMovements();
				break;
			default:
				break;
		}
	}

	sl_bool SwitchView::calculateSwitchRegion(UIRect& rect)
	{
		rect = getBoundsInnerPadding();
		if (!(rect.isValidSize())) {
			return sl_false;
		}
		if (!m_flagTextInButton) {
			rect.left = rect.right - rect.getHeight() * 2;
		}
		return sl_true;
	}

	void SwitchView::drawTrack(Canvas* canvas, const Ref<Drawable>& track, const Rectangle& rectDst)
	{
		if (track.isNull()) {
			return;
		}
		Color color;
		if (ColorDrawable::check(track.get(), &color)) {
			sl_real height = rectDst.getHeight();
			sl_real width = rectDst.getWidth();
			if (height < width) {
				height /= 2;
				canvas->fillRoundRect(rectDst, Size(height, height), color);
			} else {
				canvas->fillRectangle(rectDst, color);
			}
		} else {
			canvas->draw(rectDst, track);
		}
	}
	
	void SwitchView::drawThumb(Canvas* canvas, const Ref<Drawable>& thumb, const Rectangle& rectDst)
	{
		if (thumb.isNull()) {
			return;
		}
		Color color;
		if (ColorDrawable::check(thumb.get(), &color)) {
			sl_real width = rectDst.getWidth();
			sl_real height = rectDst.getHeight();
			sl_real padding = height / 20;
			if (padding < 1) {
				padding = 1;
			}
			Rectangle rect = rectDst;
			if (width > padding * 2 && height > padding * 2) {
				rect.left += padding;
				rect.right -= padding;
				rect.top += padding;
				rect.bottom -= padding;
				width = rect.getWidth();
				height = rect.getHeight();
			}
			if (height < width) {
				height /= 2;
				canvas->fillRoundRect(rect, Size(height, height), color);
			} else {
				rect.top = (rect.top + rect.bottom - width) / 2;
				rect.setHeight(width);
				canvas->fillEllipse(rect, color);
			}
		} else {
			canvas->draw(rectDst, thumb);
		}
	}
		
	void SwitchView::_changeValue(sl_bool value)
	{
		if (value != m_value) {
			setValue(value);
			dispatchChange(value);
		} else {
			setValue(value);
		}
	}
	
	void SwitchView::_onTimerAnimation(Timer* timer)
	{
		do {
			ObjectLocker lock(this);
			sl_real target = m_value ? 1.0f : 0.0f;
			sl_real pos = m_posThumb;
			sl_real d = 0.1f;
			sl_real a = Math::abs(target - pos);
			if (a < d || a > 2) {
				m_posThumb = target;
				m_timer.setNull();
				break;
			}
			if (target > m_posThumb) {
				m_posThumb += d;
			} else {
				m_posThumb -= d;
			}
		} while (0);
		invalidate();
	}

}
