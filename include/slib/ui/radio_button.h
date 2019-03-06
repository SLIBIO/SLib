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
		Ref<RadioGroup> getGroup();
		
		String getValue();
		
		void setValue(const String& value);
		
		void setChecked(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw) override;

	public:
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent) override;
		
		void dispatchClickEvent(UIEvent* ev) override;

	public:
		AtomicWeakRef<RadioGroup> m_group;
		AtomicString m_value;
		
		friend class RadioGroup;
		
	};

	class SLIB_EXPORT RadioGroup : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		RadioGroup();
		
		~RadioGroup();

	public:
		List< Ref<RadioButton> > getButtons();
		
		void add(const Ref<RadioButton>& button);
		
		void remove(const Ref<RadioButton>& button);
		
		void select(const Ref<RadioButton>& button);
		
		Ref<RadioButton> getSelected();
		
		void selectValue(const String& value);
		
		String getSelectedValue();
		
	private:
		void _setChecked(RadioButton* button, sl_bool flag, UIUpdateMode mode);
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(RadioGroup, Select, RadioButton*)
		
	protected:
		CList< Ref<RadioButton> > m_buttons;
		Ref<RadioButton> m_buttonSelected;
		
		friend class RadioButton;
		
	};

}

#endif
