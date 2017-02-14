#include "../../../inc/slib/ui/resource.h"
#include "../../../inc/slib/ui/core.h"

namespace slib
{

	sl_ui_len _g_ui_resource_screenWidth = 0;
	sl_ui_len _g_ui_resource_screenHeight = 0;
	sl_ui_len _g_ui_resource_defaultScreenWidth = 0;
	sl_ui_len _g_ui_resource_defaultScreenHeight = 0;
	
	sl_ui_len UIResource::getScreenWidth()
	{
		if (_g_ui_resource_screenWidth == 0) {
			if (_g_ui_resource_defaultScreenWidth == 0) {
				updateDefaultScreenSize();
			}
			return _g_ui_resource_defaultScreenWidth;
		}
		return _g_ui_resource_screenWidth;
	}
	
	void UIResource::setScreenWidth(sl_ui_len width)
	{
		_g_ui_resource_screenWidth = width;
	}
	
	sl_ui_len UIResource::getScreenHeight()
	{
		if (_g_ui_resource_screenHeight == 0) {
			if (_g_ui_resource_defaultScreenHeight == 0) {
				updateDefaultScreenSize();
			}
			return _g_ui_resource_defaultScreenHeight;
		}
		return _g_ui_resource_screenHeight;
	}
	
	void UIResource::setScreenHeight(sl_ui_len height)
	{
		_g_ui_resource_screenHeight = height;
	}
	
	sl_ui_len UIResource::getScreenMinimum()
	{
		return SLIB_MIN(getScreenWidth(), getScreenHeight());
	}
	
	sl_ui_len UIResource::getScreenMaximum()
	{
		return SLIB_MAX(getScreenWidth(), getScreenHeight());
	}
	
	void UIResource::updateDefaultScreenSize()
	{
		UISize size = UI::getScreenSize();
		_g_ui_resource_defaultScreenWidth = size.x;
		_g_ui_resource_defaultScreenHeight = size.y;
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
		Window::dispatchResize(width, height);
		_layoutViews_safe();
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
