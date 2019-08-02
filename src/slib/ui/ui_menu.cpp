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

#include "slib/ui/menu.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(MenuItem, Object)
	
	MenuItem::MenuItem()
	{
		m_flagEnabled = sl_true;
		m_flagChecked = sl_false;
	}
	
	MenuItem::~MenuItem()
	{
	}

	Ref<Menu> MenuItem::getParent() const
	{
		return m_parent;
	}
	
	String MenuItem::getText() const
	{
		return m_text;
	}
	
	void MenuItem::setText(const String& text)
	{
		m_text = text;
	}
	
	const KeycodeAndModifiers& MenuItem::getShortcutKey() const
	{
		return m_shortcutKey;
	}
	
	void MenuItem::setShortcutKey(const KeycodeAndModifiers& km)
	{
		m_shortcutKey = km;
	}
	
	const KeycodeAndModifiers& MenuItem::getSecondShortcutKey() const
	{
		return m_secondShortcutKey;
	}
	
	void MenuItem::setSecondShortcutKey(const KeycodeAndModifiers& km)
	{
		m_secondShortcutKey = km;
	}
	
	sl_bool MenuItem::isEnabled() const
	{
		return m_flagEnabled;
	}
	
	void MenuItem::setEnabled(sl_bool flag)
	{
		m_flagEnabled = flag;
	}
	
	sl_bool MenuItem::isChecked() const
	{
		return m_flagChecked;
	}
	
	void MenuItem::setChecked(sl_bool flag)
	{
		m_flagChecked = flag;
	}
	
	Ref<Bitmap> MenuItem::getIcon() const
	{
		return m_icon;
	}
	
	void MenuItem::setIcon(const Ref<Bitmap>& icon)
	{
		m_icon = icon;
	}
	
	Ref<Bitmap> MenuItem::getCheckedIcon() const
	{
		return m_checkedIcon;
	}
	
	void MenuItem::setCheckedIcon(const Ref<Bitmap>& icon)
	{
		m_checkedIcon = icon;
	}
	
	Ref<Menu> MenuItem::getSubmenu() const
	{
		return m_submenu;
	}
	
	void MenuItem::setSubmenu(const Ref<Menu>& menu)
	{
		m_submenu = menu;
	}
	
	sl_bool MenuItem::isSeparator() const
	{
		return sl_false;
	}
	
	namespace priv
	{
		namespace menu
		{
			class Separator : public MenuItem
			{
			public:
				sl_bool isSeparator() const override
				{
					return sl_true;
				}				
			};
		}
	}

	Ref<MenuItem> MenuItem::createSeparator()
	{
		return new priv::menu::Separator;
	}
	
	sl_bool MenuItem::processShortcutKey(const KeycodeAndModifiers& km)
	{
		if (km != 0) {
			if (km == m_shortcutKey) {
				getAction()();
				return sl_true;
			}
			Ref<Menu> submenu = m_submenu;
			if (submenu.isNotNull()) {
				return submenu->processShortcutKey(km);
			}
		}
		return sl_false;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(MenuItemParam)
	
	MenuItemParam::MenuItemParam()
	{
		flagEnabled = sl_true;
		flagChecked = sl_false;
	}
	
	
	SLIB_DEFINE_OBJECT(Menu, Object)
	
	Menu::Menu()
	{
	}
	
	Menu::~Menu()
	{
	}

	sl_uint32 Menu::getMenuItemsCount() const
	{
		return (sl_uint32)(m_items.getCount());
	}
	
	Ref<MenuItem> Menu::getMenuItem(sl_uint32 index) const
	{
		return m_items.getValueAt(index);
	}
	
	void Menu::show(const UIPoint& pt)
	{
		show(pt.x, pt.y);
	}
	
	Ref<MenuItem> Menu::addMenuItem(const String& title)
	{
		MenuItemParam param;
		param.text = title;
		return addMenuItem(param);
	}
	
	Ref<MenuItem> Menu::addMenuItem(const String& title, const Ref<Bitmap>& icon)
	{
		MenuItemParam param;
		param.text = title;
		param.icon = icon;
		return addMenuItem(param);
	}
	
	Ref<MenuItem> Menu::addMenuItem(const String& title, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon)
	{
		MenuItemParam param;
		param.text = title;
		param.icon = icon;
		param.checkedIcon = checkedIcon;
		return addMenuItem(param);
	}
	
	Ref<MenuItem> Menu::addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey)
	{
		MenuItemParam param;
		param.text = title;
		param.shortcutKey = shortcutKey;
		return addMenuItem(param);
	}
	
	Ref<MenuItem> Menu::addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey, const Ref<Bitmap>& icon)
	{
		MenuItemParam param;
		param.text = title;
		param.shortcutKey = shortcutKey;
		param.icon = icon;
		return addMenuItem(param);
	}
	
	Ref<MenuItem> Menu::addMenuItem(const String& title, const KeycodeAndModifiers& shortcutKey, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon)
	{
		MenuItemParam param;
		param.text = title;
		param.shortcutKey = shortcutKey;
		param.icon = icon;
		param.checkedIcon = checkedIcon;
		return addMenuItem(param);
	}
	
	Ref<MenuItem> Menu::addSubmenu(Ref<Menu>& submenu, const String& title)
	{
		MenuItemParam param;
		param.text = title;
		param.submenu = submenu;
		return addMenuItem(param);
	}
	
	Ref<MenuItem> Menu::addSubmenu(Ref<Menu>& submenu, const String& title, const Ref<Bitmap>& icon)
	{
		MenuItemParam param;
		param.text = title;
		param.icon = icon;
		param.submenu = submenu;
		return addMenuItem(param);
	}
	
	Ref<MenuItem> Menu::addSubmenu(Ref<Menu>& submenu, const String& title, const Ref<Bitmap>& icon, const Ref<Bitmap>& checkedIcon)
	{
		MenuItemParam param;
		param.text = title;
		param.icon = icon;
		param.checkedIcon = checkedIcon;
		param.submenu = submenu;
		return addMenuItem(param);
	}
	
	sl_bool Menu::processShortcutKey(const KeycodeAndModifiers& km)
	{
		if (km != 0) {
			ListLocker< Ref<MenuItem> > items(m_items);
			for (sl_size i = 0; i < items.count; i++) {
				Ref<MenuItem>& item = items[i];
				if (item.isNotNull()) {
					if (item->processShortcutKey(km)) {
						return sl_true;
					}
				}
			}
		}
		return sl_false;
	}
}

#if defined(SLIB_PLATFORM_IS_MOBILE)

#include "slib/ui/core.h"
#include "slib/ui/mobile_app.h"
#include "slib/ui/button.h"
#include "slib/ui/linear_view.h"

#include "../resources.h"

namespace slib
{

	namespace priv
	{
		namespace menu
		{
			
			class DrawnMenu;
			
			class DrawnMenuItem : public MenuItem
			{
			public:
				Ref<Button> m_button;
				
			public:
				void setText(const String& text) override
				{
					m_button->setText(text);
				}
				
				void setEnabled(sl_bool flag) override
				{
					m_button->setEnabled(flag);
				}
				
				void setChecked(sl_bool flag) override
				{
					m_button->setCurrentCategory(flag ? 1 : 0);
				}
				
				void setIcon(const Ref<Bitmap>& icon) override
				{
					m_button->setIcon(icon);
				}
				
				void setCheckedIcon(const Ref<Bitmap>& icon) override
				{
					m_button->setIcon(icon, ButtonState::Normal, 1);
				}
				
			};
				
			class DrawnMenu : public Menu
			{
			public:
				Ref<ui::MenuPopup> m_view;
				
			public:
				static Ref<DrawnMenu> create()
				{
					Ref<ui::MenuPopup> view = new ui::MenuPopup;
					if (view.isNotNull()) {
						view->setOnTouchEvent([](View*, UIEvent* ev) {
							ev->stopPropagation();
						});
						Ref<DrawnMenu> ret = new DrawnMenu();
						if (ret.isNotNull()) {
							ret->m_view = view;
							return ret;
						}
					}
					return sl_null;
				}
				
				Ref<MenuItem> addMenuItem(const MenuItemParam& param) override
				{
					Ref<Button> button = new Button;
					button->setWidthWrapping(UIUpdateMode::Init);
					button->setHeightWrapping(UIUpdateMode::Init);
					button->setFontSize(UIResource::dpToPixel(20), UIUpdateMode::Init);
					button->setTextColor(Color::Black, ButtonState::Normal, 0, UIUpdateMode::Init);
					button->setText(param.text, UIUpdateMode::Init);
					button->setPadding(UIResource::dpToPixel(4), UIUpdateMode::Init);
					if (param.icon.isNotNull()) {
						button->setIcon(param.icon, UIUpdateMode::Init);
					}
					if (param.checkedIcon.isNotNull()) {
						button->setIcon(param.icon, ButtonState::Normal, 1, UIUpdateMode::Init);
					}
					if (param.flagChecked) {
						button->setCurrentCategory(1, UIUpdateMode::Init);
					}
					if (!(param.flagEnabled)) {
						button->setEnabled(sl_false, UIUpdateMode::Init);
					}
					Ref<DrawnMenuItem> item = new DrawnMenuItem;
					item->m_button = button;
					item->setAction(param.action);
					DrawnMenuItem* pItem = item.get();
					button->setOnClick([pItem, this](View*) {
						pItem->getAction()();
						Ref<View> back = m_view->getParent();
						if (back.isNotNull()) {
							back->removeFromParent();
						}
					});
					ObjectLocker lock(this);
					m_view->container->addChild(button);
					m_items.add(item);
					return item;
				}
				
				Ref<MenuItem> insertMenuItem(sl_uint32 index, const MenuItemParam& param) override
				{
					return sl_null;
				}
				
				Ref<MenuItem> addSeparator() override
				{
					Ref<MenuItem> item = MenuItem::createSeparator();
					Ref<View> border = new View;
					border->setWidthFilling(1, UIUpdateMode::Init);
					border->setHeight(UIResource::dpToPixel(1), UIUpdateMode::Init);
					border->setBackgroundColor(Color::Gray, UIUpdateMode::Init);
					ObjectLocker lock(this);
					m_view->container->addChild(border);
					m_items.add(item);
					return item;
				}
				
				Ref<MenuItem> insertSeparator(sl_uint32 index) override
				{
					return sl_null;
				}
				
				void removeMenuItem(sl_uint32 index) override
				{
				}
				
				void removeMenuItem(const Ref<MenuItem>& item) override
				{
				}
				
				void show(sl_ui_pos x, sl_ui_pos y) override
				{
					if (!(UI::isUiThread())) {
						UI::dispatchToUiThreadUrgently(SLIB_BIND_WEAKREF(void(), DrawnMenu, show, this, x, y));
						return;
					}
					Ref<MobileApp> app = MobileApp::getApp();
					if (app.isNotNull()) {
						Ref<View> parent = app->getContentView();
						if (parent.isNotNull()) {
							Ref<ViewGroup> back = new ViewGroup;
							back->setCreatingInstance(sl_true);
							back->setWidthFilling(1, UIUpdateMode::Init);
							back->setHeightFilling(1, UIUpdateMode::Init);
							back->setOnTouchEvent([](View* view, UIEvent* ev) {
								ev->stopPropagation();
								view->removeFromParent();
							});
							m_view->forceUpdateLayout();
							if (x > m_view->getLayoutWidth()) {
								x = x - m_view->getLayoutWidth();
							}
							if (y > m_view->getLayoutHeight()) {
								y = y - m_view->getLayoutHeight();
							}
							m_view->setLocation(x, y, UIUpdateMode::Init);
							back->addChild(m_view, UIUpdateMode::Init);
							parent->addChild(back);
						}
					}
				}
				
			};

		}
	}

	Ref<Menu> Menu::create()
	{
		return Ref<Menu>::from(priv::menu::DrawnMenu::create());
	}

}

#endif