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

namespace slib
{

	sl_ui_len _g_priv_ui_resource_screenWidth = 0;
	sl_ui_len _g_priv_ui_resource_screenHeight = 0;
	sl_ui_len _g_priv_ui_resource_defaultScreenWidth = 0;
	sl_ui_len _g_priv_ui_resource_defaultScreenHeight = 0;
	double _g_priv_ui_resource_screenPPI = 0;
	double _g_priv_ui_resource_defaultScreenPPI = 0;

	void UIResource::updateDefaultScreenSize()
	{
		UISize size = UI::getScreenSize();
		_g_priv_ui_resource_defaultScreenWidth = size.x;
		_g_priv_ui_resource_defaultScreenHeight = size.y;
		_g_priv_ui_resource_defaultScreenPPI = UI::getScreenPPI();
	}
	
	sl_ui_len UIResource::getScreenWidth()
	{
		sl_ui_len ret = _g_priv_ui_resource_screenWidth;
		if (ret < 1) {
			if (_g_priv_ui_resource_defaultScreenWidth < 1) {
				updateDefaultScreenSize();
			}
			ret = _g_priv_ui_resource_defaultScreenWidth;
			if (ret < 1) {
				ret = 1;
			}
		}
		return ret;
	}
	
	void UIResource::setScreenWidth(sl_ui_len width)
	{
		_g_priv_ui_resource_screenWidth = width;
	}
	
	sl_ui_len UIResource::getScreenHeight()
	{
		sl_ui_len ret = _g_priv_ui_resource_screenHeight;
		if (ret < 1) {
			if (_g_priv_ui_resource_defaultScreenHeight < 1) {
				updateDefaultScreenSize();
			}
			ret = _g_priv_ui_resource_defaultScreenHeight;
			if (ret < 1) {
				ret = 1;
			}
		}
		return ret;
	}
	
	void UIResource::setScreenHeight(sl_ui_len height)
	{
		_g_priv_ui_resource_screenHeight = height;
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
		double ret = _g_priv_ui_resource_screenPPI;
		if (ret < 1) {
			if (_g_priv_ui_resource_defaultScreenPPI < 1) {
				updateDefaultScreenSize();
			}
			ret = _g_priv_ui_resource_defaultScreenPPI;
			if (ret < 1) {
				ret = 1;
			}
		}
		return ret;
	}
	
	void UIResource::setScreenPPI(double ppi)
	{
		_g_priv_ui_resource_screenPPI = ppi;
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
	
	sl_real UIResource::pixelToDp(sl_real px)
	{
		return (sl_real)(px * 160 / getScreenPPI());
	}
	
	sl_real UIResource::dpToPixel(sl_real dp)
	{
		return (sl_real)(dp * getScreenPPI() / 160);
	}
	
	
	UILayoutResource::UILayoutResource(sl_real sp)
	{
		m_sp = sp;
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
	
	SLIB_DEFINE_OBJECT(WindowLayoutResource, Window)
	
	WindowLayoutResource::WindowLayoutResource(sl_real sp)
	: UILayoutResource(sp)
	{
		SLIB_REFERABLE_CONSTRUCTOR
		Ref<View> view = Window::getContentView();
		m_contentViewRef = view;
		m_contentView = view.get();
	}
	
	WindowLayoutResource::~WindowLayoutResource()
	{
	}

	UISize WindowLayoutResource::getContentSize()
	{
		return getClientSize();
	}
	
	void WindowLayoutResource::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		_layoutViews_safe();
		Window::dispatchResize(width, height);
	}
	
	void WindowLayoutResource::_layoutViews_safe()
	{
		UISize size = getClientSize();
		UILayoutResource::_layoutViews_safe(size.x, size.y);
	}
	
	
	SLIB_DEFINE_OBJECT(ViewLayoutResource, ViewGroup)
	
	ViewLayoutResource::ViewLayoutResource(sl_real sp)
	: UILayoutResource(sp)
	{
		SLIB_REFERABLE_CONSTRUCTOR
		m_contentView = this;
	}
	
	ViewLayoutResource::~ViewLayoutResource()
	{
	}

	UISize ViewLayoutResource::getContentSize()
	{
		return getSize();
	}
	
	void ViewLayoutResource::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		_layoutViews_safe(width, height);
		ViewGroup::dispatchResize(width, height);
	}
	
	
	SLIB_DEFINE_OBJECT(PageLayoutResource, ViewPage)
	
	PageLayoutResource::PageLayoutResource(sl_real sp)
	: UILayoutResource(sp)
	{
		SLIB_REFERABLE_CONSTRUCTOR
		m_contentView = this;
	}
	
	PageLayoutResource::~PageLayoutResource()
	{
	}

	UISize PageLayoutResource::getContentSize()
	{
		return getSize();
	}
	
	void PageLayoutResource::dispatchResize(sl_ui_len width, sl_ui_len height)
	{
		_layoutViews_safe(width, height);
		ViewPage::dispatchResize(width, height);
	}

}
