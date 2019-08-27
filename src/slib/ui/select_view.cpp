/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/ui/core.h"
#include "slib/core/safe_static.h"

#if defined(SLIB_UI_IS_MACOS) || defined(SLIB_UI_IS_IOS) || defined(SLIB_UI_IS_WIN32) || defined(SLIB_UI_IS_ANDROID)
#	define HAS_NATIVE_WIDGET_IMPL
#endif

namespace slib
{

	namespace priv
	{
		namespace select_view
		{

			enum
			{
				ICON_NONE = 0,
				ICON_LEFT = 1,
				ICON_RIGHT = 2,
				ICON_DOWN = 3
			};
			
			class DefaultIcon : public Drawable
			{
			public:
				Ref<Brush> m_brush;
				Point m_pts[3];
				
			public:
				DefaultIcon(int type)
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
			
			class DefaultResources
			{
			public:
				Ref<Drawable> leftIcon;
				Ref<Drawable> rightIcon;
				Ref<Drawable> downIcon;
				
			public:
				DefaultResources()
				{
					leftIcon = new DefaultIcon(ICON_LEFT);
					rightIcon = new DefaultIcon(ICON_RIGHT);
					downIcon = new DefaultIcon(ICON_DOWN);
				}
				
			};
			
			SLIB_SAFE_STATIC_GETTER(DefaultResources, GetDefaultResources)

		}
	}

	using namespace priv::select_view;

	SLIB_DEFINE_OBJECT(SelectView, View)
	
	SelectView::SelectView()
	{
#ifdef HAS_NATIVE_WIDGET_IMPL
		setCreatingNativeWidget(sl_true);
#endif
		setUsingFont(sl_true);
		setBorder(sl_true, UIUpdateMode::Init);
		setBackgroundColor(Color::White, UIUpdateMode::Init);
		setSavingCanvasState(sl_false);
#if !defined(SLIB_PLATFORM_IS_MOBILE)
		setFocusable(sl_true);
#endif
		
		m_indexSelected = 0;
		
		m_iconSize.x = 0;
		m_iconSize.y = 0;
		
		m_gravity = Alignment::MiddleLeft;
		m_textColor = Color::Black;
		
		DefaultResources* def = GetDefaultResources();
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
		Ptr<ISelectViewInstance> instance = getSelectViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&SelectView::setItemsCount, n, mode)
		}
		m_values.setCount(n);
		m_titles.setCount(n);
		if (instance.isNotNull()) {
			instance->refreshItemsCount(this);
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
		Ptr<ISelectViewInstance> instance = getSelectViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&SelectView::setItemTitle, index, title, mode)
		}
		if (index < m_titles.getCount()) {
			m_titles.setAt(index, title);
			if (instance.isNotNull()) {
				instance->setItemTitle(this, index, title);
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
		Ptr<ISelectViewInstance> instance = getSelectViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&SelectView::setTitles, list, mode)
			m_titles = list;
			instance->refreshItemsContent(this);
			sl_uint32 n = (sl_uint32)(m_titles.getCount());
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
		} else {
			m_titles = list;
			sl_uint32 n = (sl_uint32)(m_titles.getCount());
			if (m_indexSelected >= n) {
				selectIndex(0, UIUpdateMode::None);
			}
			invalidate(mode);
		}
	}
	
	void SelectView::addItem(const String& value, const String& title, UIUpdateMode mode)
	{
		Ptr<ISelectViewInstance> instance = getSelectViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&SelectView::addItem, value, title, mode)
		}
		sl_size n = m_values.getCount();
		m_values.setCount(n + 1);
		m_titles.setCount(n + 1);
		m_values.setAt(n, value);
		m_titles.setAt(n, title);
		if (instance.isNotNull()) {
			instance->refreshItemsCount(this);
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
		Ptr<ISelectViewInstance> instance = getSelectViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&SelectView::selectIndex, index, mode)
		}
		if (index < m_titles.getCount()) {
			m_indexSelected = index;
			if (instance.isNotNull()) {
				instance->select(this, index);
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
		return m_gravity;
	}
	
	void SelectView::setGravity(const Alignment& gravity, UIUpdateMode mode)
	{
		Ptr<ISelectViewInstance> instance = getSelectViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&SelectView::setGravity, gravity, mode)
			m_gravity = gravity;
			instance->setGravity(this, gravity);
		} else {
			m_gravity = gravity;
			invalidate(mode);
		}
	}
	
	Color SelectView::getTextColor()
	{
		return m_textColor;
	}
	
	void SelectView::setTextColor(const Color& color, UIUpdateMode mode)
	{
		Ptr<ISelectViewInstance> instance = getSelectViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&SelectView::setTextColor, color, mode)
			m_textColor = color;
			instance->setTextColor(this, color);
		} else {
			m_textColor = color;
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
	
	void SelectView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		Ptr<ISelectViewInstance> instance = getSelectViewInstance();
		if (instance.isNotNull()) {
			UISize size;
			if (instance->measureSize(this, size)) {
				if (flagHorizontal) {
					setLayoutWidth(size.x);
				}
				if (flagVertical) {
					setLayoutHeight(size.y);
				}
				return;
			}
		}

		Ref<Font> font = getFont();
		if (flagHorizontal) {
			sl_ui_pos width = m_iconSize.x * 2 + getPaddingLeft() + getPaddingRight();
			if (font.isNotNull()) {
				sl_ui_pos t = (sl_ui_pos)(font->getFontHeight());
				if (t > 0) {
					width += t * 4;
				}
			}
			if (width < 0) {
				width = 0;
			}
			setLayoutWidth(width);
		}
		if (flagVertical) {
			sl_ui_pos height = 0;
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
		ObjectLocker lock(this);
		if (m_indexSelected == index) {
			return;
		}
		m_indexSelected = index;
		lock.unlock();
		
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
	
#if !defined(HAS_NATIVE_WIDGET_IMPL)
	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
	
	Ptr<ISelectViewInstance> SelectView::getSelectViewInstance()
	{
		return sl_null;
	}
#endif
	
	void ISelectViewInstance::setGravity(SelectView* view, const Alignment& gravity)
	{
	}
	
	void ISelectViewInstance::setTextColor(SelectView* view, const Color& color)
	{
	}
	
	sl_bool ISelectViewInstance::measureSize(SelectView* view, UISize& _out)
	{
		return sl_false;
	}
	
}
