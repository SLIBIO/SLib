/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_LINEAR_VIEW
#define CHECKHEADER_SLIB_UI_LINEAR_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT LinearView : public ViewGroup
	{
		SLIB_DECLARE_OBJECT
		
	public:
		LinearView();
		
		~LinearView();

	public:
		LayoutOrientation getOrientation();
		
		void setOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isHorizontal();
		
		void setHorizontal(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isVertical();
		
		void setVertical(UIUpdateMode mode = UIUpdateMode::Redraw);
		
	protected:
		void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical) override;
		
		void onMakeLayout() override;
		
	private:
		LayoutOrientation m_orientation;
		
	};
	
	class SLIB_EXPORT VerticalLinearView : public LinearView
	{
	public:
		VerticalLinearView();

		~VerticalLinearView();

	};
	
	class SLIB_EXPORT HorizontalLinearView : public LinearView
	{
	public:
		HorizontalLinearView();

		~HorizontalLinearView();

	};

}

#endif
