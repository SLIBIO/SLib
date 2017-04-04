/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/text_view.h"

namespace slib
{
	
	SLIB_DEFINE_OBJECT(TextView, View)
	
	TextView::TextView()
	{
		m_flagEditable = sl_false;
		setFocusable(sl_true);
	}
	
	TextView::~TextView()
	{
	}

	sl_bool TextView::isEditable()
	{
		return m_flagEditable;
	}
	
	void TextView::setEditable(sl_bool flagEditable)
	{
		m_flagEditable = flagEditable;
	}
	
	void TextView::onDraw(Canvas* canvas)
	{
		canvas->fillRectangle(getBounds(), Color::White);
	}
	
	void TextView::onMouseEvent(UIEvent* ev)
	{
		
		ev->stopPropagation();
		
	}
	
	void TextView::onKeyEvent(UIEvent* ev)
	{
		if (m_flagEditable) {
			ev->stopPropagation();
		}
	}

}
