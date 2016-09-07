#ifndef CHECKHEADER_SLIB_UI_RESOURCE
#define CHECKHEADER_SLIB_UI_RESOURCE

#include "definition.h"

#include "../core/resource.h"

#include "menu.h"
#include "event.h"
#include "view.h"
#include "window.h"

#include "button.h"
#include "label_view.h"
#include "check_box.h"
#include "radio_button.h"
#include "edit_view.h"
#include "image_view.h"
#include "select_view.h"
#include "scroll_view.h"
#include "linear_view.h"
#include "list_view.h"
#include "list_report_view.h"
#include "render_view.h"
#include "tab_view.h"
#include "tree_view.h"
#include "web_view.h"
#include "video_view.h"
#include "camera_view.h"

SLIB_UI_NAMESPACE_BEGIN

class UIResource
{
public:
	static sl_real getScreenWidth();
	
	static void setScreenWidth(sl_real width);
	
	static sl_real getScreenHeight();
	
	static void setScreenHeight(sl_real height);
	
	static sl_real getScreenMinimum();
	
	static sl_real getScreenMaximum();
	
};

#define SLIB_DECLARE_MENU_BEGIN(NAME) \
class NAME : public slib::Referable { \
public: \
	static const NAME* get(); \
	NAME(); \
	slib::Ref<slib::Menu> root; \
	slib::Ref<slib::Menu> root_menu;

#define SLIB_DECLARE_SUBMENU(NAME) \
	slib::Ref<slib::Menu> NAME##_menu; \
	slib::Ref<slib::MenuItem> NAME;

#define SLIB_DECLARE_MENU_ITEM(NAME) \
	slib::Ref<slib::MenuItem> NAME;

#define SLIB_DECLARE_MENU_SEPARATOR(NAME) \
	slib::Ref<slib::MenuItem> NAME;

#define SLIB_DECLARE_MENU_END };


#define SLIB_DEFINE_MENU_BEGIN(NAME) \
const NAME* NAME::get() { \
	SLIB_SAFE_STATIC(NAME, ret); \
	if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) { \
		return sl_null; \
	} \
	return &ret; \
} \
NAME::NAME() { \
	root = root_menu = slib::Menu::create(); \
	if (root.isNull()) return;

#define SLIB_DEFINE_SUBMENU(PARENT, NAME, ...) \
	NAME##_menu = slib::Menu::create(); \
	if (NAME##_menu.isNull()) return; \
	NAME = PARENT##_menu->addSubmenu(NAME##_menu, __VA_ARGS__);

#define SLIB_DEFINE_MENU_ITEM(PARENT, NAME, ...) \
	NAME = PARENT##_menu->addMenuItem(__VA_ARGS__);

#define SLIB_DEFINE_MENU_SEPARATOR(PARENT, NAME) \
	NAME = PARENT##_menu->addSeparator();

#define SLIB_DEFINE_MENU_SEPARATOR_NONAME(PARENT) \
	PARENT##_menu->addSeparator();

#define SLIB_DEFINE_MENU_END }

#define SLIB_DECLARE_UILAYOUT_BEGIN(NAME, BASE_CLASS) \
	class NAME : public BASE_CLASS \
	{ \
		SLIB_DECLARE_OBJECT \
	public: \
		NAME(sl_real customUnitLength = 1); \
	protected: \
		void initialize(); \
		void layoutViews(sl_real width, sl_real height);


#define SLIB_DECLARE_UILAYOUT_END \
	};

#define SLIB_DEFINE_UILAYOUT(NAME, BASE_CLASS) \
SLIB_DEFINE_OBJECT(NAME, BASE_CLASS) \
NAME::NAME(sl_real customUnitLength) \
: BASE_CLASS(customUnitLength) \
{ \
	SLIB_REFERABLE_CONSTRUCTOR \
	initialize(); \
	layoutViews(0, 0); \
}

class UILayoutResource
{
public:
	UILayoutResource(sl_real customUnitLength = 1);
	
public:
	Ref<View> getContent();
	
	sl_real getCustomUnitLength();
	
	void setCustomUnitLength(sl_real length);
	
protected:
	virtual void layoutViews(sl_real width, sl_real height) = 0;
	
protected:
	void _layoutViews_safe(sl_real width, sl_real height);
	
protected:
	View* m_contentView;
	Ref<View> m_contentViewRef;

	sl_real m_customUnit;
	
	sl_int32 m_countRecursiveLayout;
};

#define SLIB_DECLARE_WINDOW_LAYOUT_BEGIN(NAME) \
	SLIB_DECLARE_UILAYOUT_BEGIN(NAME, slib::WindowLayoutResource)

#define SLIB_DECLARE_WINDOW_LAYOUT_END \
	SLIB_DECLARE_UILAYOUT_END

#define SLIB_DEFINE_WINDOW_LAYOUT(NAME) \
	SLIB_DEFINE_UILAYOUT(NAME, slib::WindowLayoutResource)


class WindowLayoutResource : public Window, public UILayoutResource
{
	SLIB_DECLARE_OBJECT
public:
	WindowLayoutResource(sl_real customUnitLength = 1);
	
public:
	// override
	void dispatchResize(Size& size);
	
	// override
	void dispatchMaximize();
	
	// override
	void dispatchMinimize();
	
protected:
	void _layoutViews_safe();

};

#define SLIB_DECLARE_VIEW_LAYOUT_BEGIN(NAME) \
	SLIB_DECLARE_UILAYOUT_BEGIN(NAME, slib::ViewLayoutResource)

#define SLIB_DECLARE_VIEW_LAYOUT_END \
	SLIB_DECLARE_UILAYOUT_END

#define SLIB_DEFINE_VIEW_LAYOUT(NAME) \
	SLIB_DEFINE_UILAYOUT(NAME, slib::ViewLayoutResource)

class ViewLayoutResource : public ViewGroup, public UILayoutResource
{
	SLIB_DECLARE_OBJECT
public:
	ViewLayoutResource(sl_real customUnitLength = 1);
	
public:
	// override
	void dispatchResize(sl_real width, sl_real height);
	
};

SLIB_UI_NAMESPACE_END

#endif
