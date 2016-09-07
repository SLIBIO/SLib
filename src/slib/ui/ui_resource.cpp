#include "../../../inc/slib/ui/resource.h"
#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

sl_real _g_ui_resource_screenWidth = 0;
sl_real _g_ui_resource_screenHeight = 0;

sl_real UIResource::getScreenWidth()
{
	if (_g_ui_resource_screenWidth == 0) {
		Size size = UI::getScreenSize();
		_g_ui_resource_screenWidth = size.x;
		if (_g_ui_resource_screenHeight == 0) {
			_g_ui_resource_screenHeight = size.y;
		}
	}
	return _g_ui_resource_screenWidth;
}

void UIResource::setScreenWidth(sl_real width)
{
	_g_ui_resource_screenWidth = width;
}

sl_real UIResource::getScreenHeight()
{
	if (_g_ui_resource_screenHeight == 0) {
		Size size = UI::getScreenSize();
		if (_g_ui_resource_screenWidth == 0) {
			_g_ui_resource_screenWidth = size.x;
		}
		_g_ui_resource_screenHeight = size.y;
	}
	return _g_ui_resource_screenHeight;
}

void UIResource::setScreenHeight(sl_real height)
{
	_g_ui_resource_screenHeight = height;
}

sl_real UIResource::getScreenMinimum()
{
	return SLIB_MIN(getScreenWidth(), getScreenHeight());
}

sl_real UIResource::getScreenMaximum()
{
	return SLIB_MAX(getScreenWidth(), getScreenHeight());
}

UILayoutResource::UILayoutResource(sl_real customUnitLength)
{
	m_customUnit = customUnitLength;
	m_countRecursiveLayout = 0;
}

Ref<View> UILayoutResource::getContent()
{
	return m_contentView;
}

sl_real UILayoutResource::getCustomUnitLength()
{
	return m_customUnit;
}

void UILayoutResource::setCustomUnitLength(sl_real length)
{
	m_customUnit = length;
}

// avoid recursively layouting
void UILayoutResource::_layoutViews_safe(sl_real width, sl_real height)
{
	sl_int32 n = Base::interlockedIncrement32(&m_countRecursiveLayout);
	if (n == 1) {
		layoutViews(width, height);
	}
	Base::interlockedDecrement32(&m_countRecursiveLayout);
}


SLIB_DEFINE_OBJECT(WindowLayoutResource, Window)

WindowLayoutResource::WindowLayoutResource(sl_real customUnitLength)
: UILayoutResource(customUnitLength)
{
	SLIB_REFERABLE_CONSTRUCTOR
	Ref<View> view = Window::getContentView();
	m_contentViewRef = view;
	m_contentView = view.ptr;
}

void WindowLayoutResource::dispatchResize(Size& size)
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
	Size size = getClientSize();
	UILayoutResource::_layoutViews_safe(size.x, size.y);
}


SLIB_DEFINE_OBJECT(ViewLayoutResource, ViewGroup)

ViewLayoutResource::ViewLayoutResource(sl_real customUnitLength)
: UILayoutResource(customUnitLength)
{
	SLIB_REFERABLE_CONSTRUCTOR
	m_contentView = this;
}

void ViewLayoutResource::dispatchResize(sl_real width, sl_real height)
{
	_layoutViews_safe(width, height);
	ViewGroup::dispatchResize(width, height);
}

SLIB_UI_NAMESPACE_END
