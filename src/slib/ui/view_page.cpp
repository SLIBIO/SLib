#include "../../../inc/slib/ui/view_page.h"

SLIB_UI_NAMESPACE_BEGIN

void IViewPagerListener::onPushPage(ViewPager* navigation, View* pageIn)
{
}

void IViewPagerListener::onPopPage(ViewPager* navigation, View* pageOut)
{
}

void IViewPagerListener::onResumePage(ViewPager* navigation, View* page)
{
}

void IViewPagerListener::onPausePage(ViewPager* navigation, View* page)
{
}

void IViewPagerListener::onReadyPage(ViewPager* navigation, View* page)
{
}


ViewPageMoveParam::ViewPageMoveParam()
{
}

ViewPagePushParam::ViewPagePushParam()
{
}

ViewPagePopParam::ViewPagePopParam()
{
}

SLIB_DEFINE_OBJECT(ViewPager, ViewGroup)

ViewPager::ViewPager()
{
	SLIB_REFERABLE_CONSTRUCTOR
	
	m_indexCurrent = 0;
}

sl_size ViewPager::getPagesCount()
{
	return m_pages.getCount();
}

sl_size ViewPager::getCurrentPageIndex()
{
	return m_indexCurrent;
}

Ref<View> ViewPager::getCurrentPage()
{
	ObjectLocker lock(this);
	return m_pages.getItemValue_NoLock(m_indexCurrent, Ref<View>::null());
}

void ViewPager::push(const Ref<View>& view, const ViewPagePushParam& param)
{
	ObjectLocker lock(this);
}

void ViewPager::push(const Ref<View>& view)
{
	ViewPagePushParam param;
	push(view, param);
}

void ViewPager::pop(const ViewPagePopParam& param)
{
	ObjectLocker lock(this);
}

void ViewPager::pop()
{
	ViewPagePopParam param;
	pop(param);
}

void ViewPager::moveNext(const ViewPageMoveParam& param)
{
	ObjectLocker lock(this);
}

void ViewPager::moveNext()
{
	ViewPageMoveParam param;
	moveNext(param);
}

void ViewPager::movePrevious(const ViewPageMoveParam& param)
{
	ObjectLocker lock(this);
}

void ViewPager::movePrevious()
{
	ViewPageMoveParam param;
	movePrevious(param);
}

void ViewPager::moveTo(sl_size index)
{
	ObjectLocker lock(this);
}

void ViewPager::onPushPage(View* pageIn)
{
}

void ViewPager::onPopPage(View* pageOut)
{
}

void ViewPager::onResumePage(View* page)
{
}

void ViewPager::onPausePage(View* page)
{
}

void ViewPager::onReadyPage(View* page)
{
}

void ViewPager::dispatchPushPage(View* pageIn)
{
	if (pageIn) {
		onPushPage(pageIn);
		PtrLocker<IViewPagerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onPushPage(this, pageIn);
		}
		if (ViewPage::checkInstance(pageIn)) {
			((ViewPage*)pageIn)->dispatchPush(this);
		}
	}
}

void ViewPager::dispatchPopPage(View* pageOut)
{
	if (pageOut) {
		onPopPage(pageOut);
		PtrLocker<IViewPagerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onPopPage(this, pageOut);
		}
		if (ViewPage::checkInstance(pageOut)) {
			((ViewPage*)pageOut)->dispatchPop(this);
		}
	}
}

void ViewPager::dispatchResumePage(View* page)
{
	if (page) {
		onResumePage(page);
		PtrLocker<IViewPagerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onResumePage(this, page);
		}
		if (ViewPage::checkInstance(page)) {
			((ViewPage*)page)->dispatchResume(this);
		}
	}
}

void ViewPager::dispatchPausePage(View* page)
{
	if (page) {
		onPausePage(page);
		PtrLocker<IViewPagerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onPausePage(this, page);
		}
		if (ViewPage::checkInstance(page)) {
			((ViewPage*)page)->dispatchPause(this);
		}
	}
}

void ViewPager::dispatchReadyPage(View* page)
{
	if (page) {
		onReadyPage(page);
		PtrLocker<IViewPagerListener> listener(getListener());
		if (listener.isNotNull()) {
			listener->onReadyPage(this, page);
		}
		if (ViewPage::checkInstance(page)) {
			((ViewPage*)page)->dispatchReady(this);
		}
	}
}


SLIB_DEFINE_OBJECT(ViewPage, ViewGroup)

ViewPage::ViewPage()
{
	SLIB_REFERABLE_CONSTRUCTOR
}

void ViewPage::onPush(ViewPager* pager)
{
}

void ViewPage::onPop(ViewPager* pager)
{
}

void ViewPage::onResume(ViewPager* pager)
{
}

void ViewPage::onPause(ViewPager* pager)
{
}

void ViewPage::onReady(ViewPager* pager)
{
}

void ViewPage::dispatchPush(ViewPager* pager)
{
	onPush(pager);
	onResume(pager);
}

void ViewPage::dispatchPop(ViewPager* pager)
{
	onPause(pager);
	onPop(pager);
}

void ViewPage::dispatchResume(ViewPager* pager)
{
	onResume(pager);
}

void ViewPage::dispatchPause(ViewPager* pager)
{
	onPause(pager);
}

void ViewPage::dispatchReady(ViewPager* pager)
{
	onReady(pager);
}

SLIB_UI_NAMESPACE_END
