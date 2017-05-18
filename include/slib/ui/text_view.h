/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_TEXT_VIEW
#define CHECKHEADER_SLIB_UI_TEXT_VIEW

#include "definition.h"

#include "view.h"

#include "../graphics/text.h"

namespace slib
{
	
	class SLIB_EXPORT TextView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TextView();
		
		~TextView();

	public:
		sl_bool isEditable();
		
		void setEditable(sl_bool flagEditable);
		
	protected:
		void onDraw(Canvas* canvas) override;
		
		void onMouseEvent(UIEvent* ev) override;
		
		void onKeyEvent(UIEvent* event) override;
		
	protected:
		sl_bool m_flagEditable;
		
	};

}

#endif
