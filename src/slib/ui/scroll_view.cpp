#include "../../../inc/slib/ui/scroll_view.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ScrollView, View)

ScrollView::ScrollView()
{
	SLIB_REFERABLE_CONSTRUCTOR
	_init(sl_true, sl_true);
}

ScrollView::ScrollView(sl_bool flagVertical)
{
	SLIB_REFERABLE_CONSTRUCTOR
	_init(sl_false, flagVertical);
}

Ref<View> ScrollView::getContentView()
{
	return m_viewContent;
}

void ScrollView::setContentView(const Ref<slib::View>& view, sl_bool flagRedraw)
{
	ObjectLocker lock(this);
	if (m_viewContent != view) {
		Ref<View> viewOld = m_viewContent;
		removeChild(viewOld);
		m_viewContent = view;
		if (view.isNotNull()) {
			view->setParent(this);
			view->setCreatingInstance(sl_true);
			addChildNotAttach(view, sl_false);
			View::setContentSize(view->getWidth(), view->getHeight());
		} else {
			View::setContentSize(0, 0);
		}
		if (isNativeWidget()) {
			if (UI::isUiThread()) {
				_setContentView_NW_OnUiThread(view);
			} else {
				UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(ScrollView, _setContentView_NW_OnUiThread, this, view));
			}
		} else {
			Point pt = View::getScrollPosition();
			_scrollTo(pt.x, pt.y, sl_false);
			if (flagRedraw) {
				invalidate();
			}
		}
	}
}

void ScrollView::setContentSize(sl_real width, sl_real height, sl_bool flagRefresh)
{
	Ref<View> viewContent = m_viewContent;
	if (viewContent.isNotNull()) {
		viewContent->setSize(width, height);
	}
	View::setContentSize(width, height, flagRefresh);
	if (isNativeWidget()) {
		_refreshContentSize_NW();
	}
}

void ScrollView::setContentSize(const Size& size, sl_bool flagRefresh)
{
	setContentSize(size.x, size.y, flagRefresh);
}

Point ScrollView::getScrollPosition()
{
	if (isNativeWidget()) {
		return _getScrollPosition_NW();
	}
	return View::getScrollPosition();
}

Size ScrollView::getScrollRange()
{
	if (isNativeWidget()) {
		return _getScrollRange_NW();
	}
	return View::getScrollRange();
}

void ScrollView::scrollTo(sl_real x, sl_real y, sl_bool flagRedraw)
{
	_scrollTo(x, y, sl_false);
	View::scrollTo(x, y, flagRedraw);
}

void ScrollView::scrollTo(const Point& position, sl_bool flagRedraw)
{
	scrollTo(position.x, position.y, flagRedraw);
}

void ScrollView::onResize(sl_real width, sl_real height)
{
	if (isNativeWidget()) {
		_refreshContentSize_NW();
	}
}

void ScrollView::onResizeChild(View* child, sl_real width, sl_real height)
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

void ScrollView::_init(sl_bool flagBothScroll, sl_bool flagVertical)
{
	setCreatingNativeWidget(sl_true);
	setCreatingChildInstances(sl_false);
	
	m_flagBothScroll = flagBothScroll;
	m_flagVerticalScroll = flagVertical;
	
	if (flagBothScroll) {
		createScrollBars(sl_false);
	} else {
		if (flagVertical) {
			createVerticalScrollBar(sl_false);
		} else {
			createHorizontalScrollBar(sl_false);
		}
	}
	
	setBorder(sl_true, sl_false);

}

void ScrollView::_scrollTo(sl_real x, sl_real y, sl_bool flagRedraw)
{
	Ref<View> view = m_viewContent;
	if (view.isNotNull()) {
		if (isNativeWidget()) {
			_scrollTo_NW(x, y);
		} else {
			sl_real rx = view->getWidth() - getWidth();
			if (x > rx) {
				x = rx;
			}
			if (x < 0) {
				x = 0;
			}
			sl_real ry = view->getHeight() - getHeight();
			if (y > ry) {
				y = ry;
			}
			if (y < 0) {
				y = 0;
			}
			view->setPosition(-x, -y, flagRedraw);
		}
	}
}

void ScrollView::_setContentView_NW_OnUiThread(const Ref<View> view)
{
	_setContentView_NW(view);
	if (view.isNotNull()) {
		Point pt = View::getScrollPosition();
		_scrollTo_NW(pt.x, pt.y);
	}
}

void ScrollView::_onScroll_NW(sl_real x, sl_real y)
{
	View::scrollTo(x, y, sl_false);
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void ScrollView::_refreshContentSize_NW()
{
}

void ScrollView::_setContentView_NW(const Ref<View>& view)
{
}

void ScrollView::_scrollTo_NW(sl_real x, sl_real y)
{
}

Point ScrollView::_getScrollPosition_NW()
{
	return Point::zero();
}

Size ScrollView::_getScrollRange_NW()
{
	return Size::zero();
}

void ScrollView::_setBorder_NW(sl_bool flag)
{
}

void ScrollView::_setBackgroundColor_NW(const Color& color)
{
}

#endif


HorizontalScrollView::HorizontalScrollView() : ScrollView(sl_false)
{
}

VerticalScrollView::VerticalScrollView() : ScrollView(sl_true)
{
}

SLIB_UI_NAMESPACE_END
