/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_GESTURE
#define CHECKHEADER_SLIB_UI_GESTURE

#include "definition.h"

#include "constants.h"
#include "motion_tracker.h"

namespace slib
{

	class GestureEvent : public Referable
	{
	public:
		GestureType type;
		
	public:
		GestureEvent();
		
		~GestureEvent();
		
	public:
		Ref<GestureEvent> duplicate();
		
	};
	
	class View;
	class UIEvent;
	
	class GestureRecognizer;
	
	class GestureDetector : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		GestureDetector(const Ref<View>& view);
		
		~GestureDetector();

	public:
		void enable(GestureType type);
		
		void enableNative();
		
		void processEvent(UIEvent* ev);
		
	private:
		static sl_bool _enableNative(const Ref<View>& view, GestureType type);
		
	protected:
		WeakRef<View> m_view;
		MotionTracker m_tracker;
		Ref<GestureRecognizer> m_recognizers[(int)(GestureType::Count)];
		
		friend class GestureRecognizer;
	};

}

#endif
