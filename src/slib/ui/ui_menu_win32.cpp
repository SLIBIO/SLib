#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/menu.h"
#include "../../../inc/slib/ui/platform.h"

#include "ui_core_win32.h"

SLIB_UI_NAMESPACE_BEGIN

class _Win32_Menu;
typedef HashMap<HMENU, WeakRef<_Win32_Menu> > _UiMenuMap;
SLIB_SAFE_STATIC_GETTER(_UiMenuMap, _UI_getMenu);

class _Win32_MenuItem : public MenuItem
{
	SLIB_DECLARE_OBJECT

public:
	HBITMAP m_hbmChecked;
	HBITMAP m_hbmUnchecked;

public:
	_Win32_MenuItem()
	{
		m_hbmChecked = NULL;
		m_hbmUnchecked = NULL;
	}

	~_Win32_MenuItem()
	{
		if (m_hbmChecked) {
			::DeleteObject(m_hbmChecked);
		}
		if (m_hbmUnchecked) {
			::DeleteObject(m_hbmUnchecked);
		}
	}

	static Ref<_Win32_MenuItem> create(_Win32_Menu* parent, sl_uint32 index, const MenuItemParam& param);

	static String makeText(const String& title, const KeycodeAndModifiers& shortcutKey, const KeycodeAndModifiers& secondShortcutKey);

	void _updateText();

	// override
	void setText(const String& text)
	{
		MenuItem::setText(text);
		_updateText();
	}

	// override
	void setShortcutKey(const KeycodeAndModifiers& km)
	{
		MenuItem::setShortcutKey(km);
		_updateText();
	}

	// override
	void setSecondShortcutKey(const KeycodeAndModifiers& km)
	{
		MenuItem::setSecondShortcutKey(km);
		_updateText();
	}

	void _updateState();

	// override
	void setEnabled(sl_bool flag)
	{
		MenuItem::setEnabled(flag);
		_updateState();
	}

	// override
	void setChecked(sl_bool flag)
	{
		MenuItem::setChecked(flag);
		_updateState();
	}

	// override
	void setIcon(const Ref<Bitmap>& icon);

	// override
	void setCheckedIcon(const Ref<Bitmap>& icon);

	// override
	void setSubmenu(const Ref<Menu>& menu);

};

SLIB_DEFINE_OBJECT(_Win32_MenuItem, MenuItem)

class _Win32_Menu : public Menu
{
	SLIB_DECLARE_OBJECT

public:
	HMENU m_hMenu;

public:
	_Win32_Menu()
	{
		m_hMenu = NULL;
	}

	~_Win32_Menu()
	{
		if (m_hMenu) {
			::DestroyMenu(m_hMenu);
			_UiMenuMap* map = &(_UI_getMenu());
			if (map) {
				map->remove(m_hMenu);
			}
		}
	}

	static Ref<_Win32_Menu> create()
	{
		HMENU hMenu = ::CreateMenu();
		if (hMenu) {
			MENUINFO mi;
			mi.cbSize = sizeof(mi);
			mi.fMask = MIM_STYLE;
			mi.dwStyle = MNS_NOTIFYBYPOS;
			if (::SetMenuInfo(hMenu, &mi)) {
				Ref<_Win32_Menu> ret = new _Win32_Menu();
				if (ret.isNotNull()) {
					ret->m_hMenu = hMenu;
					_UI_getMenu().put(hMenu, ret);
					return ret;
				}
			}
			::DestroyMenu(hMenu);
		}
		return Ref<_Win32_Menu>::null();
	}

	// override
	Ref<MenuItem> addMenuItem(const MenuItemParam& param)
	{
		return insertMenuItem(SLIB_UINT32_MAX, param);
	}

	// override
	Ref<MenuItem> insertMenuItem(sl_uint32 index, const MenuItemParam& param)
	{
		ObjectLocker lock(this);
		sl_uint32 n = (sl_uint32)(m_items.getCount());
		if (index > n) {
			index = n;
		}
		Ref<MenuItem> item = _Win32_MenuItem::create(this, index, param);
		if (item.isNotNull()) {
			m_items.insert(index, item);
			return item;
		}
		return Ref<MenuItem>::null();
	}

	Ref<MenuItem> addSeparator()
	{
		return insertSeparator(SLIB_UINT32_MAX);
	}
	
	Ref<MenuItem> insertSeparator(sl_uint32 index)
	{
		ObjectLocker lock(this);
		sl_uint32 n = (sl_uint32)(m_items.getCount());
		if (index > n) {
			index = n;
		}
		::MENUITEMINFOW mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_FTYPE;
		mii.fType = MFT_SEPARATOR;
		if (::InsertMenuItemW(m_hMenu, index, TRUE, &mii)) {
			Ref<MenuItem> item = MenuItem::createSeparator();
			if (item.isNotNull()) {
				m_items.insert(index, item);
				return item;
			}
		}
		return Ref<MenuItem>::null();
	}

	// override
	void removeMenuItem(sl_uint32 index)
	{
		ObjectLocker lock(this);
		if (index < m_items.getCount()) {
			::RemoveMenu(m_hMenu, index, MF_BYPOSITION);
			m_items.remove(index);
		}
	}

	// override
	void removeMenuItem(const Ref<MenuItem>& item)
	{
		ObjectLocker lock(this);
		sl_reg index = m_items.indexOf(item);
		if (index >= 0) {
			::RemoveMenu(m_hMenu, (sl_uint32)index, MF_BYPOSITION);
			m_items.remove(index);
		}
	}

	// override
	void show(sl_real x, sl_real y)
	{
		::TrackPopupMenuEx(m_hMenu, 0, (int)x, (int)y, Win32_UI_Shared::get()->hWndMessage, NULL);
	}

	friend class _Win32_MenuItem;
};

SLIB_DEFINE_OBJECT(_Win32_Menu, Menu)

Ref<Menu> Menu::create()
{
	return _Win32_Menu::create();
}

Ref<_Win32_MenuItem> _Win32_MenuItem::create(_Win32_Menu* parent, sl_uint32 index, const MenuItemParam& param)
{
	MENUITEMINFOW mii;
	Base::zeroMemory(&mii, sizeof(mii));
	mii.cbSize = sizeof(MENUITEMINFOW);
	mii.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_STRING;
	mii.fType = MFT_STRING;
	mii.fState = 0;
	if (param.flagChecked) {
		mii.fState |= MFS_CHECKED;
	}
	if (!(param.flagEnabled)) {
		mii.fState |= MFS_DISABLED;
	}
	if (param.submenu.isNotNull()) {
		mii.hSubMenu = UIPlatform::getMenuHandle(param.submenu);
		if (mii.hSubMenu) {
			mii.fMask |= MIIM_SUBMENU;
		}
	}
	mii.hbmpUnchecked = UIPlatform::createDIBFromBitmap(param.icon);
	mii.hbmpChecked = UIPlatform::createDIBFromBitmap(param.checkedIcon);
	if (mii.hbmpUnchecked || mii.hbmpChecked) {
		mii.fMask |= MIIM_CHECKMARKS;
	}
	String16 text = makeText(param.text, param.shortcutKey, param.secondShortcutKey);
	mii.dwTypeData = text.getData();
	if (::InsertMenuItemW(parent->m_hMenu, index, TRUE, &mii)) {
		Ref<_Win32_MenuItem> ret = new _Win32_MenuItem;
		if (ret.isNotNull()) {
			ret->m_parent = parent;
			ret->m_text = param.text;
			ret->m_shortcutKey = param.shortcutKey;
			ret->m_secondShortcutKey = param.secondShortcutKey;
			ret->m_flagEnabled = param.flagEnabled;
			ret->m_flagChecked = param.flagChecked;
			ret->m_icon = param.icon;
			ret->m_checkedIcon = param.checkedIcon;
			ret->m_submenu = param.submenu;
			ret->m_action = param.action;
			ret->m_hbmUnchecked = mii.hbmpUnchecked;
			ret->m_hbmChecked = mii.hbmpChecked;
			return ret;
		}
	}
	if (mii.hbmpChecked) {
		::DeleteObject(mii.hbmpChecked);
	}
	if (mii.hbmpUnchecked) {
		::DeleteObject(mii.hbmpUnchecked);
	}
	return Ref<_Win32_MenuItem>::null();
}

String _Win32_MenuItem::makeText(const String& title, const KeycodeAndModifiers& shortcutKey, const KeycodeAndModifiers& secondShortcutKey)
{
	String text = title;
	if (shortcutKey.getKeycode() != Keycode::Unknown) {
		text += "\t";
		text += shortcutKey.toString();
		if (secondShortcutKey.getKeycode() != Keycode::Unknown) {
			text += ", ";
			text += secondShortcutKey.toString();
		}
	} else {
		if (secondShortcutKey.getKeycode() != Keycode::Unknown) {
			text += "\t";
			text += secondShortcutKey.toString();
		}
	}
	return text;
}

#define MENU_ITEM_SET_PROLOG \
	Ref<_Win32_Menu> parent(WeakRef<_Win32_Menu>::from(m_parent)); \
	if (parent.isNull()) { \
		return; \
	} \
	ObjectLocker lock(parent.ptr); \
	sl_reg _index = parent->m_items.indexOf(this); \
	if (_index < 0) { \
		return; \
	} \
	HMENU hMenu = parent->m_hMenu; \
	sl_uint32 index = (sl_uint32)_index; \
	MENUITEMINFOW mii; \
	Base::zeroMemory(&mii, sizeof(mii)); \
	mii.cbSize = sizeof(mii);

void _Win32_MenuItem::_updateText()
{
	MENU_ITEM_SET_PROLOG;
	mii.fMask = MIIM_STRING;
	String16 text = makeText(m_text, m_shortcutKey, m_secondShortcutKey);
	mii.dwTypeData = text.getData();
	::SetMenuItemInfoW(hMenu, index, TRUE, &mii);
}

void _Win32_MenuItem::_updateState()
{
	MENU_ITEM_SET_PROLOG;
	mii.fMask = MIIM_STATE;
	mii.fState = 0;
	if (m_flagChecked) {
		mii.fState |= MFS_CHECKED;
	}
	if (!(m_flagEnabled)) {
		mii.fState |= MFS_DISABLED;
	}
	::SetMenuItemInfoW(hMenu, index, TRUE, &mii);
}

void _Win32_MenuItem::setIcon(const Ref<Bitmap>& icon)
{
	MenuItem::setIcon(icon);
	MENU_ITEM_SET_PROLOG;
	if (m_hbmUnchecked) {
		::DeleteObject(m_hbmUnchecked);
	}
	m_hbmUnchecked = UIPlatform::createDIBFromBitmap(icon);
	mii.fMask = MIIM_CHECKMARKS;
	mii.hbmpUnchecked = m_hbmUnchecked;
	mii.hbmpChecked = m_hbmChecked;
	::SetMenuItemInfoW(hMenu, index, TRUE, &mii);
}

void _Win32_MenuItem::setCheckedIcon(const Ref<Bitmap>& icon)
{
	MenuItem::setIcon(icon);
	MENU_ITEM_SET_PROLOG;
	if (m_hbmChecked) {
		::DeleteObject(m_hbmChecked);
	}
	m_hbmChecked = UIPlatform::createDIBFromBitmap(icon);
	mii.fMask = MIIM_CHECKMARKS;
	mii.hbmpUnchecked = m_hbmUnchecked;
	mii.hbmpChecked = m_hbmChecked;
	::SetMenuItemInfoW(hMenu, index, TRUE, &mii);
}

void _Win32_MenuItem::setSubmenu(const Ref<Menu>& menu)
{
	MenuItem::setSubmenu(menu);
	MENU_ITEM_SET_PROLOG;
	mii.fMask = MIIM_SUBMENU;
	mii.hSubMenu = UIPlatform::getMenuHandle(menu);
	::SetMenuItemInfoW(hMenu, index, TRUE, &mii);
}


HMENU UIPlatform::getMenuHandle(const Ref<Menu>& menu)
{
	if (_Win32_Menu::checkInstance(menu.ptr)) {
		return ((_Win32_Menu*)(menu.ptr))->m_hMenu;
	}
	return NULL;
}

Ref<Menu> UIPlatform::getMenu(HMENU hMenu)
{
	return _UI_getMenu().getValue(hMenu, WeakRef<_Win32_Menu>::null());
}

void _Win32_processMenuCommand(WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu = (HMENU)(lParam);
	sl_uint32 index = (sl_uint32)(wParam);
	Ref<_Win32_Menu> menu(_UI_getMenu().getValue(hMenu, WeakRef<_Win32_Menu>::null()));
	if (menu.isNotNull()) {
		Ref<MenuItem> item = menu->getMenuItem(index);
		if (item.isNotNull()) {
			Ref<Runnable> action = item->getAction();
			if (action.isNotNull()) {
				action->run();
			}
		}
	}
}

SLIB_UI_NAMESPACE_END

#endif
