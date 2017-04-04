/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "slib/ui/menu.h"
#include "slib/ui/core.h"
#include "slib/ui/app.h"
#include "slib/ui/platform.h"

@interface _OSX_MenuItemHandle : NSMenuItem
{
	@public slib::WeakRef<slib::MenuItem> m_item;
	@public NSImage* m_defaultCheckedImage;
}
@end

namespace slib
{

	class _OSX_Menu;

	class _OSX_MenuItem : public MenuItem
	{
		SLIB_DECLARE_OBJECT
		
	public:
		_OSX_MenuItemHandle* m_handle;

	public:
		static Ref<_OSX_MenuItem> create(_OSX_Menu* parent, const MenuItemParam& param);
		
		// override
		void setText(const String& text);

		// override
		void setShortcutKey(const KeycodeAndModifiers& km);
		
		// override
		void setSecondShortcutKey(const KeycodeAndModifiers& km);
		
		// override
		void setEnabled(sl_bool flag);

		// override
		void setChecked(sl_bool flag);

		// override
		void setIcon(const Ref<Bitmap>& icon);

		// override
		void setCheckedIcon(const Ref<Bitmap>& icon);

		// override
		void setSubmenu(const Ref<Menu>& menu);
		
		static NSImage* _createIcon(const Ref<Bitmap>& iconSrc);

	};

	SLIB_DEFINE_OBJECT(_OSX_MenuItem, MenuItem)

	class _OSX_Menu : public Menu
	{
		SLIB_DECLARE_OBJECT

	public:
		NSMenu* m_handle;

	public:
		static Ref<_OSX_Menu> create()
		{
			NSMenu* handle = [[NSMenu alloc] initWithTitle:@""];
			if (handle != nil) {
				handle.autoenablesItems = NO;
				Ref<_OSX_Menu> ret = new _OSX_Menu();
				if (ret.isNotNull()) {
					ret->m_handle = handle;
					return ret;
				}
			}
			return sl_null;
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
			Ref<_OSX_MenuItem> item = _OSX_MenuItem::create(this, param);
			if (item.isNotNull()) {
				[m_handle insertItem:item->m_handle atIndex:index];
				m_items.insert(index, item);
				return item;
			}
			return sl_null;
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
			Ref<MenuItem> item = MenuItem::createSeparator();
			if (item.isNotNull()) {
				[m_handle insertItem:[NSMenuItem separatorItem] atIndex:index];
				m_items.insert(index, item);
				return item;
			}
			return sl_null;
		}

		// override
		void removeMenuItem(sl_uint32 index)
		{
			ObjectLocker lock(this);
			if (index < m_items.getCount()) {
				[m_handle removeItemAtIndex:index];
				m_items.removeAt(index);
			}
		}

		// override
		void removeMenuItem(const Ref<MenuItem>& item)
		{
			ObjectLocker lock(this);
			sl_reg index = m_items.indexOf(item);
			if (index >= 0) {
				[m_handle removeItemAtIndex:index];
				m_items.removeAt(index);
			}
		}

		// override
		void show(sl_ui_pos x, sl_ui_pos y)
		{
			NSPoint pt;
			pt.x = (CGFloat)x;
			pt.y = (CGFloat)(UI::getScreenSize().y - y);
			[m_handle popUpMenuPositioningItem:nil atLocation:pt inView:nil];
		}

		friend class _OSX_MenuItem;
	};

	SLIB_DEFINE_OBJECT(_OSX_Menu, Menu)

	Ref<Menu> Menu::create()
	{
		return _OSX_Menu::create();
	}

	Ref<_OSX_MenuItem> _OSX_MenuItem::create(_OSX_Menu* parent, const MenuItemParam& param)
	{
		_OSX_MenuItemHandle* handle = [[_OSX_MenuItemHandle alloc] init];
		if (handle != nil) {
			handle.title = Apple::getNSStringFromString(param.text.replaceAll("&", String::null()));
			NSUInteger keMask;
			if (param.shortcutKey.getKeycode() != Keycode::Unknown) {
				handle.keyEquivalent = UIPlatform::getKeyEquivalent(param.shortcutKey, keMask);
				handle.keyEquivalentModifierMask = keMask;
			} else {
				handle.keyEquivalent = UIPlatform::getKeyEquivalent(param.secondShortcutKey, keMask);
				handle.keyEquivalentModifierMask = keMask;
			}
			handle.enabled = param.flagEnabled ? YES : NO;
			handle.state = param.flagChecked ? NSOnState : NSOffState;
			handle.submenu = UIPlatform::getMenuHandle(param.submenu);
			if (handle.submenu != nil) {
				handle.submenu.title = handle.title;
			}
			handle.offStateImage = _createIcon(param.icon);
			handle.onStateImage = _createIcon(param.checkedIcon);
			if (handle.onStateImage == nil) {
				handle.onStateImage = handle->m_defaultCheckedImage;
			}
			Ref<_OSX_MenuItem> ret = new _OSX_MenuItem;
			if (ret.isNotNull()) {
				ret->m_handle = handle;
				handle->m_item = ret.get();
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
				return ret;
			}

		}
		return sl_null;
	}

	void _OSX_MenuItem::setText(const String& text)
	{
		MenuItem::setText(text);
		m_handle.title = Apple::getNSStringFromString(text.replaceAll("&", String::null()));
		if (m_handle.submenu != nil) {
			m_handle.submenu.title = m_handle.title;
		}
	}

	void _OSX_MenuItem::setShortcutKey(const slib::KeycodeAndModifiers &km)
	{
		MenuItem::setShortcutKey(km);
		NSUInteger keMask;
		if (km.getKeycode() != Keycode::Unknown) {
			m_handle.keyEquivalent = UIPlatform::getKeyEquivalent(km, keMask);
			m_handle.keyEquivalentModifierMask = keMask;
		} else {
			m_handle.keyEquivalent = UIPlatform::getKeyEquivalent(m_secondShortcutKey, keMask);
			m_handle.keyEquivalentModifierMask = keMask;
		}
	}

	void _OSX_MenuItem::setSecondShortcutKey(const slib::KeycodeAndModifiers &km)
	{
		MenuItem::setSecondShortcutKey(km);
		NSUInteger keMask;
		if (m_shortcutKey.getKeycode() == Keycode::Unknown) {
			m_handle.keyEquivalent = UIPlatform::getKeyEquivalent(km, keMask);
			m_handle.keyEquivalentModifierMask = keMask;
		}
	}

	void _OSX_MenuItem::setEnabled(sl_bool flag)
	{
		MenuItem::setEnabled(flag);
		m_handle.enabled = flag ? YES : NO;
	}

	void _OSX_MenuItem::setChecked(sl_bool flag)
	{
		MenuItem::setChecked(flag);
		m_handle.state = flag ? NSOnState : NSOffState;
	}

	void _OSX_MenuItem::setIcon(const Ref<Bitmap>& icon)
	{
		MenuItem::setIcon(icon);
		m_handle.offStateImage = GraphicsPlatform::createNSImageFromBitmap(icon);
	}

	void _OSX_MenuItem::setCheckedIcon(const Ref<Bitmap>& icon)
	{
		MenuItem::setCheckedIcon(icon);
		m_handle.onStateImage = GraphicsPlatform::createNSImageFromBitmap(icon);
		if (m_handle.onStateImage == nil) {
			m_handle.onStateImage = m_handle->m_defaultCheckedImage;
		}
	}

	void _OSX_MenuItem::setSubmenu(const Ref<Menu>& menu)
	{
		MenuItem::setSubmenu(menu);
		m_handle.submenu = UIPlatform::getMenuHandle(menu);
		if (m_handle.submenu != nil) {
			m_handle.submenu.title = m_handle.title;
		}
	}

	NSImage* _OSX_MenuItem::_createIcon(const Ref<Bitmap>& iconSrc)
	{
		if (iconSrc.isNotNull()) {
			NSImage* icon = GraphicsPlatform::createNSImageFromBitmap(iconSrc);
			if (icon != nil) {
				if (icon.size.width > 0 && icon.size.height > 0) {
					double w = [[NSFont menuFontOfSize:0] pointSize];
					NSSize size;
					size.width = w;
					size.height = w;
					icon.size = size;
					return icon;
				}
			}
		}
		return nil;
	}


	NSMenu* UIPlatform::getMenuHandle(const Ref<Menu>& menu)
	{
		if (_OSX_Menu* _menu = CastInstance<_OSX_Menu>(menu.get())) {
			return _menu->m_handle;
		}
		return nil;
	}

	NSMenuItem* UIPlatform::getMenuItemHandle(const Ref<MenuItem>& item)
	{
		if (_OSX_MenuItem* _item = CastInstance<_OSX_MenuItem>(item.get())) {
			return _item->m_handle;
		}
		return nil;
	}

	void UIApp::setMenu(const Ref<Menu>& _menu)
	{
		m_mainMenu = _menu;
		NSMenu* menu = UIPlatform::getMenuHandle(_menu);
		if (menu != nil) {
			NSApplication* app = [NSApplication sharedApplication];
			[app setMainMenu: menu];
			Ref<MenuItem> item = _menu->getMenuItem(0);
			if (item.isNotNull()) {
				String text = item->getText();
				if (text.isNotEmpty()) {
					NSMenuItem* nsItem = [menu itemAtIndex:0];
					if (nsItem != nil) {
						NSMenu* nsSubMenu = [nsItem submenu];
						if (nsSubMenu != nil) {
							[nsSubMenu setTitle:(Apple::getNSStringFromString(text.replaceAll("&", String::null()) + " "))];
						}
					}
				}
			}
		}
	}

}

@implementation _OSX_MenuItemHandle

-(id)init
{
	self = [super init];
	if (self != nil) {
		self.target = self;
		self.action = @selector(onAction);
		self->m_defaultCheckedImage = self.onStateImage;
	}
	return self;
}

-(void)onAction
{
	slib::Ref<slib::MenuItem> item(m_item);
	if (item.isNotNull()) {
		(item->getAction())();
	}
}

@end

#endif
