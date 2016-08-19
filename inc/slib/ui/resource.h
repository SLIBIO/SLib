#ifndef CHECKHEADER_SLIB_UI_RESOURCE
#define CHECKHEADER_SLIB_UI_RESOURCE

#include "definition.h"

#include "menu.h"
#include "event.h"
#include "view.h"
#include "window.h"

#include "../core/resource.h"

SLIB_UI_NAMESPACE_BEGIN

#define SLIB_DECLARE_MENU_BEGIN(NAME) \
class NAME : public slib::Referable { \
public: \
	static const NAME& get(); \
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
const NAME& NAME::get() { \
	SLIB_SAFE_STATIC(NAME, ret); \
	return ret; \
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

SLIB_UI_NAMESPACE_END

#endif
