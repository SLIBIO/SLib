#include "../../../inc/slib/ui/scroll_view.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ScrollView, View)

ScrollView::ScrollView()
{
	SLIB_REFERABLE_CONSTRUCTOR

	setCreatingNativeWidget(sl_true);
	setCreatingChildInstances(sl_false);
    setMakingLayout(sl_true, UIUpdateMode::Init);
    setBorder(sl_true, UIUpdateMode::Init);

	m_flagPaging = sl_false;
	m_pageWidth = 0;
	m_pageHeight = 0;
	
	setHorizontalScrolling(sl_true);
	setVerticalScrolling(sl_true);
    
}

Ref<View> ScrollView::getContentView()
{
	return m_viewContent;
}

void ScrollView::setContentView(const Ref<slib::View>& view, UIUpdateMode mode)
{
	ObjectLocker lock(this);
	if (m_viewContent != view) {
		Ref<View> viewOld = m_viewContent;
		removeChild(viewOld, UIUpdateMode::NoRedraw);
		m_viewContent = view;
		if (view.isNotNull()) {
			view->setParent(this);
			view->setCreatingInstance(sl_true);
			view->setAttachMode(UIAttachMode::NotAttach);
			if (mode == UIUpdateMode::Init) {
				addChild(view, UIUpdateMode::Init);
			} else {
				addChild(view, UIUpdateMode::NoRedraw);
			}
			View::setContentSize(view->getWidth(), view->getHeight(), UIUpdateMode::NoRedraw);
		} else {
			View::setContentSize(0, 0, UIUpdateMode::NoRedraw);
		}
		if (mode != UIUpdateMode::Init) {
			if (isNativeWidget()) {
				if (UI::isUiThread()) {
					_setContentView_NW(view);
				} else {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setContentView_NW, this, view));
				}
			} else {
				UIPoint pt = View::getScrollPosition();
				_scrollTo(pt.x, pt.y, sl_false, UIUpdateMode::NoRedraw);
				if (mode == UIUpdateMode::Redraw) {
					invalidate();
				}
			}
		}
	}
}

void ScrollView::setContentSize(sl_scroll_pos _width, sl_scroll_pos _height, UIUpdateMode mode)
{
	sl_ui_pos width = (sl_ui_pos)(_width);
	if (width < 0) {
		width = 0;
	}
	sl_ui_pos height = (sl_ui_pos)(_height);
	if (height < 0) {
		height = 0;
	}
	ObjectLocker lock(this);
	Ref<View> viewContent = m_viewContent;
	if (viewContent.isNotNull()) {
		if (mode == UIUpdateMode::Init) {
			viewContent->setSize(width, height, UIUpdateMode::Init);
		} else {
			viewContent->setSize(width, height, UIUpdateMode::NoRedraw);
		}
	}
	View::setContentSize(_width, _height, mode);
	if (isNativeWidget()) {
		_refreshContentSize_NW();
	}
}

void ScrollView::setContentSize(const ScrollPoint& size, UIUpdateMode mode)
{
	setContentSize(size.x, size.y, mode);
}

ScrollPoint ScrollView::getScrollPosition()
{
	if (isNativeWidget()) {
		return _getScrollPosition_NW();
	}
	return View::getScrollPosition();
}

ScrollPoint ScrollView::getScrollRange()
{
	if (isNativeWidget()) {
		return _getScrollRange_NW();
	}
	return View::getScrollRange();
}

void ScrollView::scrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode)
{
	_scrollTo(x, y, sl_false, UIUpdateMode::NoRedraw);
	View::scrollTo(x, y, mode);
}

void ScrollView::scrollTo(const ScrollPoint& position, UIUpdateMode mode)
{
	scrollTo(position.x, position.y, mode);
}

void ScrollView::smoothScrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode)
{
    _scrollTo(x, y, sl_true, UIUpdateMode::NoRedraw);
    View::scrollTo(x, y, mode);
}

sl_bool ScrollView::isPaging()
{
	return m_flagPaging;
}

void ScrollView::setPaging(sl_bool flagPaging)
{
	m_flagPaging = flagPaging;
	_updatePaging();
}

sl_ui_len ScrollView::getPageWidth()
{
	return m_pageWidth;
}

void ScrollView::setPageWidth(sl_ui_len width)
{
	m_pageWidth = width;
	_updatePaging();
}

sl_ui_len ScrollView::getPageHeight()
{
	return m_pageHeight;
}

void ScrollView::setPageHeight(sl_ui_len height)
{
	m_pageHeight = height;
	_updatePaging();
}

void ScrollView::setScrollBarsVisible(sl_bool flagHorizontal, sl_bool flagVertical, UIUpdateMode mode)
{
	View::setScrollBarsVisible(flagHorizontal, flagVertical, mode);
	_setScrollBarsVisible_NW(flagHorizontal, flagVertical);
}

void ScrollView::_updatePaging()
{
	_setPaging_NW(m_flagPaging, m_pageWidth, m_pageHeight);
}

void ScrollView::onResize(sl_ui_len width, sl_ui_len height)
{
	if (isNativeWidget()) {
		_refreshContentSize_NW();
	}
}

void ScrollView::onResizeChild(View* child, sl_ui_len width, sl_ui_len height)
{
	if (child == m_viewContent) {
		View::setContentSize(width, height);
		if (isNativeWidget()) {
			_refreshContentSize_NW();
		}
	}
}

void ScrollView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
}

void ScrollView::onMakeLayout()
{	
}

void ScrollView::_scrollTo(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate, UIUpdateMode mode)
{
	Ref<View> view = m_viewContent;
	if (view.isNotNull()) {
		if (isNativeWidget()) {
			_scrollTo_NW(x, y, flagAnimate);
		} else {
			sl_ui_len w = getWidth();
			sl_ui_len cw = view->getWidth();
			if (cw > w) {
				sl_ui_pos rx = cw - w;
				if (x > (sl_scroll_pos)rx) {
					x = rx;
				}
				if (x < 0) {
					x = 0;
				}
			} else {
				x = 0;
			}
			sl_ui_len h = getHeight();
			sl_ui_len ch = view->getHeight();
			if (ch > h) {
				sl_ui_pos ry = ch - h;
				if (y > (sl_scroll_pos)ry) {
					y = ry;
				}
				if (y < 0) {
					y = 0;
				}
			} else {
				y = 0;
			}
			view->setPosition((sl_ui_pos)-x, (sl_ui_pos)-y, mode);
		}
	}
}

void ScrollView::_onScroll_NW(sl_scroll_pos x, sl_scroll_pos y)
{
	View::scrollTo(x, y, UIUpdateMode::NoRedraw);
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
}

void ScrollView::_refreshContentSize_NW()
{
}

void ScrollView::_setContentView_NW(const Ref<View>& view)
{
}

void ScrollView::_scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
{
}

ScrollPoint ScrollView::_getScrollPosition_NW()
{
	return ScrollPoint::zero();
}

ScrollPoint ScrollView::_getScrollRange_NW()
{
	return ScrollPoint::zero();
}

void ScrollView::_setBorder_NW(sl_bool flag)
{
}

void ScrollView::_setBackgroundColor_NW(const Color& color)
{
}

#endif


#if !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

void ScrollView::_setPaging_NW(sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight)
{
}

void ScrollView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
{
}

#endif


HorizontalScrollView::HorizontalScrollView()
{
	setHorizontalScrolling(sl_true);
	setVerticalScrolling(sl_false);
}

VerticalScrollView::VerticalScrollView()
{
	setHorizontalScrolling(sl_false);
	setVerticalScrolling(sl_true);
}

SLIB_UI_NAMESPACE_END
