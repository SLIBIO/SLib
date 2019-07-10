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

#include "slib/ui/radio_button.h"

#include "slib/ui/resource.h"
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace radio_button
		{

			class Icon : public Drawable
			{
			public:
				Ref<Pen> m_penBorder;
				Ref<Brush> m_brushBack;
				Ref<Brush> m_brushCheck;
				
			public:
				Icon(const Ref<Pen>& penBorder, const Color& backColor, const Color& checkColor)
				{
					m_penBorder = penBorder;
					if (backColor.a > 0) {
						m_brushBack = Brush::createSolidBrush(backColor);
					}
					if (checkColor.a > 0) {
						m_brushCheck = Brush::createSolidBrush(checkColor);
					}
				}
				
			public:
				void onDrawAll(Canvas* canvas, const Rectangle& rect, const DrawParam& param) override
				{
					canvas->drawEllipse(rect, m_penBorder, m_brushBack);
					if (m_brushCheck.isNotNull()) {
						Rectangle rcCheck;
						rcCheck.setLeftTop(rect.getCenter());
						sl_real w = rect.getWidth() / 2;
						sl_real h = rect.getHeight() / 2;
						rcCheck.left -= w / 2;
						rcCheck.top -= h / 2;
						rcCheck.right = rcCheck.left + w;
						rcCheck.bottom = rcCheck.top + h;
						canvas->fillEllipse(rcCheck, m_brushCheck);
					}
				}
				
			};

			class Categories
			{
			public:
				ButtonCategory categories[2];
				
				Categories()
				{
					sl_real w = (sl_real)(UIResource::toUiPos(UIResource::dpToPixel(1)));
					Color colorBackNormal = Color::White;
					Color colorBackHover = Color::White;
					Color colorBackDown(220, 230, 255);
					Color colorBackDisabled = Color(220, 220, 220);
					Ref<Pen> penNormal = Pen::createSolidPen(w, Color::Black);
					Ref<Pen> penHover = Pen::createSolidPen(w, Color(0, 80, 200));
					Ref<Pen> penDown = penHover;
					Ref<Pen> penDisabled = Pen::createSolidPen(w, Color(90, 90, 90));
					Color colorCheckNormal = Color::Black;
					Color colorCheckDisabled = Color(90, 90, 90);
					Color colorCheckHover = Color(0, 80, 200);
					Color colorCheckDown = colorCheckHover;
					categories[0].properties[(int)ButtonState::Normal].icon = new Icon(penNormal, colorBackNormal, Color::zero());
					categories[0].properties[(int)ButtonState::Disabled].icon = new Icon(penDisabled, colorBackDisabled, Color::zero());
					categories[0].properties[(int)ButtonState::Hover].icon = new Icon(penHover, colorBackHover, Color::zero());
					categories[0].properties[(int)ButtonState::Pressed].icon = new Icon(penDown, colorBackDown, Color::zero());
					
					categories[1] = categories[0];
					categories[1].properties[(int)ButtonState::Normal].icon = new Icon(penNormal, colorBackNormal, colorCheckNormal);
					categories[1].properties[(int)ButtonState::Disabled].icon = new Icon(penDisabled, colorBackDisabled, colorCheckDisabled);
					categories[1].properties[(int)ButtonState::Hover].icon = new Icon(penHover, colorBackHover, colorCheckHover);
					categories[1].properties[(int)ButtonState::Pressed].icon = new Icon(penDown, colorBackDown, colorCheckDown);
				}
				
				static ButtonCategory* getCategories()
				{
					SLIB_SAFE_STATIC(Categories, ret)
					if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
						return sl_null;
					}
					return ret.categories;
				}

			};

		}
	}

	SLIB_DEFINE_OBJECT(RadioButton, CheckBox)

	RadioButton::RadioButton() : CheckBox(2, priv::radio_button::Categories::getCategories())
	{
	}

	RadioButton::RadioButton(sl_uint32 nCategories, ButtonCategory* categories) : CheckBox(nCategories, categories)
	{
	}

	RadioButton::~RadioButton()
	{
	}
	
	Ref<RadioGroup> RadioButton::getGroup()
	{
		return m_group;
	}
	
	String RadioButton::getValue()
	{
		return m_value;
	}
	
	void RadioButton::setValue(const String& value)
	{
		m_value = value;
	}
	
	void RadioButton::setChecked(sl_bool flag, UIUpdateMode mode)
	{
		Ref<RadioGroup> group = m_group;
		if (group.isNotNull()) {
			group->_setChecked(this, flag, mode);
			return;
		}
		CheckBox::setChecked(flag, mode);
	}
	
	void RadioButton::dispatchClickEvent(UIEvent* ev)
	{
		Ref<RadioGroup> group = m_group;
		if (group.isNotNull()) {
			group->select(this);
			group->dispatchSelect(this);
		} else {
			CheckBox::setChecked(sl_true);
		}
		Button::dispatchClickEvent(ev);
	}

	
	SLIB_DEFINE_OBJECT(RadioGroup, Object)

	RadioGroup::RadioGroup()
	{
	}

	RadioGroup::~RadioGroup()
	{
	}
	
	List< Ref<RadioButton> > RadioGroup::getButtons()
	{
		ObjectLocker lock(this);
		return m_buttons.duplicate_NoLock();
	}

	void RadioGroup::add(const Ref<RadioButton>& button)
	{
		if (button.isNull()) {
			return;
		}
		button->m_group = this;
		ObjectLocker lock(this);
		m_buttons.addIfNotExist_NoLock(button);
		if (button->isChecked()) {
			if (button != m_buttonSelected) {
				if (m_buttonSelected.isNotNull()) {
					m_buttonSelected->CheckBox::setChecked(sl_false);
				}
				m_buttonSelected = button;
			}
		}
	}

	void RadioGroup::remove(const Ref<RadioButton>& button)
	{
		if (button.isNull()) {
			return;
		}
		button->m_group.setNull();
		ObjectLocker lock(this);
		m_buttons.remove_NoLock(button);
		if (m_buttonSelected == button) {
			m_buttonSelected.setNull();
		}
	}

	void RadioGroup::select(const Ref<RadioButton>& button)
	{
		ObjectLocker lock(this);
		if (button != m_buttonSelected) {
			if (m_buttonSelected.isNotNull()) {
				m_buttonSelected->CheckBox::setChecked(sl_false);
			}
			m_buttonSelected = button;
			if (button.isNotNull()) {
				button->CheckBox::setChecked(sl_true);
			}
		}
	}

	Ref<RadioButton> RadioGroup::getSelected()
	{
		ObjectLocker lock(this);
		return m_buttonSelected;
	}

	void RadioGroup::selectValue(const String& value)
	{
		ObjectLocker lock(this);
		ListElements< Ref<RadioButton> > buttons(m_buttons);
		Ref<RadioButton> selected;
		for (sl_size i = 0; i < buttons.count; i++) {
			Ref<RadioButton>& button = buttons[i];
			if (button->m_value == value) {
				selected = button;
				break;
			}
		}
		select(selected);
	}
	
	String RadioGroup::getSelectedValue()
	{
		ObjectLocker lock(this);
		if (m_buttonSelected.isNotNull()) {
			return m_buttonSelected->m_value;
		}
		return sl_null;
	}
	
	void RadioGroup::_setChecked(RadioButton* button, sl_bool flag, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		if (flag) {
			if (button != m_buttonSelected) {
				if (m_buttonSelected.isNotNull()) {
					m_buttonSelected->CheckBox::setChecked(sl_false);
				}
				m_buttonSelected = button;
				button->CheckBox::setChecked(sl_true, mode);
			}
		} else {
			if (button == m_buttonSelected) {
				m_buttonSelected.setNull();
			}
			button->CheckBox::setChecked(sl_false, mode);
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(RadioGroup, Select, RadioButton*)
	
	void RadioGroup::dispatchSelect(RadioButton* button)
	{
		SLIB_INVOKE_EVENT_HANDLER(Select, button)
	}
	

#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	Ref<ViewInstance> RadioButton::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
#endif

}

