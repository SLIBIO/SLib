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

#include "slib/ui/check_box.h"

#include "slib/ui/core.h"
#include "slib/ui/resource.h"
#include "slib/core/safe_static.h"

namespace slib
{

	class _priv_CheckBox_Icon : public Drawable
	{
	public:
		Ref<Pen> m_penBorder;
		Ref<Brush> m_brush;
		Ref<Pen> m_penCheck;
		
	public:
		_priv_CheckBox_Icon(const Ref<Pen>& penBorder, const Color& backColor, const Ref<Pen>& penCheck)
		{
			m_penBorder = penBorder;
			if (backColor.a > 0) {
				m_brush = Brush::createSolidBrush(backColor);
			}
			m_penCheck = penCheck;
		}
		
	public:
		void onDrawAll(Canvas* canvas, const Rectangle& rect, const DrawParam& param) override
		{
			sl_bool flagAntiAlias = canvas->isAntiAlias();
			canvas->setAntiAlias(sl_false);
			canvas->drawRectangle(rect, m_penBorder, m_brush);
			canvas->setAntiAlias(flagAntiAlias);
			
			if (m_penCheck.isNotNull()) {
				Point pts[3];
				pts[0] = Point(0.2f, 0.6f);
				pts[1] = Point(0.4f, 0.8f);
				pts[2] = Point(0.8f, 0.3f);
				for (int i = 0; i < 3; i++) {
					pts[i].x = rect.left + rect.getWidth() * pts[i].x;
					pts[i].y = rect.top + rect.getHeight() * pts[i].y;
				}
				canvas->drawLines(pts, 3, m_penCheck);
			}
		}
		
	};

	class _priv_CheckBox_Categories
	{
	public:
		ButtonCategory categories[2];
		
	public:
		_priv_CheckBox_Categories()
		{
			sl_real w = (sl_real)(UIResource::toUiPos(UIResource::dpToPixel(1)));
			Color colorBackNormal = Color::White;
			Color colorBackHover = Color::White;
			Color colorBackDown(220, 230, 255);
			Color colorBackDisabled(220, 220, 220);
			Ref<Pen> penNormal = Pen::createSolidPen(w, Color::Black);
			Ref<Pen> penHover = Pen::createSolidPen(w, Color(0, 80, 200));
			Ref<Pen> penDown = penHover;
			Ref<Pen> penDisabled = Pen::createSolidPen(w, Color(90, 90, 90));
			Ref<Pen> penCheckNormal = Pen::createSolidPen(w*2, Color::Black);
			Ref<Pen> penCheckHover = Pen::createSolidPen(w*2, Color(0, 80, 200));
			Ref<Pen> penCheckDown = penCheckHover;
			Ref<Pen> penCheckDisabled = Pen::createSolidPen(w*2, Color(90, 90, 90));
			categories[0].properties[(int)ButtonState::Normal].icon = new _priv_CheckBox_Icon(penNormal, colorBackNormal, Ref<Pen>::null());
			categories[0].properties[(int)ButtonState::Disabled].icon = new _priv_CheckBox_Icon(penDisabled, colorBackDisabled, Ref<Pen>::null());
			categories[0].properties[(int)ButtonState::Hover].icon = new _priv_CheckBox_Icon(penHover, colorBackHover, Ref<Pen>::null());
			categories[0].properties[(int)ButtonState::Pressed].icon = new _priv_CheckBox_Icon(penDown, colorBackDown, Ref<Pen>::null());
			
			categories[1] = categories[0];
			categories[1].properties[(int)ButtonState::Normal].icon = new _priv_CheckBox_Icon(penNormal, colorBackNormal, penCheckNormal);
			categories[1].properties[(int)ButtonState::Disabled].icon = new _priv_CheckBox_Icon(penDisabled, colorBackDisabled, penCheckDisabled);
			categories[1].properties[(int)ButtonState::Hover].icon = new _priv_CheckBox_Icon(penHover, colorBackHover, penCheckHover);
			categories[1].properties[(int)ButtonState::Pressed].icon = new _priv_CheckBox_Icon(penDown, colorBackDown, penCheckDown);
		}
		
	public:
		static ButtonCategory* getCategories()
		{
			SLIB_SAFE_STATIC(_priv_CheckBox_Categories, ret)
			if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
				return sl_null;
			}
			return ret.categories;
		}
	};

	SLIB_DEFINE_OBJECT(CheckBox, Button)

	CheckBox::CheckBox() : CheckBox(2, _priv_CheckBox_Categories::getCategories())
	{
	}

	CheckBox::CheckBox(sl_uint32 nCategories, ButtonCategory* categories) : Button(nCategories, categories)
	{
		setCreatingNativeWidget(sl_false);
		
		m_flagChecked = sl_false;
		
		setGravity(Alignment::MiddleLeft, UIUpdateMode::Init);
		setIconAlignment(Alignment::MiddleLeft, UIUpdateMode::Init);
		setTextAlignment(Alignment::MiddleLeft, UIUpdateMode::Init);
		
		setBorder(sl_false, UIUpdateMode::Init);
		setBackground(Ref<Drawable>::null(), UIUpdateMode::Init);
		
		setTextColor(Color::Black, UIUpdateMode::Init);
		setTextMarginLeft(2 * UIResource::toUiPos(UIResource::dpToPixel(1)), UIUpdateMode::Init);
	}

	CheckBox::~CheckBox()
	{
	}

	sl_bool CheckBox::isChecked()
	{
		if (isNativeWidget()) {
			_getChecked_NW();
		}
		return m_flagChecked;
	}

	void CheckBox::setChecked(sl_bool flag, UIUpdateMode mode)
	{
		m_flagChecked = flag;
		if (isNativeWidget()) {
			setCurrentCategory(flag ? 1 : 0, UIUpdateMode::None);
			_setChecked_NW(flag);
		} else {
			setCurrentCategory(flag ? 1 : 0, mode);
		}
	}
	
	UISize _priv_CheckBox_macOS_measureSize(Button* view);
	UISize _priv_CheckBox_Win32_measureSize(Button* view);

	UISize CheckBox::measureLayoutContentSize()
	{
#if defined(SLIB_PLATFORM_IS_MACOS)
		if (isCreatingNativeWidget()) {
			return _priv_CheckBox_macOS_measureSize(this);
		}
#endif
#if defined(SLIB_PLATFORM_IS_WIN32)
		if (isCreatingNativeWidget()) {
			return _priv_CheckBox_Win32_measureSize(this);
		}
#endif
		return measureContentSize();
	}

	void CheckBox::onClickEvent(UIEvent* ev)
	{
		if (isNativeWidget()) {
			_getChecked_NW();
		} else {
			setChecked(!m_flagChecked);
		}
	}

#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	Ref<ViewInstance> CheckBox::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}

	void CheckBox::_getChecked_NW()
	{
	}

	void CheckBox::_setChecked_NW(sl_bool flag)
	{
	}
#endif

}

