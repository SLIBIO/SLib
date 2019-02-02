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

#include "slib/ui/select_view.h"

#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(SelectView, View)
	
	enum
	{
		ICON_NONE = 0,
		ICON_LEFT = 1,
		ICON_RIGHT = 2,
		ICON_DOWN = 3
	};
	
	class _priv_SelectView_DefaultIcon : public Drawable
	{
	public:
		Ref<Brush> m_brush;
		Point m_pts[3];
		
	public:
		_priv_SelectView_DefaultIcon(int type)
		{
			m_brush = Brush::createSolidBrush(Color::Black);
			if (type == ICON_LEFT) {
				m_pts[0] = Point(0.67f, 0.24f);
				m_pts[1] = Point(0.33f, 0.51f);
				m_pts[2] = Point(0.67f, 0.78f);
			} else if (type == ICON_RIGHT) {
				m_pts[0] = Point(0.33f, 0.24f);
				m_pts[1] = Point(0.67f, 0.51f);
				m_pts[2] = Point(0.33f, 0.78f);
			} else if (type == ICON_DOWN) {
				m_pts[0] = Point(0.3f, 0.35f);
				m_pts[1] = Point(0.5f, 0.65f);
				m_pts[2] = Point(0.7f, 0.35f);
			}
		}
		
	public:
		sl_real getDrawableWidth() override
		{
			return 1;
		}
		
		sl_real getDrawableHeight() override
		{
			return 1;
		}
		
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override
		{
			if (m_brush.isNotNull()) {
				Point pts[3];
				for (int i = 0; i < 3; i++) {
					pts[i].x = rectDst.left + rectDst.getWidth() * m_pts[i].x;
					pts[i].y = rectDst.top + rectDst.getHeight() * m_pts[i].y;
				}
				canvas->fillPolygon(pts, 3, m_brush);
			}
		}
		
	};
	
	class _priv_SelectView_DefaultResources
	{
	public:
		Ref<Drawable> leftIcon;
		Ref<Drawable> rightIcon;
		Ref<Drawable> downIcon;
		
	public:
		_priv_SelectView_DefaultResources()
		{
			leftIcon = new _priv_SelectView_DefaultIcon(ICON_LEFT);
			rightIcon = new _priv_SelectView_DefaultIcon(ICON_RIGHT);
			downIcon = new _priv_SelectView_DefaultIcon(ICON_DOWN);
		}
		
	};
	
	SLIB_SAFE_STATIC_GETTER(_priv_SelectView_DefaultResources, _priv_SelectView_getDefaultResources)
	
	SelectView::SelectView()
	{
		setCreatingNativeWidget(sl_true);
		setUsingFont(sl_true);
		setBorder(sl_true, UIUpdateMode::Init);
		setBackgroundColor(Color::White, UIUpdateMode::Init);
		setSavingCanvasState(sl_false);
		
		m_indexSelected = 0;
		
		m_iconSize.x = 0;
		m_iconSize.y = 0;
		
		m_textAlignment = Alignment::MiddleLeft;
		m_textColor = Color::Black;
		
		_priv_SelectView_DefaultResources* def = _priv_SelectView_getDefaultResources();
		if (def) {
			m_leftIcon = def->leftIcon;
			m_rightIcon = def->rightIcon;
		}
		
		m_clickedIconNo = ICON_NONE;
	}
	
	SelectView::~SelectView()
	{
	}

	sl_uint32 SelectView::getItemsCount()
	{
		return (sl_uint32)(m_titles.getCount());
	}
	
	void SelectView::setItemsCount(sl_uint32 n, UIUpdateMode mode)
	{
		m_values.setCount(n);
		m_titles.setCount(n);
		if (isNativeWidget()) {
			_refreshItemsCount_NW();
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
		} else {
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
			invalidate(mode);
		}
	}
	
	void SelectView::removeAllItems(UIUpdateMode mode)
	{
		setItemsCount(0, mode);
	}
	
	String SelectView::getItemValue(sl_uint32 index)
	{
		return m_values.getValueAt(index);
	}
	
	void SelectView::setItemValue(sl_uint32 index, const String& value)
	{
		m_values.setAt(index, value);
	}
	
	List<String> SelectView::getValues()
	{
		return m_values;
	}
	
	void SelectView::setValues(const List<String>& list)
	{
		m_values = list;
	}
	
	String SelectView::getItemTitle(sl_uint32 index)
	{
		return m_titles.getValueAt(index);
	}
	
	void SelectView::setItemTitle(sl_uint32 index, const String& title, UIUpdateMode mode)
	{
		if (index < m_titles.getCount()) {
			m_titles.setAt(index, title);
			if (isNativeWidget()) {
				_setItemTitle_NW(index, title);
			} else {
				invalidate(mode);
			}
		}
	}
	
	List<String> SelectView::getTitles()
	{
		return m_titles;
	}
	
	void SelectView::setTitles(const List<String>& list, UIUpdateMode mode)
	{
		m_titles = list;
		if (isNativeWidget()) {
			_refreshItemsContent_NW();
			sl_uint32 n = (sl_uint32)(m_titles.getCount());
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
		} else {
			sl_uint32 n = (sl_uint32)(m_titles.getCount());
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
			invalidate(mode);
		}
	}
	
	void SelectView::addItem(const String& value, const String& title, UIUpdateMode mode)
	{
		sl_size n = m_values.getCount();
		m_values.setCount(n + 1);
		m_titles.setCount(n + 1);
		m_values.setAt(n, value);
		m_titles.setAt(n, title);
		if (isNativeWidget()) {
			_refreshItemsCount_NW();
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
		} else {
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
			invalidate(mode);
		}
	}
	
	void SelectView::selectIndex(sl_uint32 index, UIUpdateMode mode)
	{
		if (index < m_titles.getCount()) {
			m_indexSelected = index;
			if (isNativeWidget()) {
				_select_NW(index);
			} else {
				invalidate(mode);
			}
		} else {
			if (index == 0) {
				m_indexSelected = 0;
			}
		}
	}
	
	void SelectView::selectValue(const String& value, UIUpdateMode mode)
	{
		sl_int32 m = (sl_int32)(m_values.indexOf(value));
		if (m > 0) {
			selectIndex(m, mode);
		}
	}
	
	sl_uint32 SelectView::getSelectedIndex()
	{
		return m_indexSelected;
	}
	
	String SelectView::getSelectedValue()
	{
		return m_values.getValueAt(m_indexSelected);
	}
	
	String SelectView::getSelectedTitle()
	{
		return m_titles.getValueAt(m_indexSelected);
	}
	
	const UISize& SelectView::getIconSize()
	{
		return m_iconSize;
	}
	
	void SelectView::setIconSize(const UISize& size, UIUpdateMode mode)
	{
		m_iconSize = size;
		invalidateLayoutOfWrappingControl(mode);
	}
	
	void SelectView::setIconSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode)
	{
		setIconSize(UISize(width, height), mode);
	}
	
	void SelectView::setIconSize(sl_ui_len size, UIUpdateMode mode)
	{
		setIconSize(UISize(size, size),mode);
	}
	
	sl_ui_len SelectView::getIconWidth()
	{
		return m_iconSize.x;
	}
	
	void SelectView::setIconWidth(sl_ui_len width, UIUpdateMode mode)
	{
		setIconSize(UISize(width, m_iconSize.y), mode);
	}
	
	sl_ui_len SelectView::getIconHeight()
	{
		return m_iconSize.y;
	}
	
	void SelectView::setIconHeight(sl_ui_len height, UIUpdateMode mode)
	{
		setIconSize(UISize(m_iconSize.x, height), mode);
	}
	
	Ref<Drawable> SelectView::getLeftIcon()
	{
		return m_leftIcon;
	}
	
	void SelectView::setLeftIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
	{
		m_leftIcon = icon;
		invalidate(mode);
	}
	
	Ref<Drawable> SelectView::getRightIcon()
	{
		return m_rightIcon;
	}
	
	void SelectView::setRightIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
	{
		m_rightIcon = icon;
		invalidate(mode);
	}
	
	Alignment SelectView::getGravity()
	{
		return m_textAlignment;
	}
	
	void SelectView::setGravity(Alignment align, UIUpdateMode mode)
	{
		m_textAlignment = align;
		if (isNativeWidget()) {
			_setTextAlignment_NW(align);
		} else {
			invalidate(mode);
		}
	}
	
	Color SelectView::getTextColor()
	{
		return m_textColor;
	}
	
	void SelectView::setTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColor = color;
		if (isNativeWidget()) {
			_setTextColor_NW(color);
		} else {
			invalidate(mode);
		}
	}
	
	void SelectView::onDraw(Canvas* canvas)
	{
		canvas->drawText(getSelectedTitle(), getBounds(), getFont(), m_textColor, Alignment::MiddleCenter);
		canvas->draw(getLeftIconRegion(), m_leftIcon);
		canvas->draw(getRightIconRegion(), m_rightIcon);
	}
	
	void SelectView::onMouseEvent(UIEvent* ev)
	{
		UIAction action = ev->getAction();
		UIPoint pt = ev->getPoint();
		if (action == UIAction::LeftButtonDown || action == UIAction::TouchBegin) {
			if (getLeftIconRegion().containsPoint(pt)) {
				m_clickedIconNo = ICON_LEFT;
				ev->stopPropagation();
			} else if (getRightIconRegion().containsPoint(pt)) {
				m_clickedIconNo = ICON_RIGHT;
				ev->stopPropagation();
			}
		} else if (action == UIAction::MouseLeave || action == UIAction::TouchCancel) {
			m_clickedIconNo = ICON_NONE;
		} else if (action == UIAction::LeftButtonUp || action == UIAction::TouchEnd) {
			if (m_clickedIconNo == ICON_LEFT) {
				if (getLeftIconRegion().containsPoint(pt)) {
					sl_uint32 index = m_indexSelected;
					if (index > 0) {
						index --;
						selectIndex(index);
						dispatchSelectItem(index);
						invalidate();
					}
				}
			} else if (m_clickedIconNo == ICON_RIGHT) {
				if (getRightIconRegion().containsPoint(pt)) {
					sl_uint32 index = m_indexSelected;
					if (index + 1 < getItemsCount()) {
						index ++;
						selectIndex(index);
						dispatchSelectItem(index);
						invalidate();
					}
				}
			}
			m_clickedIconNo = ICON_NONE;
		}
	}
	
	UISize _priv_SelectView_macOS_measureSize(SelectView* view);

	void SelectView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
#ifdef SLIB_UI_IS_MACOS
		if (isCreatingNativeWidget()) {
			UISize size = _priv_SelectView_macOS_measureSize(this);
			if (flagHorizontal) {
				setLayoutWidth(size.x);
			}
			if (flagVertical) {
				setLayoutHeight(size.y);
			}
			return;
		}
#endif
		if (flagHorizontal) {
			sl_ui_pos width = m_iconSize.x * 2 + getPaddingLeft() + getPaddingRight();
			if (width < 0) {
				width = 0;
			}
			setLayoutWidth(width);
		}
		if (flagVertical) {
			sl_ui_pos height = 0;
			Ref<Font> font = getFont();
			if (font.isNotNull()) {
				height = (sl_ui_pos)(font->getFontHeight() * 1.5f);
				if (height < 0) {
					height = 0;
				}
			}
			if (height < m_iconSize.y) {
				height = m_iconSize.y;
			}
			height += getPaddingTop() + getPaddingBottom();
			if (height < 0) {
				height = 0;
			}
			setLayoutHeight(height);
		}
	}
	
	SLIB_DEFINE_EVENT_HANDLER(SelectView, SelectItem, sl_uint32 index)

	void SelectView::dispatchSelectItem(sl_uint32 index)
	{
		m_indexSelected = index;
		
		SLIB_INVOKE_EVENT_HANDLER(SelectItem, index)
	}
	
	UIRect SelectView::getLeftIconRegion()
	{
		sl_ui_pos heightView = getHeight();
		sl_ui_pos h = heightView - getPaddingTop() - getPaddingBottom();
		if (h < 0) {
			h = 0;
		}
		UIRect ret;
		ret.left = getPaddingLeft();
		if (m_iconSize.x > 0) {
			ret.right = ret.left + m_iconSize.x;
		} else {
			ret.right = ret.left + h;
		}
		if (m_iconSize.y > 0) {
			h = m_iconSize.y;
		}
		ret.top = (heightView - h) / 2;
		ret.bottom = ret.top + h;
		ret.fixSizeError();
		return ret;
	}
	
	UIRect SelectView::getRightIconRegion()
	{
		UISize sizeView = getSize();
		sl_ui_pos h = sizeView.y - getPaddingTop() - getPaddingBottom();
		if (h < 0) {
			h = 0;
		}
		UIRect ret;
		ret.right = sizeView.x - getPaddingRight();
		if (m_iconSize.x > 0) {
			ret.left = ret.right - m_iconSize.x;
		} else {
			ret.left = ret.right - h;
		}
		if (m_iconSize.y > 0) {
			h = m_iconSize.y;
		}
		ret.top = (sizeView.y - h) / 2;
		ret.bottom = ret.top + h;
		ret.fixSizeError();
		return ret;
	}
	
#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_WIN32) && !defined(SLIB_UI_IS_ANDROID)
	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
	
	void SelectView::_select_NW(sl_uint32 index)
	{
	}
	
	void SelectView::_refreshItemsCount_NW()
	{
	}
	
	void SelectView::_refreshItemsContent_NW()
	{
	}
	
	void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
	{
	}
	
	void SelectView::_setFont_NW(const Ref<Font>& font)
	{
	}
#endif
	
#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
	void SelectView::_setTextAlignment_NW(Alignment align)
	{
	}
	
	void SelectView::_setTextColor_NW(const Color& color)
	{
	}
	
	void SelectView::_setBorder_NW(sl_bool flag)
	{
	}
	
	void SelectView::_setBackgroundColor_NW(const Color& color)
	{
	}
#endif

}
