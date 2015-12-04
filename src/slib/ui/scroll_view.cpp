#include "../../../inc/slib/ui/scroll_view.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN
ScrollView::ScrollView()
{
	m_backgroundColor = Color::zero();
	m_flagBorder = sl_true;
}

Ref<View> ScrollView::getContentView()
{
	return m_viewContent;
}

void ScrollView::setContentView(const Ref<slib::View>& _view)
{
	MutexLocker lock(getLocker());
	Ref<View> view = _view;
	if (m_viewContent != view) {
		Ref<View> viewOld = m_viewContent;
		if (viewOld.isNotNull()) {
			viewOld->removeParent(this);
			viewOld->detach();
		}
		m_viewContent = view;
		if (view.isNotNull()) {
			view->setParent(this);
		}
		if (UI::isUIThread()) {
			_setContentView(view);
		} else {
			UI::runOnUIThread(SLIB_CALLBACK_REF(ScrollView, _setContentView_Safe, this, view));
		}
	}
}

void ScrollView::_setContentView_Safe(const Ref<View> view)
{
	_setContentView(view);
}

void ScrollView::scrollTo(const Point& pt)
{
	scrollTo(pt.x, pt.y);
}


sl_bool ScrollView::isBorder()
{
	return m_flagBorder;
}

void ScrollView::setBorder(sl_bool flag)
{
	m_flagBorder = flag;
	_setBorder(flag);
}

Color ScrollView::getBackgroundColor()
{
	return m_backgroundColor;
}

void ScrollView::setBackgroundColor(const Color& color)
{
	m_backgroundColor = color;
	_setBackgroundColor(color);
}
SLIB_UI_NAMESPACE_END
