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

#include "slib/core/safe_static.h"

namespace slib
{

	class _priv_RadioButton_Icon : public Drawable
	{
	public:
		Ref<Pen> m_penBorder;
		Ref<Brush> m_brushBack;
		Ref<Brush> m_brushCheck;
		
	public:
		_priv_RadioButton_Icon(const Ref<Pen>& penBorder, const Color& backColor, const Color& checkColor)
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
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override
		{
			canvas->drawEllipse(rectDst, m_penBorder, m_brushBack);
			if (m_brushCheck.isNotNull()) {
				Rectangle rcCheck;
				rcCheck.setLeftTop(rectDst.getCenter());
				sl_real w = rectDst.getWidth() / 2;
				sl_real h = rectDst.getHeight() / 2;
				rcCheck.left -= w / 2;
				rcCheck.top -= h / 2;
				rcCheck.right = rcCheck.left + w;
				rcCheck.bottom = rcCheck.top + h;
				canvas->fillEllipse(rcCheck, m_brushCheck);
			}
		}
		
	};

	class _priv_RadioButton_Categories
	{
	public:
		ButtonCategory categories[2];
		
		_priv_RadioButton_Categories()
		{
			Color colorBackNormal = Color::White;
			Color colorBackHover = Color::White;
			Color colorBackDown(220, 230, 255);
			Color colorBackDisabled = Color(220, 220, 220);
			Ref<Pen> penNormal = Pen::createSolidPen(1, Color::Black);
			Ref<Pen> penHover = Pen::createSolidPen(1, Color(0, 80, 200));
			Ref<Pen> penDown = penHover;
			Ref<Pen> penDisabled = Pen::createSolidPen(1, Color(90, 90, 90));
			Color colorCheckNormal = Color::Black;
			Color colorCheckDisabled = Color(90, 90, 90);
			Color colorCheckHover = Color(0, 80, 200);
			Color colorCheckDown = colorCheckHover;
			categories[0].properties[(int)ButtonState::Normal].icon = new _priv_RadioButton_Icon(penNormal, colorBackNormal, Color::zero());
			categories[0].properties[(int)ButtonState::Disabled].icon = new _priv_RadioButton_Icon(penDisabled, colorBackDisabled, Color::zero());
			categories[0].properties[(int)ButtonState::Hover].icon = new _priv_RadioButton_Icon(penHover, colorBackHover, Color::zero());
			categories[0].properties[(int)ButtonState::Pressed].icon = new _priv_RadioButton_Icon(penDown, colorBackDown, Color::zero());
			
			categories[1] = categories[0];
			categories[1].properties[(int)ButtonState::Normal].icon = new _priv_RadioButton_Icon(penNormal, colorBackNormal, colorCheckNormal);
			categories[1].properties[(int)ButtonState::Disabled].icon = new _priv_RadioButton_Icon(penDisabled, colorBackDisabled, colorCheckDisabled);
			categories[1].properties[(int)ButtonState::Hover].icon = new _priv_RadioButton_Icon(penHover, colorBackHover, colorCheckHover);
			categories[1].properties[(int)ButtonState::Pressed].icon = new _priv_RadioButton_Icon(penDown, colorBackDown, colorCheckDown);
		}
		
		static ButtonCategory* getCategories()
		{
			SLIB_SAFE_STATIC(_priv_RadioButton_Categories, ret)
			if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
				return sl_null;
			}
			return ret.categories;
		}

	};


	SLIB_DEFINE_OBJECT(RadioButton, CheckBox)

	RadioButton::RadioButton() : CheckBox(2, _priv_RadioButton_Categories::getCategories())
	{
		setCreatingNativeWidget(sl_true);
	}

	RadioButton::RadioButton(sl_uint32 nCategories, ButtonCategory* categories) : CheckBox(nCategories, categories)
	{
	}

	RadioButton::~RadioButton()
	{
	}

	void RadioButton::dispatchClickEvent(UIEvent* ev)
	{
		CheckBox::dispatchClickEvent(ev);
		Ref<RadioGroup> group = getRadioGroup();
		if (group.isNotNull()) {
			group->select(this);
		}
	}

	SLIB_DEFINE_OBJECT(RadioGroup, Object)

	RadioGroup::RadioGroup()
	{
	}

	RadioGroup::~RadioGroup()
	{
	}

	void RadioGroup::add(const Ref<RadioButton>& view)
	{
		if (view.isNotNull()) {
			view->setRadioGroup(this);
			m_views.addIfNotExist(view);
			select(m_viewSelected);
		}
	}

	void RadioGroup::remove(const Ref<RadioButton>& view)
	{
		if (view.isNotNull()) {
			if (view->getRadioGroup() == this) {
				view->setRadioGroup(Ref<RadioGroup>::null());
			}
			m_views.remove(view);
		}
	}

	void RadioGroup::select(const Ref<RadioButton>& sel)
	{
		m_viewSelected = sel;
		ListLocker< Ref<RadioButton> > views(m_views);
		for (sl_size i = 0; i < views.count; i++) {
			Ref<RadioButton> view = views[i];
			if (view.isNotNull() && view != sel) {
				view->setChecked(sl_false);
			}
		}
		if (sel.isNotNull()) {
			sel->setChecked(sl_true);
		}
	}

	Ref<RadioButton> RadioGroup::getSelected()
	{
		return m_viewSelected;
	}


#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	Ref<ViewInstance> RadioButton::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
#endif

}

