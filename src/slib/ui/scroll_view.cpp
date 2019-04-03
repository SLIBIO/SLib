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

#include "slib/ui/scroll_view.h"
#include "slib/ui/core.h"

#if defined(SLIB_UI_IS_MACOS) || defined(SLIB_UI_IS_IOS) || defined(SLIB_UI_IS_WIN32) || defined(SLIB_UI_IS_ANDROID)
#	define HAS_NATIVE_WIDGET_IMPL
#endif

namespace slib
{

	SLIB_DEFINE_OBJECT(ScrollView, ViewGroup)
	
	ScrollView::ScrollView()
	{
#ifdef HAS_NATIVE_WIDGET_IMPL
		setCreatingNativeWidget(sl_true);
#endif
		setSavingCanvasState(sl_false);

		setClipping(sl_true, UIUpdateMode::Init);
		setScrolling(sl_true, sl_true, UIUpdateMode::Init);
	}
	
	ScrollView::~ScrollView()
	{
	}

	Ref<View> ScrollView::getContentView()
	{
		return m_viewContent;
	}
	
	void ScrollView::setContentView(const Ref<slib::View>& view, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		Ref<View> viewOld = m_viewContent;
		if (viewOld == view) {
			return;
		}
		UIUpdateMode uiUpdateModeNone = SLIB_UI_UPDATE_MODE_IS_INIT(mode) ? UIUpdateMode::Init : UIUpdateMode::None;
		removeChild(viewOld, uiUpdateModeNone);
		m_viewContent = view;
		if (view.isNotNull()) {
			view->setParent(this);
			view->setAttachMode(UIAttachMode::NotAttachInNativeWidget);
			if (isVerticalScrolling()) {
				view->setTopFree(UIUpdateMode::Init);
				view->setBottomFree(UIUpdateMode::Init);
			}
			if (isHorizontalScrolling()) {
				view->setLeftFree(UIUpdateMode::Init);
				view->setRightFree(UIUpdateMode::Init);
			}
			addChild(view);
			ViewGroup::setContentSize(view->getWidth(), view->getHeight(), uiUpdateModeNone);
		} else {
			ViewGroup::setContentSize(0, 0, uiUpdateModeNone);
		}
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			return;
		}
		if (isNativeWidget()) {
			if (UI::isUiThread()) {
				_setContentView_NW(view);
			} else {
				UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), ScrollView, _setContentView_NW, this, view));
			}
		} else {
			invalidate(mode);
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
			viewContent->setSize(width, height, SLIB_UI_UPDATE_MODE_IS_REDRAW(mode) ? UIUpdateMode::UpdateLayout : mode);
		}
		ViewGroup::setContentSize(_width, _height, mode);
		if (isNativeWidget()) {
			_refreshContentSize_NW();
		}
	}
	
	ScrollPoint ScrollView::getScrollPosition()
	{
		if (isNativeWidget()) {
			return _getScrollPosition_NW();
		}
		return ViewGroup::getScrollPosition();
	}
	
	ScrollPoint ScrollView::getScrollRange()
	{
		if (isNativeWidget()) {
			return _getScrollRange_NW();
		}
		return ViewGroup::getScrollRange();
	}
	
	UIRect ScrollView::getBounds()
	{
		if (isNativeWidget()) {
			return _getBounds_NW();
		}
		return ViewGroup::getBounds();
	}
	
	void ScrollView::dispatchScroll(sl_scroll_pos x, sl_scroll_pos y)
	{
		ViewGroup::dispatchScroll(x, y);
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			if (!(isNativeWidget())) {
				view->setLocation((sl_ui_pos)-x, (sl_ui_pos)-y);
			}
		}
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
			ViewGroup::setContentSize(width, height);
			if (isNativeWidget()) {
				_refreshContentSize_NW();
			}
		}
	}
	
	void ScrollView::onUpdatePaging()
	{
		if (isNativeWidget()) {
			_setPaging_NW(isPaging(), getPageWidth(), getPageHeight());
		}
	}
	
#if !defined(HAS_NATIVE_WIDGET_IMPL)
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
	
	void ScrollView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
	}
#endif
	
#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
	void ScrollView::_setPaging_NW(sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight)
	{
	}
#endif

#if !defined(SLIB_UI_IS_WIN32)
	UIRect ScrollView::_getBounds_NW()
	{
		return ViewGroup::getBounds();
	}
#endif
	
	HorizontalScrollView::HorizontalScrollView()
	{
		setScrolling(sl_true, sl_false, UIUpdateMode::Init);
	}
	
	HorizontalScrollView::~HorizontalScrollView()
	{
	}


	VerticalScrollView::VerticalScrollView()
	{
		setScrolling(sl_false, sl_true, UIUpdateMode::Init);
	}

	VerticalScrollView::~VerticalScrollView()
	{
	}

}
