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

#ifndef CHECKHEADER_SLIB_UI_MENU
#define CHECKHEADER_SLIB_UI_MENU

#include "definition.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/function.h"
#include "../graphics/bitmap.h"

#include "event.h"

namespace slib
{

	class Menu;
	
	class MenuItem : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		MenuItem();
		
		~MenuItem();

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
		
		virtual sl_bool isSeparator() const;
		
		static Ref<MenuItem> createSeparator();
		
		sl_bool processShortcutKey(const KeycodeAndModifiers& km);

	public:
		SLIB_PROPERTY_FUNCTION(void(), Action)
		
	protected:
		WeakRef<Menu> m_parent;
		
		AtomicString m_text;
		KeycodeAndModifiers m_shortcutKey;
		KeycodeAndModifiers m_secondShortcutKey;
		sl_bool m_flagEnabled;
		sl_bool m_flagChecked;
		AtomicRef<Bitmap> m_icon;
		AtomicRef<Bitmap> m_checkedIcon;
		AtomicRef<Menu> m_submenu;
		
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
		Function<void()> action;
		
	public:
		MenuItemParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(MenuItemParam)

	};
	
	class Menu : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		Menu();
		
		~Menu();

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
		
		virtual void show(sl_ui_pos x, sl_ui_pos y) = 0;
		
		void show(const UIPoint& pt);
		
		Ref<MenuItem> addMenuItem(const String& title);
		
		Ref<MenuItem> addMenuItem(const String& title, const Ref<Bitmap>& icon);
		
		Ref<MenuItem> addMenuItem(const String& title, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon);
		
		Ref<MenuItem> addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey);
		
		Ref<MenuItem> addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey, const Ref<Bitmap>& icon);
		
		Ref<MenuItem> addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon);
		
		Ref<MenuItem> addSubmenu(Ref<Menu>& submenu, const String& title);
		
		Ref<MenuItem> addSubmenu(Ref<Menu>& submenu, const String& title, const Ref<Bitmap>& icon);
		
		Ref<MenuItem> addSubmenu(Ref<Menu>& submenu, const String& title, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon);
		
		sl_bool processShortcutKey(const KeycodeAndModifiers& km);
		
	protected:
		CList< Ref<MenuItem> > m_items;
		
	};

}

#endif
