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

#include "slib/ui/drawer.h"

#include "slib/ui/core.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(Drawer, ViewGroup)
	
	Drawer::Drawer()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setSavingCanvasState(sl_false);
		
		setWidthWeight(1, UIUpdateMode::Init);
		setHeightFilling(1, UIUpdateMode::Init);
		setBackgroundColor(Color(0, 0, 0, 100), UIUpdateMode::Init);
		setVisible(sl_false, UIUpdateMode::Init);
		
		m_flagOpened = sl_false;
		m_drawerSize = 0;
		m_dragEdgeSize = 0;
		m_gravity = Alignment::Left;
		
		m_callbackParentMouseEvent = SLIB_FUNCTION_WEAKREF(Drawer, _onParentMouseEvent, this);
		
		m_flagMouseDown = sl_false;
		m_posMouseDown = 0;
	}
	
	Drawer::~Drawer()
	{
	}
	
	sl_bool Drawer::isOpened()
	{
		return m_flagOpened;
	}
	
	void Drawer::open()
	{
		if (!(isDrawingThread())) {
			dispatchToDrawingThread(SLIB_FUNCTION_WEAKREF(Drawer, open, this));
			return;
		}
		Ref<View> content = getContent();
		if (content.isNull()) {
			return;
		}
		if (m_flagOpened) {
			return;
		}
		m_flagOpened = sl_true;
		Ref<Animation> animation(m_animation);
		if (animation.isNotNull()) {
			animation->stop();
		}
		setVisible(sl_true);
		sl_real w = (sl_real)(getDrawerSize());
		m_animation = content->startTranslateAnimationTo(_makeContentTranslation(w), (w - _getContentTranslation()) / w * 0.5f, SLIB_FUNCTION_WEAKREF(Drawer, _onEndOpenAnimation, this), AnimationCurve::EaseOut, AnimationFlags::NotSelfAlive);
	}
	
	void Drawer::_onEndOpenAnimation()
	{
		m_animation.setNull();
	}
	
	void Drawer::close()
	{
		if (!(isDrawingThread())) {
			dispatchToDrawingThread(SLIB_FUNCTION_WEAKREF(Drawer, close, this));
			return;
		}
		Ref<View> content = getContent();
		if (content.isNull()) {
			return;
		}
		if (!m_flagOpened) {
			return;
		}
		m_flagOpened = sl_false;
		Ref<Animation> animation(m_animation);
		if (animation.isNotNull()) {
			animation->stop();
		}
		sl_real w = (sl_real)(getDrawerSize());
		m_animation = content->startTranslateAnimationTo(_makeContentTranslation(0), _getContentTranslation() / w * 0.5f, SLIB_FUNCTION_WEAKREF(Drawer, _onEndCloseAnimation, this), AnimationCurve::EaseOut, AnimationFlags::NotSelfAlive);
	}
	
	void Drawer::_onEndCloseAnimation()
	{
		m_animation.setNull();
		setVisible(sl_false);
	}
	
	Ref<View> Drawer::getContent()
	{
		return getChild(0);
	}
	
	void Drawer::setContent(const Ref<View>& content)
	{
		removeAllChildren();
		if (content.isNotNull()) {
			addChild(content);
		}
	}
	
	sl_ui_len Drawer::getDrawerSize()
	{
		sl_ui_len ret = m_drawerSize;
		if (ret <= 0) {
			ret = 0;
			Ref<View> content = getContent();
			if (content.isNotNull()) {
				if (m_gravity == Alignment::Top || m_gravity == Alignment::Bottom) {
					ret = content->getHeight();
				} else {
					ret = content->getWidth();
				}
			}
		}
		return ret;
	}
	
	void Drawer::setDrawerSize(sl_ui_len size)
	{
		m_drawerSize = size;
	}
	
	sl_ui_len Drawer::getDragEdgeSize()
	{
		sl_ui_len ret = m_dragEdgeSize;
		if (ret <= 0) {
			ret = (sl_ui_len)(UI::millimeterToPixel(5));
		}
		return ret;
	}
	
	void Drawer::setDragEdgeSize(sl_ui_len size)
	{
		m_dragEdgeSize = size;
	}
	
	Alignment Drawer::getGravity()
	{
		return m_gravity;
	}
	
	void Drawer::setGravity(const Alignment& align)
	{
		if ((align & Alignment::HorizontalMask) == Alignment::Left) {
			m_gravity = Alignment::Left;
		} else if ((align & Alignment::HorizontalMask) == Alignment::Right) {
			m_gravity = Alignment::Right;
		} else if ((align & Alignment::VerticalMask) == Alignment::Top) {
			m_gravity = Alignment::Top;
		} else if ((align & Alignment::VerticalMask) == Alignment::Bottom) {
			m_gravity = Alignment::Bottom;
		} else {
			m_gravity = Alignment::Left;
		}
	}

	void Drawer::onChangeParent(View* oldParent, View* newParent)
	{
		if (oldParent) {
			oldParent->removeOnMouseEvent(m_callbackParentMouseEvent);
		}
		if (newParent) {
			newParent->addOnMouseEvent(m_callbackParentMouseEvent);
		}
	}
	
	void Drawer::dispatchMouseEvent(UIEvent *ev)
	{
		_onMouseEvent(ev);
		ViewGroup::dispatchMouseEvent(ev);
	}
	
	void Drawer::dispatchTouchEvent(UIEvent *ev)
	{
		_onMouseEvent(ev);
		ViewGroup::dispatchTouchEvent(ev);
	}

	void Drawer::_onMouseEvent(UIEvent* ev)
	{
		if (!m_flagOpened) {
			return;
		}
		UIAction action = ev->getAction();
		if (action == UIAction::LeftButtonDown || action == UIAction::RightButtonDown || action == UIAction::TouchBegin) {
			Ref<View> content = getContent();
			if (content.isNotNull()) {
				if (content->hitTest(content->convertCoordinateFromParent(ev->getPoint()))) {
					return;
				}
			}
			close();
		}
	}
	
	void Drawer::onCancel(UIEvent* ev)
	{
		close();
		ev->preventDefault();
		ev->stopPropagation();
	}
	
	void Drawer::_onParentMouseEvent(View* parent, UIEvent* ev)
	{
		Ref<View> content = getContent();
		if (content.isNull()) {
			return;
		}
		if (m_flagOpened) {
			return;
		}
		UIAction action = ev->getAction();
		sl_real pos;
		if (m_gravity == Alignment::Left) {
			pos = ev->getX();
		} else if (m_gravity == Alignment::Right) {
			pos = parent->getWidth() - ev->getX();
		} else if (m_gravity == Alignment::Top) {
			pos = ev->getY();
		} else if (m_gravity == Alignment::Bottom) {
			pos = parent->getHeight() - ev->getY();
		} else {
			return;
		}
		if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
			if (!m_flagMouseDown) {
				if (pos < getDragEdgeSize()) {
					_setContentTranslation(0);
					setVisible(sl_true);
					m_motionTracker.clearMovements();
					m_flagMouseDown = sl_true;
					m_posMouseDown = pos;
				}
			}
		} else if (action == UIAction::LeftButtonDrag || action == UIAction::TouchMove) {
			if (m_flagMouseDown) {
				m_motionTracker.addMovement(pos, 0);
				_setContentTranslation(pos - m_posMouseDown);
			}
		} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd || action == UIAction::TouchCancel) {
			if (m_flagMouseDown) {
				m_flagMouseDown = sl_false;
				sl_real v = 0;
				m_motionTracker.getVelocity(&v, sl_null);
				if (v > 0) {
					m_flagOpened = sl_false;
					open();
				} else if (v < 0) {
					m_flagOpened = sl_true;
					close();
				} else {
					if (pos > getDrawerSize() / 2) {
						m_flagOpened = sl_false;
						open();
					} else {
						m_flagOpened = sl_true;
						close();
					}
				}
				m_motionTracker.clearMovements();
			}
		}
		
	}
	
	Vector2 Drawer::_makeContentTranslation(sl_real t)
	{
		if (t < 0) {
			t = 0;
		}
		sl_real size = (sl_real)(getDrawerSize());
		t -= size;
		if (t > 0) {
			t = 0;
		}
		if (m_gravity == Alignment::Left) {
			return Vector2f(t, 0);
		} else if (m_gravity == Alignment::Right) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				return Vector2f(parent->getWidth() - t - size, 0);
			}
		} else if (m_gravity == Alignment::Top) {
			return Vector2f(0, t);
		} else if (m_gravity == Alignment::Bottom) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				return Vector2f(0, parent->getHeight() - t - size);
			}
		}
		return Vector2::zero();
	}
	
	sl_real Drawer::_getContentTranslation()
	{
		Ref<View> content = getContent();
		if (content.isNull()) {
			return 0;
		}
		sl_real size = (sl_real)(getDrawerSize());
		sl_real t;
		if (m_gravity == Alignment::Left) {
			t = content->getTranslationX();
		} else if (m_gravity == Alignment::Right) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				t = parent->getWidth() - content->getTranslationX() - size;
			} else {
				return 0;
			}
		} else if (m_gravity == Alignment::Top) {
			t = content->getTranslationY();
		} else if (m_gravity == Alignment::Bottom) {
			Ref<View> parent = getParent();
			if (parent.isNotNull()) {
				t = parent->getHeight() - content->getTranslationY() - size;
			} else {
				return 0;
			}
		} else {
			return 0;
		}
		t += size;
		if (t < 0) {
			t = 0;
		}
		if (t > size) {
			t = size;
		}
		return t;
	}
	
	void Drawer::_setContentTranslation(sl_real t)
	{
		Ref<View> content = getContent();
		if (content.isNull()) {
			return;
		}
		content->setTranslation(_makeContentTranslation(t));
	}
	
}
