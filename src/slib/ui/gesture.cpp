/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/gesture.h"

#include "slib/ui/view.h"
#include "slib/ui/resource.h"
#include "slib/core/math.h"

namespace slib
{

	GestureEvent::GestureEvent(): type(GestureType::SwipeLeft)
	{
	}
	
	GestureEvent::~GestureEvent()
	{
	}
	
	Ref<GestureEvent> GestureEvent::duplicate()
	{
		Ref<GestureEvent> ev = new GestureEvent;
		if (ev.isNotNull()) {
			ev->type = type;
			return ev;
		}
		return sl_null;
	}
	
	SLIB_DEFINE_OBJECT(GestureDetector, Object)
	
	class GestureRecognizer : public Referable
	{
	public:
		GestureDetector* m_detector;
		
	public:
		GestureRecognizer(GestureDetector* detector) : m_detector(detector)
		{
		}
		
		virtual void processEvent(UIEvent* ev, const Ref<View>& view, MotionTracker* tracker)
		{
		}
		
	};
	
	class SwipeGestureRecognizer : public GestureRecognizer
	{
	public:
		GestureType type;
		sl_bool flagHorizontal;
		sl_bool flagPositive;
		
	public:
		SwipeGestureRecognizer(GestureDetector* detector, GestureType _type)
		: GestureRecognizer(detector)
		{
			type = _type;
			switch (type) {
				case GestureType::SwipeLeft:
					flagHorizontal = sl_true;
					flagPositive = sl_false;
					break;
				case GestureType::SwipeRight:
					flagHorizontal = sl_true;
					flagPositive = sl_true;
					break;
				case GestureType::SwipeUp:
					flagHorizontal = sl_false;
					flagPositive = sl_false;
					break;
				default:
					flagHorizontal = sl_false;
					flagPositive = sl_true;
					break;
			}
		}
		
		// override
		void processEvent(UIEvent* ev, const Ref<View>& view, MotionTracker* tracker)
		{
			UIAction action = ev->getAction();
			if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd || action == UIAction::TouchCancel) {
				sl_real t = (sl_real)(UIResource::getScreenMinimum()) * 0.2f;
				if (t < 5) {
					return;
				}
				sl_real vx, vy;
				if (tracker->getVelocity(&vx, &vy)) {
					if (flagHorizontal) {
						if (Math::abs(vx) > Math::abs(vy)) {
							vx /= t;
							if (!flagPositive) {
								vx = -vx;
							}
							if (vx > 1) {
								Ref<GestureEvent> ev = new GestureEvent;
								if (ev.isNotNull()) {
									ev->type = type;
									view->dispatchSwipe(ev.get());
								}
							}
						}
					} else {
						if (Math::abs(vy) > Math::abs(vx)) {
							vy /= t;
							if (!flagPositive) {
								vy = -vy;
							}
							if (vy > 1) {
								Ref<GestureEvent> ev = new GestureEvent;
								if (ev.isNotNull()) {
									ev->type = type;
									view->dispatchSwipe(ev.get());
								}
							}
						}
					}
				}
			}
		}
	};
	
	GestureDetector::GestureDetector(const Ref<View>& view) : m_view(view)
	{
	}
	
	GestureDetector::~GestureDetector()
	{
	}

	void GestureDetector::enable(GestureType type)
	{
		ObjectLocker lock(this);
		Ref<View> view = m_view;
		if (view.isNull()) {
			return;
		}
		_enableNative(view, type);
		if (m_recognizers[(int)type].isNotNull()) {
			return;
		}
		Ref<GestureRecognizer> recognizer;
		switch (type) {
			case GestureType::SwipeLeft:
			case GestureType::SwipeRight:
			case GestureType::SwipeUp:
			case GestureType::SwipeDown:
				recognizer = new SwipeGestureRecognizer(this, type);
				break;
			default:
				break;
		}
		m_recognizers[(int)type] = recognizer;
	}
	
	void GestureDetector::enableNative()
	{
		Ref<View> view = m_view;
		if (view.isNull()) {
			return;
		}
		int nRecognizers = (int)(GestureType::Count);
		for (int i = 0; i < nRecognizers; i++) {
			if (m_recognizers[i].isNotNull()) {
				_enableNative(view, (GestureType)i);
			}
		}
	}
	
	void GestureDetector::processEvent(UIEvent* ev)
	{
		ObjectLocker lock(this);
		Ref<View> view = m_view;
		if (view.isNull()) {
			return;
		}
		int i;
		int nRecognizers = (int)(GestureType::Count);
		sl_bool flagProcess = sl_false;
		sl_bool flagNative[(int)(GestureType::Count)];
		for (i = 0; i < nRecognizers; i++) {
			if (m_recognizers[i].isNotNull()) {
				flagNative[i] = _enableNative(view, (GestureType)i);
				if (!(flagNative[i])) {
					flagProcess = sl_true;
				}
			}
		}
		if (!flagProcess) {
			return;
		}
		UIAction action = ev->getAction();
		switch (action) {
			case UIAction::LeftButtonDown:
			case UIAction::TouchBegin:
				m_tracker.clearMovements();
				m_tracker.addMovement(ev);
				break;
			case UIAction::LeftButtonDrag:
			case UIAction::TouchMove:
			case UIAction::LeftButtonUp:
			case UIAction::TouchEnd:
			case UIAction::TouchCancel:
				m_tracker.addMovement(ev);
				break;
			default:
				break;
		}
		for (i = 0; i < nRecognizers; i++) {
			if (m_recognizers[i].isNotNull() && !(flagNative[i])) {
				m_recognizers[i]->processEvent(ev, view, &m_tracker);
			}
		}
	}
	
#if !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_ANDROID))
	
	sl_bool GestureDetector::_enableNative(const Ref<View>& view, GestureType type)
	{
		return sl_false;
	}
	
#endif

}
