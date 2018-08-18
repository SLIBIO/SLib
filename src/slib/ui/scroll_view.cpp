/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/scroll_view.h"
#include "slib/ui/core.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(ScrollView, View)
	
	ScrollView::ScrollView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setCreatingNativeWidget(sl_true);
		setCreatingChildInstances(sl_false);
		setClipping(sl_true, UIUpdateMode::Init);
		setSavingCanvasState(sl_false);
		
		setHorizontalScrolling(sl_true);
		setVerticalScrolling(sl_true);
		
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
			view->setCreatingInstance(sl_true);
			view->setAttachMode(UIAttachMode::NotAttach);
			addChild(view, uiUpdateModeNone);
			View::setContentSize(view->getWidth(), view->getHeight(), uiUpdateModeNone);
		} else {
			View::setContentSize(0, 0, uiUpdateModeNone);
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
			UIPoint pt = View::getScrollPosition();
			scrollTo(pt.x, pt.y, UIUpdateMode::None);
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
			viewContent->setSize(width, height, mode);
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
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			if (isNativeWidget()) {
				_scrollTo_NW(x, y, sl_false);
			}
		}
		View::scrollTo(x, y, mode);
	}
	
	void ScrollView::scrollTo(const ScrollPoint& position, UIUpdateMode mode)
	{
		scrollTo(position.x, position.y, mode);
	}
	
	void ScrollView::smoothScrollTo(sl_scroll_pos x, sl_scroll_pos y, UIUpdateMode mode)
	{
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			if (isNativeWidget()) {
				_scrollTo_NW(x, y, sl_true);
				return;
			}
		}
		View::smoothScrollTo(x, y, mode);
	}
	
	void ScrollView::smoothScrollTo(const ScrollPoint& position, UIUpdateMode mode)
	{
		smoothScrollTo(position.x, position.y, mode);
	}
	
	void ScrollView::setScrollBarsVisible(sl_bool flagHorizontal, sl_bool flagVertical, UIUpdateMode mode)
	{
		View::setScrollBarsVisible(flagHorizontal, flagVertical, mode);
		_setScrollBarsVisible_NW(flagHorizontal, flagVertical);
	}
	
	void ScrollView::dispatchScroll(sl_scroll_pos x, sl_scroll_pos y)
	{
		View::dispatchScroll(x, y);
		Ref<View> view = m_viewContent;
		if (view.isNotNull()) {
			if (!(isNativeWidget())) {
				view->setPosition((sl_ui_pos)-x, (sl_ui_pos)-y);
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
			View::setContentSize(width, height);
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
	
	void ScrollView::_onScroll_NW(sl_scroll_pos x, sl_scroll_pos y)
	{
		if (isNativeWidget()) {
			View::scrollTo(x, y, UIUpdateMode::None);
		}
	}
	
	
#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_WIN32) && !defined(SLIB_UI_IS_ANDROID)	
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
	
#if !defined(SLIB_UI_IS_IOS) && !defined(SLIB_UI_IS_ANDROID)
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
	
	HorizontalScrollView::~HorizontalScrollView()
	{
	}


	VerticalScrollView::VerticalScrollView()
	{
		setHorizontalScrolling(sl_false);
		setVerticalScrolling(sl_true);
	}

	VerticalScrollView::~VerticalScrollView()
	{
	}

}
