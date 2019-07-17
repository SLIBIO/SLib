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

#include "slib/ui/resource.h"

#include "slib/ui/core.h"
#include "slib/graphics/resource.h"

namespace slib
{

	namespace priv
	{
		namespace ui_resource
		{

			sl_ui_len g_screenWidth = 0;
			sl_ui_len g_screenHeight = 0;
			sl_ui_len g_defaultScreenWidth = 0;
			sl_ui_len g_defaultScreenHeight = 0;
			double g_screenPPI = 0;
			double g_defaultScreenPPI = 0;
			sl_ui_len g_statusBarHeight = -1;
			sl_ui_len g_defaultStatusBarHeight = -1;
			sl_ui_len g_safeAreaLeft = -1;
			sl_ui_len g_safeAreaTop = -1;
			sl_ui_len g_safeAreaRight = -1;
			sl_ui_len g_safeAreaBottom = -1;
			sl_ui_len g_defaultSafeAreaLeft = -1;
			sl_ui_len g_defaultSafeAreaTop = -1;
			sl_ui_len g_defaultSafeAreaRight = -1;
			sl_ui_len g_defaultSafeAreaBottom = -1;

		}
	}

	using namespace priv::ui_resource;

	void UIResource::updateDefaultScreenSize()
	{
		UISize size = UI::getScreenSize();
		g_defaultScreenWidth = size.x;
		g_defaultScreenHeight = size.y;
		g_defaultScreenPPI = UI::getScreenPPI();
		g_defaultStatusBarHeight = UI::getStatusBarHeight();
		UIEdgeInsets insets = UI::getSafeAreaInsets();
		g_defaultSafeAreaLeft = insets.left;
		g_defaultSafeAreaTop = insets.top;
		g_defaultSafeAreaRight = insets.right;
		g_defaultSafeAreaBottom = insets.bottom;
		GraphicsResource::setScreenWidth(size.x);
		GraphicsResource::setScreenHeight(size.y);
	}
	
	sl_ui_len UIResource::getScreenWidth()
	{
		sl_ui_len ret = g_screenWidth;
		if (ret < 1) {
			if (g_defaultScreenWidth < 1) {
				updateDefaultScreenSize();
			}
			ret = g_defaultScreenWidth;
			if (ret < 1) {
				ret = 1;
			}
		}
		return ret;
	}
	
	void UIResource::setScreenWidth(sl_ui_len width)
	{
		g_screenWidth = width;
	}
	
	sl_ui_len UIResource::getScreenHeight()
	{
		sl_ui_len ret = g_screenHeight;
		if (ret < 1) {
			if (g_defaultScreenHeight < 1) {
				updateDefaultScreenSize();
			}
			ret = g_defaultScreenHeight;
			if (ret < 1) {
				ret = 1;
			}
		}
		return ret;
	}
	
	void UIResource::setScreenHeight(sl_ui_len height)
	{
		g_screenHeight = height;
	}
	
	sl_ui_len UIResource::getScreenMinimum()
	{
		return SLIB_MIN(getScreenWidth(), getScreenHeight());
	}
	
	sl_ui_len UIResource::getScreenMaximum()
	{
		return SLIB_MAX(getScreenWidth(), getScreenHeight());
	}
	
	double UIResource::getScreenPPI()
	{
		double ret = g_screenPPI;
		if (ret < 1) {
			if (g_defaultScreenPPI < 1) {
				updateDefaultScreenSize();
			}
			ret = g_defaultScreenPPI;
			if (ret < 1) {
				ret = 1;
			}
		}
		return ret;
	}
	
	void UIResource::setScreenPPI(double ppi)
	{
		g_screenPPI = ppi;
	}
	
	sl_ui_len UIResource::getStatusBarHeight()
	{
		sl_ui_len ret = g_statusBarHeight;
		if (ret < 0) {
			if (g_defaultStatusBarHeight < 0) {
				updateDefaultScreenSize();
			}
			ret = g_defaultStatusBarHeight;
			if (ret < 0) {
				ret = 0;
			}
		}
		return ret;
	}
	
	void UIResource::setStatusBarHeight(sl_ui_len height)
	{
		g_statusBarHeight = height;
	}
	
	sl_ui_len UIResource::getSafeAreaInsetLeft()
	{
		sl_ui_len ret = g_safeAreaLeft;
		if (ret < 0) {
			if (g_defaultSafeAreaLeft < 0) {
				updateDefaultScreenSize();
			}
			ret = g_defaultSafeAreaLeft;
			if (ret < 0) {
				ret = 0;
			}
		}
		return ret;
	}
	
	void UIResource::setSafeAreaInsetLeft(sl_ui_len left)
	{
		g_safeAreaLeft = left;
	}
	
	sl_ui_len UIResource::getSafeAreaInsetTop()
	{
		sl_ui_len ret = g_safeAreaTop;
		if (ret < 0) {
			if (g_defaultSafeAreaTop < 0) {
				updateDefaultScreenSize();
			}
			ret = g_defaultSafeAreaTop;
			if (ret < 0) {
				ret = 0;
			}
		}
		return ret;
	}
	
	void UIResource::setSafeAreaInsetTop(sl_ui_len top)
	{
		g_safeAreaTop = top;
	}
	
	sl_ui_len UIResource::getSafeAreaInsetRight()
	{
		sl_ui_len ret = g_safeAreaRight;
		if (ret < 0) {
			if (g_defaultSafeAreaRight < 0) {
				updateDefaultScreenSize();
			}
			ret = g_defaultSafeAreaRight;
			if (ret < 0) {
				ret = 0;
			}
		}
		return ret;
	}
	
	void UIResource::setSafeAreaInsetRight(sl_ui_len right)
	{
		g_safeAreaRight = right;
	}
	
	sl_ui_len UIResource::getSafeAreaInsetBottom()
	{
		sl_ui_len ret = g_safeAreaBottom;
		if (ret < 0) {
			if (g_defaultSafeAreaBottom < 0) {
				updateDefaultScreenSize();
			}
			ret = g_defaultSafeAreaBottom;
			if (ret < 0) {
				ret = 0;
			}
		}
		return ret;
	}
	
	void UIResource::setSafeAreaInsetBottom(sl_ui_len bottom)
	{
		g_safeAreaBottom = bottom;
	}
	
	sl_ui_len UIResource::getSafeAreaWidth()
	{
		sl_ui_len w = getScreenWidth() - getSafeAreaInsetLeft() - getSafeAreaInsetRight();
		if (w < 0) {
			w = 0;
		}
		return w;
	}
	
	sl_ui_len UIResource::getSafeAreaHeight()
	{
		sl_ui_len h = getScreenHeight() - getSafeAreaInsetTop() - getSafeAreaInsetBottom();
		if (h < 0) {
			h = 0;
		}
		return h;
	}
	
	sl_real UIResource::pixelToInch(sl_real px)
	{
		return (sl_real)(px / getScreenPPI());
	}
	
	sl_real UIResource::inchToPixel(sl_real inch)
	{
		return (sl_real)(inch * getScreenPPI());
	}
	
	sl_real UIResource::pixelToMeter(sl_real px)
	{
		return (sl_real)(px * 0.0254 / getScreenPPI());
	}
	
	sl_real UIResource::meterToPixel(sl_real meters)
	{
		return (sl_real)(meters * getScreenPPI() * 39.3701);
	}

	sl_real UIResource::pixelToCentimeter(sl_real px)
	{
		return (sl_real)(px * 2.54 / getScreenPPI());
	}
	
	sl_real UIResource::centimeterToPixel(sl_real cm)
	{
		return (sl_real)(cm * getScreenPPI() * 0.393701);
	}
	
	sl_real UIResource::pixelToMillimeter(sl_real px)
	{
		return (sl_real)(px * 25.4 / getScreenPPI());
	}
	
	sl_real UIResource::millimeterToPixel(sl_real mm)
	{
		return (sl_real)(mm * getScreenPPI() * 0.0393701);
	}
	
	sl_real UIResource::pixelToPoint(sl_real px)
	{
		return (sl_real)(px * 72 / getScreenPPI());
	}
	
	sl_real UIResource::pointToPixel(sl_real pt)
	{
		return (sl_real)(pt * getScreenPPI() / 72);
	}
	
	sl_real UIResource::pixelToPicas(sl_real px)
	{
		return (sl_real)(px * 12 / getScreenPPI());
	}
	
	sl_real UIResource::picasToPixel(sl_real pc)
	{
		return (sl_real)(pc * getScreenPPI() / 12);
	}
	
	sl_real UIResource::pixelToDp(sl_real px)
	{
		return (sl_real)(px * 160 / getScreenPPI());
	}
	
	sl_real UIResource::dpToPixel(sl_real dp)
	{
		return (sl_real)(dp * getScreenPPI() / 160);
	}
	
	sl_ui_pos UIResource::toUiPos(sl_real f)
	{
		if (Math::isAlmostZero(f)) {
			return 0;
		}
		if (f > 0 && f < 1) {
			return 1;
		}
		if (f < 0 && f > -1) {
			return -1;
		}
		return (sl_ui_pos)f;
	}
	
	UILayoutResource::UILayoutResource()
	{
		m_sp = 1;
		m_countRecursiveLayout = 0;
		m_flagInitialized = sl_false;
		m_contentView = sl_null;
	}
	
	UILayoutResource::~UILayoutResource()
	{
	}

	Ref<View> UILayoutResource::getContent()
	{
		return m_contentView;
	}
	
	sl_real UILayoutResource::getScaledPixel()
	{
		return m_sp;
	}
	
	void UILayoutResource::setScaledPixel(sl_real sp)
	{
		m_sp = sp;
	}
	
	// avoid recursively layouting
	void UILayoutResource::_layoutViews_safe(sl_ui_len width, sl_ui_len height)
	{
		if (width > 0 && height > 0) {
			if (!m_flagInitialized) {
				return;
			}
			sl_int32 n = Base::interlockedIncrement32(&m_countRecursiveLayout);
			if (n == 1) {
				layoutViews(width, height);
				m_contentView->invalidate();
			}
			Base::interlockedDecrement32(&m_countRecursiveLayout);
		}
	}
	
	sl_bool UILayoutResource::isInitialized()
	{
		return m_flagInitialized;
	}
	
	void UILayoutResource::setInitialized()
	{
		m_flagInitialized = sl_true;
	}
	
	SLIB_DEFINE_OBJECT(WindowLayout, Window)
	
	WindowLayout::WindowLayout()
	{
	}
	
	WindowLayout::~WindowLayout()
	{
	}

	void WindowLayout::init()
	{
		Window::init();

		Ref<View> view = Window::getContentView();
		m_contentViewRef = view;
		m_contentView = view.get();
	}

	UISize WindowLayout::getContentSize()
	{
		return getClientSize();
	}
	
	void WindowLayout::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		_layoutViews_safe();
		Window::dispatchResize(width, height);
	}
	
	void WindowLayout::_layoutViews_safe()
	{
		UISize size = getClientSize();
		UILayoutResource::_layoutViews_safe(size.x, size.y);
	}
	
	
	SLIB_DEFINE_OBJECT(ViewLayout, ViewGroup)
	
	ViewLayout::ViewLayout()
	{
	}
	
	ViewLayout::~ViewLayout()
	{
	}

	void ViewLayout::init()
	{
		ViewGroup::init();

		m_contentView = this;
	}

	UISize ViewLayout::getContentSize()
	{
		return getSize();
	}
	
	void ViewLayout::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		_layoutViews_safe(width, height);
		ViewGroup::dispatchResize(width, height);
	}
	
	
	SLIB_DEFINE_OBJECT(PageLayout, ViewPage)
	
	PageLayout::PageLayout()
	{
	}
	
	PageLayout::~PageLayout()
	{
	}

	void PageLayout::init()
	{
		ViewPage::init();

		m_contentView = this;
	}

	UISize PageLayout::getContentSize()
	{
		return getSize();
	}
	
	void PageLayout::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		_layoutViews_safe(width, height);
		ViewPage::dispatchResize(width, height);
	}

}
