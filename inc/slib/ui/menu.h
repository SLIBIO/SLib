#ifndef CHECKHEADER_SLIB_UI_MENU
#define CHECKHEADER_SLIB_UI_MENU

#include "definition.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/callback.h"
#include "../graphics/bitmap.h"

#include "event.h"

SLIB_UI_NAMESPACE_BEGIN

class Menu;

class MenuItem : public Object
{
	SLIB_DECLARE_OBJECT

protected:
	MenuItem();

public:
	Ref<Menu> getParent() const;

	String getText() const;

	virtual void setText(const String& text);

	const KeycodeAndModifiers& getShortcutKey() const;

	virtual void setShortcutKey(const KeycodeAndModifiers& km);
	
	const KeycodeAndModifiers& getSecondShortcutKey() const;
	
	virtual void setSecondShortcutKey(const KeycodeAndModifiers& km);

	sl_bool isEnabled() const;

	virtual void setEnabled(sl_bool flag);

	sl_bool isChecked() const;

	virtual void setChecked(sl_bool flag);

	Ref<Bitmap> getIcon() const;

	virtual void setIcon(const Ref<Bitmap>& icon);

	Ref<Bitmap> getCheckedIcon() const;

	virtual void setCheckedIcon(const Ref<Bitmap>& icon);

	Ref<Menu> getSubmenu() const;

	virtual void setSubmenu(const Ref<Menu>& menu);

	Ref<Runnable> getAction() const;

	virtual void setAction(const Ref<Runnable>& action);

	virtual sl_bool isSeparator() const;

	static Ref<MenuItem> createSeparator();

protected:
	WeakRef<Menu> m_parent;

	SafeString m_text;
	KeycodeAndModifiers m_shortcutKey;
	KeycodeAndModifiers m_secondShortcutKey;
	sl_bool m_flagEnabled;
	sl_bool m_flagChecked;
	SafeRef<Bitmap> m_icon;
	SafeRef<Bitmap> m_checkedIcon;
	Ref<Menu> m_submenu;
	SafeRef<Runnable> m_action;

};

class MenuItemParam
{
public:
	String text;
	KeycodeAndModifiers shortcutKey;
	KeycodeAndModifiers secondShortcutKey;
	sl_bool flagEnabled;
	sl_bool flagChecked;
	Ref<Bitmap> icon;
	Ref<Bitmap> checkedIcon;
	Ref<Menu> submenu;
	Ref<Runnable> action;

public:
	MenuItemParam();

};

class Menu : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	Menu();

public:
	static Ref<Menu> create();

	sl_uint32 getMenuItemsCount() const;

	Ref<MenuItem> getMenuItem(sl_uint32 index) const;

	virtual Ref<MenuItem> addMenuItem(const MenuItemParam& param) = 0;

	virtual Ref<MenuItem> insertMenuItem(sl_uint32 index, const MenuItemParam& param) = 0;

	virtual Ref<MenuItem> addSeparator() = 0;

	virtual Ref<MenuItem> insertSeparator(sl_uint32 index) = 0;

	virtual void removeMenuItem(sl_uint32 index) = 0;

	virtual void removeMenuItem(const Ref<MenuItem>& item) = 0;

	virtual void show(sl_real x, sl_real y) = 0;

	void show(const Point& pt);
	
	Ref<MenuItem> addMenuItem(const String& title);
	
	Ref<MenuItem> addMenuItem(const String& title, const Ref<Bitmap>& icon);
	
	Ref<MenuItem> addMenuItem(const String& title, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon);
	
	Ref<MenuItem> addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey);
	
	Ref<MenuItem> addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey, const Ref<Bitmap>& icon);
	
	Ref<MenuItem> addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon);
	
	Ref<MenuItem> addSubmenu(Ref<Menu>& submenu, const String& title);
	
	Ref<MenuItem> addSubmenu(Ref<Menu>& submenu, const String& title, const Ref<Bitmap>& icon);
	
	Ref<MenuItem> addSubmenu(Ref<Menu>& submenu, const String& title, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon);
	
protected:
	CList< Ref<MenuItem> > m_items;

};

SLIB_UI_NAMESPACE_END

#endif
