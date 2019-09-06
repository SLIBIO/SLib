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
		Ptr<IScrollViewInstance> instance = getScrollViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&ScrollView::setContentView, view, mode);
		}
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
			addChild(view, uiUpdateModeNone);
			ViewGroup::setContentSize(view->getWidth(), view->getHeight(), uiUpdateModeNone);
		} else {
			ViewGroup::setContentSize(0, 0, uiUpdateModeNone);
		}
		if (SLIB_UI_UPDATE_MODE_IS_INIT(mode)) {
			return;
		}
		if (instance.isNotNull()) {
			instance->setContentView(this, view);
		} else {
			invalidate(mode);
		}
	}
	
	void ScrollView::setContentSize(sl_scroll_pos _width, sl_scroll_pos _height, UIUpdateMode mode)
	{
		Ptr<IScrollViewInstance> instance = getScrollViewInstance();
		if (instance.isNotNull()) {
			void (ScrollView::*func)(sl_scroll_pos, sl_scroll_pos, UIUpdateMode) = &ScrollView::setContentSize;
			SLIB_VIEW_RUN_ON_UI_THREAD(func, _width, _height, mode)
		}
		_initializeScrollAttributes();
		Ref<ScrollAttributes>& attrs = m_scrollAttrs;
		if (attrs.isNull()) {
			return;
		}
		sl_ui_pos width = (sl_ui_pos)(_width);
		if (width < 0) {
			width = 0;
		}
		sl_ui_pos height = (sl_ui_pos)(_height);
		if (height < 0) {
			height = 0;
		}
		if (Math::isAlmostZero(width - attrs->contentWidth) && Math::isAlmostZero(height - attrs->contentHeight)) {
			attrs->contentWidth = width;
			attrs->contentHeight = height;
			return;
		}
		ObjectLocker lock(this);
		Ref<View> viewContent = m_viewContent;
		if (viewContent.isNotNull()) {
			viewContent->setSize(width, height, mode);
		}
		ViewGroup::setContentSize(_width, _height, mode);
		if (instance.isNotNull()) {
			instance->refreshContentSize(this);
		}
	}
	
	ScrollPoint ScrollView::getScrollPosition()
	{
		Ptr<IScrollViewInstance> instance = getScrollViewInstance();
		if (instance.isNotNull()) {
			ScrollPoint pt;
			if (instance->getScrollPosition(this, pt)) {
				return pt;
			}
		}
		return ViewGroup::getScrollPosition();
	}
	
	ScrollPoint ScrollView::getScrollRange()
	{
		Ptr<IScrollViewInstance> instance = getScrollViewInstance();
		if (instance.isNotNull()) {
			ScrollPoint pt;
			if (instance->getScrollRange(this, pt)) {
				return pt;
			}
		}
		return ViewGroup::getScrollRange();
	}
	
	UIRect ScrollView::getBounds()
	{
		Ptr<IScrollViewInstance> instance = getScrollViewInstance();
		if (instance.isNotNull()) {
			UIRect ret;
			if (instance->getBounds(this, ret)) {
				return ret;
			}
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
		_refreshSize();
	}
	
	void ScrollView::onResizeChild(View* child, sl_ui_len width, sl_ui_len height)
	{
		if (child == m_viewContent) {
			ViewGroup::setContentSize(width, height);
			_refreshSize();
		}
	}
	
	void ScrollView::_refreshSize()
	{
		Ptr<IScrollViewInstance> instance = getScrollViewInstance();
		if (instance.isNotNull()) {
			SLIB_VIEW_RUN_ON_UI_THREAD(&ScrollView::_refreshSize)
			instance->refreshContentSize(this);
		}
	}
	
#if !defined(HAS_NATIVE_WIDGET_IMPL)
	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
	}
	
	Ptr<IScrollViewInstance> ScrollView::getScrollViewInstance()
	{
		return sl_null;
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

	
	sl_bool IScrollViewInstance::getBounds(ScrollView* view, UIRect& _out)
	{
		return sl_false;
	}
	
}
