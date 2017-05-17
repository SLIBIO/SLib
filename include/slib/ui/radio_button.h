/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_RADIO_BUTTON
#define CHECKHEADER_SLIB_UI_RADIO_BUTTON

#include "definition.h"

#include "check_box.h"

namespace slib
{

	class RadioGroup;

	class SLIB_EXPORT RadioButton : public CheckBox
	{
		SLIB_DECLARE_OBJECT
		
	public:
		RadioButton();
		
		RadioButton(sl_uint32 nCategories, ButtonCategory* categories);

		~RadioButton();

	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		void dispatchClick(UIEvent* ev) override;

		void dispatchClick();
		
	public:
		SLIB_PROPERTY(AtomicWeakRef<RadioGroup>, RadioGroup)
		
	};

	class SLIB_EXPORT RadioGroup : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		RadioGroup();
		
		~RadioGroup();

	public:
		void add(const Ref<RadioButton>& view);
		
		void remove(const Ref<RadioButton>& view);
		
		void select(const Ref<RadioButton>& view);
		
		Ref<RadioButton> getSelected();
		
	protected:
		CList< Ref<RadioButton> > m_views;
		AtomicRef<RadioButton> m_viewSelected;
		
	};

}

#endif
