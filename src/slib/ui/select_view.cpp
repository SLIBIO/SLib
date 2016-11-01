#include "../../../inc/slib/ui/select_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(SelectView, View)

enum
{
	ICON_NONE = 0,
	ICON_LEFT = 1,
	ICON_RIGHT = 2,
	ICON_DOWN = 3
};

class _SelectView_DefaultIcon : public Drawable
{
public:
	Ref<Brush> m_brush;
	Point m_pts[3];
	
public:
	_SelectView_DefaultIcon(int type)
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
	// override
	sl_real getDrawableWidth()
	{
		return 1;
	}
	
	// override
	sl_real getDrawableHeight()
	{
		return 1;
	}
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
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

class _SelectView_DefaultResources
{
public:
	Ref<Drawable> leftIcon;
	Ref<Drawable> rightIcon;
	Ref<Drawable> downIcon;
	
public:
	_SelectView_DefaultResources()
	{
		leftIcon = new _SelectView_DefaultIcon(ICON_LEFT);
		rightIcon = new _SelectView_DefaultIcon(ICON_RIGHT);
		downIcon = new _SelectView_DefaultIcon(ICON_DOWN);
	}
	
};

SLIB_SAFE_STATIC_GETTER(_SelectView_DefaultResources, _SelectView_getDefaultResources)

SelectView::SelectView()
{
	setCreatingNativeWidget(sl_true);
	setUsingFont(sl_true);
	setBorder(sl_true, UIUpdateMode::NoRedraw);
	
	m_indexSelected = 0;
	
	m_iconSize.x = 0;
	m_iconSize.y = 0;
	
	m_textColor = Color::Black;
	
	_SelectView_DefaultResources* def = _SelectView_getDefaultResources();
	if (def) {
		m_leftIcon = def->leftIcon;
		m_rightIcon = def->rightIcon;
	}

	m_clickedIconNo = ICON_NONE;
	
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
			selectIndex(0, UIUpdateMode::NoRedraw);
		}
	} else {
		if (m_indexSelected >= n) {
			selectIndex(0, UIUpdateMode::NoRedraw);
		}
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void SelectView::removeAllItems(UIUpdateMode mode)
{
	setItemsCount(0, mode);
}

String SelectView::getItemValue(sl_uint32 index)
{
	return m_values.getItemValue(index, String::null());
}

void SelectView::setItemValue(sl_uint32 index, const String& value)
{
	m_values.setItem(index, value);
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
	return m_titles.getItemValue(index, String::null());
}

void SelectView::setItemTitle(sl_uint32 index, const String& title, UIUpdateMode mode)
{
	if (index < m_titles.getCount()) {
		m_titles.setItem(index, title);
		if (isNativeWidget()) {
			_setItemTitle_NW(index, title);
		} else {
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
			selectIndex(0, UIUpdateMode::NoRedraw);
		}
	} else {
		sl_uint32 n = (sl_uint32)(m_titles.getCount());
		if (m_indexSelected >= n) {
			selectIndex(0, UIUpdateMode::NoRedraw);
		}
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void SelectView::selectIndex(sl_uint32 index, UIUpdateMode mode)
{
	if (index < m_titles.getCount()) {
		m_indexSelected = index;
		if (isNativeWidget()) {
			_select_NW(index);
		} else {
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
	if (isNativeWidget()) {
		_getSelectedIndex_NW();
	}
	return m_indexSelected;
}

String SelectView::getSelectedValue()
{
	return m_values.getItemValue(m_indexSelected, String::null());
}

String SelectView::getSelectedTitle()
{
	return m_titles.getItemValue(m_indexSelected, String::null());
}

const UISize& SelectView::getIconSize()
{
	return m_iconSize;
}

void SelectView::setIconSize(const UISize& size, UIUpdateMode mode)
{
	m_iconSize = size;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
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
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

Ref<Drawable> SelectView::getRightIcon()
{
	return m_rightIcon;
}

void SelectView::setRightIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
{
	m_rightIcon = icon;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

Color SelectView::getTextColor()
{
	return m_textColor;
}

void SelectView::setTextColor(const Color& color, UIUpdateMode mode)
{
	m_textColor = color;
	if (mode == UIUpdateMode::Redraw) {
		invalidate();
	}
}

void SelectView::onSelectItem(sl_uint32 index)
{
	PtrLocker<ISelectViewListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onSelectItem(this, index);
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
					onSelectItem(index);
					invalidate();
				}
			}
		} else if (m_clickedIconNo == ICON_RIGHT) {
			if (getRightIconRegion().containsPoint(pt)) {
				sl_uint32 index = m_indexSelected;
				if (index + 1 < getItemsCount()) {
					index ++;
					selectIndex(index);
					onSelectItem(index);
					invalidate();
				}
			}
		}
		m_clickedIconNo = ICON_NONE;
	}
}

void SelectView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagVertical && !flagHorizontal) {
		return;
	}
	
	if (flagHorizontal) {
		sl_ui_pos width = m_iconSize.x * 2 + getPaddingLeft() + getPaddingRight();
		if (width < 0) {
			width = 0;
		}
		setMeasuredWidth(width);
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
		setMeasuredHeight(height);
	}
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

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void SelectView::_getSelectedIndex_NW()
{
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

SLIB_UI_NAMESPACE_END
