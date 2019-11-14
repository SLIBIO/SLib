/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/ui/system_tray_icon.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(SystemTrayIconParam)

	SystemTrayIconParam::SystemTrayIconParam()
	{
		flagVisible = sl_true;
		flagHighlight = sl_true;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(SystemTrayIconNotifyParam)

	SystemTrayIconNotifyParam::SystemTrayIconNotifyParam()
	{
		iconType = SystemTrayIcon::NotifyIcon::None;
	}


	SLIB_DEFINE_OBJECT(SystemTrayIcon, Object)

	SystemTrayIcon::SystemTrayIcon()
	{
		m_flagVisible = sl_true;
	}

	SystemTrayIcon::~SystemTrayIcon()
	{
	}

	Ref<Bitmap> SystemTrayIcon::getIcon()
	{
		ObjectLocker lock(this);
		return m_icon;
	}

	void SystemTrayIcon::setIcon(const Ref<Bitmap>& icon)
	{
		ObjectLocker lock(this);
		m_icon = icon;
		m_iconName.setNull();
		setIcon_NI(icon, sl_null);
	}

	String SystemTrayIcon::getIconName()
	{
		ObjectLocker lock(this);
		return m_iconName;
	}

	void SystemTrayIcon::setIconName(const String& name)
	{
		ObjectLocker lock(this);
		m_iconName = name;
		m_icon.setNull();
		setIcon_NI(sl_null, name);
	}

	String SystemTrayIcon::getToolTip()
	{
		return m_toolTip;
	}

	void SystemTrayIcon::setToolTip(const String& toolTip)
	{
		m_toolTip = toolTip;
		setToolTip_NI(toolTip);
	}

	sl_bool SystemTrayIcon::isVisible()
	{
		return m_flagVisible;
	}

	void SystemTrayIcon::setVisible(sl_bool flagVisible)
	{
		m_flagVisible = flagVisible;
		setVisible_NI(flagVisible);
	}

	Ref<Menu> SystemTrayIcon::getMenu()
	{
		return m_menu;
	}

	void SystemTrayIcon::setMenu(const Ref<Menu>& menu)
	{
		m_menu = menu;
		setMenu_NI(menu);
	}

	void SystemTrayIcon::notify(const SystemTrayIconNotifyParam& param)
	{
	}

	void SystemTrayIcon::notify(const String& title, const String& message)
	{
		SystemTrayIconNotifyParam param;
		param.title = title;
		param.message = message;
		notify(param);
	}

	void SystemTrayIcon::notify(const String& title, const String& message, const Ref<Bitmap>& icon)
	{
		SystemTrayIconNotifyParam param;
		param.title = title;
		param.message = message;
		param.icon = icon;
		notify(param);
	}

	void SystemTrayIcon::notify(const String& title, const String& message, const String& iconName)
	{
		SystemTrayIconNotifyParam param;
		param.title = title;
		param.message = message;
		param.iconName = iconName;
		notify(param);
	}

	void SystemTrayIcon::notify(const String& title, const String& message, SystemTrayIcon::NotifyIcon icon)
	{
		SystemTrayIconNotifyParam param;
		param.title = title;
		param.message = message;
		param.iconType = icon;
		notify(param);
	}

	void SystemTrayIcon::dispatchEvent(UIEvent* ev)
	{
		m_onEvent(this, ev);
		if (ev->isPreventedDefault()) {
			return;
		}
		switch (ev->getAction()) {
			case UIAction::LeftButtonDown:
			case UIAction::RightButtonDown:
			case UIAction::MiddleButtonDown:
			case UIAction::LeftButtonDoubleClick:
			case UIAction::RightButtonDoubleClick:
			case UIAction::MiddleButtonDoubleClick:
			case UIAction::KeyDown:
			case UIAction::Unknown:
				m_onAction(this, ev);
				break;
			default:
				break;
		}
	}

	void SystemTrayIcon::_init(const SystemTrayIconParam& param)
	{
		if (param.iconName.isNotEmpty()) {
			m_iconName = param.iconName;
		} else {
			m_icon = param.icon;
		}
		m_toolTip = param.toolTip;
		m_flagVisible = param.flagVisible;
		m_flagHighlight = param.flagHighlight;
		m_menu = param.menu;
		
		m_onAction = param.onAction;
		m_onEvent = param.onEvent;
	}

#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	Ref<SystemTrayIcon> SystemTrayIcon::create(const SystemTrayIconParam& param)
	{
		return sl_null;
	}
#endif

}
