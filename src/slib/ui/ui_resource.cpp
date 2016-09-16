#include "../../../inc/slib/ui/resource.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

sl_ui_len _g_ui_resource_screenWidth = 0;
sl_ui_len _g_ui_resource_screenHeight = 0;

sl_ui_len UIResource::getScreenWidth()
{
	if (_g_ui_resource_screenWidth == 0) {
		UISize size = UI::getScreenSize();
		_g_ui_resource_screenWidth = size.x;
		if (_g_ui_resource_screenHeight == 0) {
			_g_ui_resource_screenHeight = size.y;
		}
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
		UISize size = UI::getScreenSize();
		if (_g_ui_resource_screenWidth == 0) {
			_g_ui_resource_screenWidth = size.x;
		}
		_g_ui_resource_screenHeight = size.y;
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

UILayoutResource::UILayoutResource(sl_real sp)
{
	m_sp = sp;
	m_countRecursiveLayout = 0;
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
	sl_int32 n = Base::interlockedIncrement32(&m_countRecursiveLayout);
	if (n == 1) {
		layoutViews(width, height);
		m_contentView->invalidate();
	}
	Base::interlockedDecrement32(&m_countRecursiveLayout);
}


SLIB_DEFINE_OBJECT(WindowLayoutResource, Window)

WindowLayoutResource::WindowLayoutResource(sl_real sp)
: UILayoutResource(sp)
{
	SLIB_REFERABLE_CONSTRUCTOR
	Ref<View> view = Window::getContentView();
	m_contentViewRef = view;
	m_contentView = view.ptr;
}

void WindowLayoutResource::dispatchResize(UISize& size)
{
	Window::dispatchResize(size);
	UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(WindowLayoutResource, _layoutViews_safe, this));
}

void WindowLayoutResource::dispatchMaximize()
{
	Window::dispatchMaximize();
	UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(WindowLayoutResource, _layoutViews_safe, this));
}

void WindowLayoutResource::dispatchMinimize()
{
	Window::dispatchMinimize();
	UI::dispatchToUiThread(SLIB_CALLBACK_WEAKREF(WindowLayoutResource, _layoutViews_safe, this));
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

void ViewLayoutResource::dispatchResize(sl_ui_len width, sl_ui_len height)
{
	_layoutViews_safe(width, height);
	ViewGroup::dispatchResize(width, height);
}

SLIB_UI_NAMESPACE_END
