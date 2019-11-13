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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/menu.h"
#include "slib/ui/core.h"
#include "slib/ui/app.h"
#include "slib/ui/platform.h"

@interface SLIBMenuItemHandle : NSMenuItem
{
	@public slib::WeakRef<slib::MenuItem> m_item;
	@public NSImage* m_defaultCheckedImage;
}
@end

namespace slib
{
	
	namespace priv
	{
		namespace menu
		{
			
			class MenuImpl;
			
			class MenuItemImpl : public MenuItem
			{
				SLIB_DECLARE_OBJECT
				
			public:
				SLIBMenuItemHandle* m_handle;
				
			public:
				static Ref<MenuItemImpl> create(MenuImpl* parent, const MenuItemParam& param);
				
				void setText(const String& text) override;
				
				void setShortcutKey(const KeycodeAndModifiers& km) override;
				
				void setSecondShortcutKey(const KeycodeAndModifiers& km) override;
				
				void setEnabled(sl_bool flag) override;
				
				void setChecked(sl_bool flag) override;
				
				void setIcon(const Ref<Bitmap>& icon) override;
				
				void setCheckedIcon(const Ref<Bitmap>& icon) override;
				
				void setSubmenu(const Ref<Menu>& menu) override;
				
				static NSImage* _createIcon(const Ref<Bitmap>& iconSrc);
				
			};
			
			SLIB_DEFINE_OBJECT(MenuItemImpl, MenuItem)
			
			class MenuImpl : public Menu
			{
				SLIB_DECLARE_OBJECT
				
			public:
				NSMenu* m_handle;
				
			public:
				static Ref<MenuImpl> create()
				{
					NSMenu* handle = [[NSMenu alloc] initWithTitle:@""];
					if (handle != nil) {
						handle.autoenablesItems = NO;
						Ref<MenuImpl> ret = new MenuImpl();
						if (ret.isNotNull()) {
							ret->m_handle = handle;
							return ret;
						}
					}
					return sl_null;
				}
				
				Ref<MenuItem> addMenuItem(const MenuItemParam& param) override
				{
					return insertMenuItem(SLIB_UINT32_MAX, param);
				}
				
				Ref<MenuItem> insertMenuItem(sl_uint32 index, const MenuItemParam& param) override
				{
					ObjectLocker lock(this);
					sl_uint32 n = (sl_uint32)(m_items.getCount());
					if (index > n) {
						index = n;
					}
					Ref<MenuItemImpl> item = MenuItemImpl::create(this, param);
					if (item.isNotNull()) {
						[m_handle insertItem:item->m_handle atIndex:index];
						m_items.insert(index, item);
						return item;
					}
					return sl_null;
				}
				
				Ref<MenuItem> addSeparator() override
				{
					return insertSeparator(SLIB_UINT32_MAX);
				}
				
				Ref<MenuItem> insertSeparator(sl_uint32 index) override
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
				
				void removeMenuItem(sl_uint32 index) override
				{
					ObjectLocker lock(this);
					if (index < m_items.getCount()) {
						[m_handle removeItemAtIndex:index];
						m_items.removeAt(index);
					}
				}
				
				void removeMenuItem(const Ref<MenuItem>& item) override
				{
					ObjectLocker lock(this);
					sl_reg index = m_items.indexOf(item);
					if (index >= 0) {
						[m_handle removeItemAtIndex:index];
						m_items.removeAt(index);
					}
				}
				
				void show(sl_ui_pos x, sl_ui_pos y) override
				{
					NSPoint pt;
					pt.x = (CGFloat)x;
					pt.y = (CGFloat)(UI::getScreenSize().y - y);
					[m_handle popUpMenuPositioningItem:nil atLocation:pt inView:nil];
				}
				
				friend class MenuItemImpl;
			};
			
			SLIB_DEFINE_OBJECT(MenuImpl, Menu)

			Ref<MenuItemImpl> MenuItemImpl::create(MenuImpl* parent, const MenuItemParam& param)
			{
				SLIBMenuItemHandle* handle = [[SLIBMenuItemHandle alloc] init];
				if (handle != nil) {
					handle.title = Apple::getNSStringFromString(param.text.replaceAll("&", String::null()));
					NSUInteger keMask;
					if (param.shortcutKey.getKeycode() != Keycode::Unknown) {
						handle.keyEquivalent = UIPlatform::getKeyEquivalent(param.shortcutKey, keMask);
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
					Ref<MenuItemImpl> ret = new MenuItemImpl;
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
						ret->setAction(param.action);
						return ret;
					}
					
				}
				return sl_null;
			}
			
			void MenuItemImpl::setText(const String& text)
			{
				MenuItem::setText(text);
				m_handle.title = Apple::getNSStringFromString(text.replaceAll("&", String::null()));
				if (m_handle.submenu != nil) {
					m_handle.submenu.title = m_handle.title;
				}
			}
			
			void MenuItemImpl::setShortcutKey(const slib::KeycodeAndModifiers &km)
			{
				MenuItem::setShortcutKey(km);
				NSUInteger keMask;
				if (km.getKeycode() != Keycode::Unknown) {
					m_handle.keyEquivalent = UIPlatform::getKeyEquivalent(km, keMask);
					m_handle.keyEquivalentModifierMask = keMask;
				} else {
					m_handle.keyEquivalent = @"";
					m_handle.keyEquivalentModifierMask = 0;
				}
			}
			
			void MenuItemImpl::setSecondShortcutKey(const slib::KeycodeAndModifiers &km)
			{
				MenuItem::setSecondShortcutKey(km);
			}
			
			void MenuItemImpl::setEnabled(sl_bool flag)
			{
				MenuItem::setEnabled(flag);
				m_handle.enabled = flag ? YES : NO;
			}
			
			void MenuItemImpl::setChecked(sl_bool flag)
			{
				MenuItem::setChecked(flag);
				m_handle.state = flag ? NSOnState : NSOffState;
			}
			
			void MenuItemImpl::setIcon(const Ref<Bitmap>& icon)
			{
				MenuItem::setIcon(icon);
				m_handle.offStateImage = GraphicsPlatform::createNSImageFromBitmap(icon);
			}
			
			void MenuItemImpl::setCheckedIcon(const Ref<Bitmap>& icon)
			{
				MenuItem::setCheckedIcon(icon);
				m_handle.onStateImage = GraphicsPlatform::createNSImageFromBitmap(icon);
				if (m_handle.onStateImage == nil) {
					m_handle.onStateImage = m_handle->m_defaultCheckedImage;
				}
			}
			
			void MenuItemImpl::setSubmenu(const Ref<Menu>& menu)
			{
				MenuItem::setSubmenu(menu);
				m_handle.submenu = UIPlatform::getMenuHandle(menu);
				if (m_handle.submenu != nil) {
					m_handle.submenu.title = m_handle.title;
				}
			}
			
			NSImage* MenuItemImpl::_createIcon(const Ref<Bitmap>& iconSrc)
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

		}
	}
	
	using namespace priv::menu;

	Ref<Menu> Menu::create()
	{
		return MenuImpl::create();
	}


	NSMenu* UIPlatform::getMenuHandle(const Ref<Menu>& menu)
	{
		if (MenuImpl* _menu = CastInstance<MenuImpl>(menu.get())) {
			return _menu->m_handle;
		}
		return nil;
	}

	NSMenuItem* UIPlatform::getMenuItemHandle(const Ref<MenuItem>& item)
	{
		if (MenuItemImpl* _item = CastInstance<MenuItemImpl>(item.get())) {
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

	sl_bool UIApp::isMenuBarVisible()
	{
		return [NSMenu menuBarVisible] == YES;
	}

	void UIApp::setMenuBarVisible(sl_bool flagVisible)
	{
		[NSMenu setMenuBarVisible:flagVisible ? YES : NO];
	}

	void UIApp::setVisibleOnDock(sl_bool flagVisible)
	{
		ProcessSerialNumber psn = { 0, kCurrentProcess };
		if (flagVisible) {
			TransformProcessType(&psn, kProcessTransformToForegroundApplication);
		} else {
			TransformProcessType(&psn, kProcessTransformToUIElementApplication);
		}
	}

	void UIApp::activate(sl_bool flagIgnoreOtherApps)
	{
		[NSApp activateIgnoringOtherApps:(flagIgnoreOtherApps ? YES : NO)];
	}

}

@implementation SLIBMenuItemHandle

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
