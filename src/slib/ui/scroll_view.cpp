#include "../../../inc/slib/ui/scroll_view.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ScrollView, View)

ScrollView::ScrollView()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	setCreatingNativeWidget(sl_true);
	
	createScrollBars();
	setBorder(sl_true);
	setDoubleBuffering(sl_true, sl_false);

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
			addChild(view, sl_false);
			setContentSize(view->getSize(), sl_false);
		} else {
			setContentSize(0, 0, sl_false);
		}
		if (isNativeWidget()) {
			if (UI::isUiThread()) {
				_setContentView_NW_OnUiThread(view);
			} else {
				UI::dispatchToUiThread(SLIB_CALLBACK_REF(ScrollView, _setContentView_NW_OnUiThread, this, view));
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

Point ScrollView::getScrollPosition()
{
	if (isNativeWidget()) {
		Point pt = _getScrollPosition_NW();
		scrollTo(pt.x, pt.y);
		return pt;
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

void ScrollView::dispatchScroll(sl_real x, sl_real y)
{
	View::dispatchScroll(x, y);
	if (isNativeWidget()) {
		return;
	}
	_scrollTo(x, y, sl_true);
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
		setContentSize(width, height);
		if (isNativeWidget()) {
			_refreshContentSize_NW();
		}
	}
}

void ScrollView::_scrollTo(sl_real x, sl_real y, sl_bool flagRedraw)
{
	Ref<View> view = m_viewContent;
	if (view.isNotNull()) {
		if (isNativeWidget()) {
			_scrollTo_NW(x, y);
		} else {
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

SLIB_UI_NAMESPACE_END
