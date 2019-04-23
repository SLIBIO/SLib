#include "menus.h"

#include "strings.h"
#include "drawables.h"

namespace slib
{
	namespace menu
	{

		SLIB_DEFINE_MENU_BEGIN(PopupMenuChatItem)
			SLIB_DEFINE_MENU_ITEM(root, copy, "Copy", slib::Keycode::Unknown)
			SLIB_DEFINE_MENU_SEPARATOR_NONAME(root)
			SLIB_DEFINE_MENU_ITEM(root, remove, "Delete", slib::Keycode::Unknown)
		SLIB_DEFINE_MENU_END

	}
}
