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

#include "slib/ui/core.h"

namespace slib
{

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(SystemTrayIconParam)

	SystemTrayIconParam::SystemTrayIconParam()
	{
		flagHighlight = sl_true;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(SystemTrayIconNotifyParam)

	SystemTrayIconNotifyParam::SystemTrayIconNotifyParam()
	{
		iconType = SystemTrayIcon::NotifyIcon::None;
		timeout = 5000;
		flagSound = sl_true;
		flagLargeIcon = sl_true;
	}


	SLIB_DEFINE_OBJECT(SystemTrayIcon, Object)

	SystemTrayIcon::SystemTrayIcon()
	{
	}

	SystemTrayIcon::~SystemTrayIcon()
	{
	}

	Ref<Drawable> SystemTrayIcon::getIcon()
	{
		return m_icon;
	}

	void SystemTrayIcon::setIcon(const Ref<Drawable>& icon)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SystemTrayIcon, setIcon, this, icon));
			return;
		}
		m_icon = icon;
		m_iconName.setNull();
		setIcon_NI(icon, sl_null);
	}

	String SystemTrayIcon::getIconName()
	{
		return m_iconName;
	}

	void SystemTrayIcon::setIconName(const String& name)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SystemTrayIcon, setIconName, this, name));
			return;
		}
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
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SystemTrayIcon, setToolTip, this, toolTip));
			return;
		}
		m_toolTip = toolTip;
		setToolTip_NI(toolTip);
	}

	Ref<Menu> SystemTrayIcon::getMenu()
	{
		return m_menu;
	}

	void SystemTrayIcon::setMenu(const Ref<Menu>& menu)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SystemTrayIcon, setMenu, this, menu));
			return;
		}
		m_menu = menu;
		setMenu_NI(menu);
	}

	void SystemTrayIcon::notify(const SystemTrayIconNotifyParam& param)
	{
		if (UI::isUiThread()) {
			notify_NI(param);
		} else {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), SystemTrayIcon, notify_NI, this, param));
		}
	}

	void SystemTrayIcon::notify(const String& title, const String& message)
	{
		SystemTrayIconNotifyParam param;
		param.title = title;
		param.message = message;
		notify(param);
	}

	void SystemTrayIcon::notify(const String& title, const String& message, const Ref<Drawable>& icon)
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

	void SystemTrayIcon::dispatchClick(UIEvent* ev)
	{
		m_onClick(this, ev);
	}

	void SystemTrayIcon::dispatchRightClick(UIEvent* ev)
	{
		m_onRightClick(this, ev);
	}

	void SystemTrayIcon::dispatchKeySelect(UIEvent* ev)
	{
		m_onKeySelect(this, ev);
	}

	void SystemTrayIcon::dispatchEvent(UIEvent* ev)
	{
		m_onEvent(this, ev);
	}

	void SystemTrayIcon::dispatchShowBalloon()
	{
		m_onShowBalloon(this);
	}

	void SystemTrayIcon::dispatchHideBalloon()
	{
		m_onHideBalloon(this);
	}

	void SystemTrayIcon::dispatchClickBalloon()
	{
		m_onClickBalloon(this);
	}

	void SystemTrayIcon::dispatchBalloonTimeout()
	{
		m_onBalloonTimeout(this);
	}

	void SystemTrayIcon::notify_NI(const SystemTrayIconNotifyParam& param)
	{
	}

	void SystemTrayIcon::_init(const SystemTrayIconParam& param)
	{
		if (param.iconName.isNotEmpty()) {
			m_iconName = param.iconName;
		} else {
			m_icon = param.icon;
		}
		m_toolTip = param.toolTip;
		m_flagHighlight = param.flagHighlight;
		m_menu = param.menu;
		
		m_onClick = param.onClick;
		m_onRightClick = param.onRightClick;
		m_onKeySelect = param.onKeySelect;
		m_onEvent = param.onEvent;

		m_onShowBalloon = param.onShowBalloon;
		m_onHideBalloon = param.onHideBalloon;
		m_onClickBalloon = param.onClickBalloon;
		m_onBalloonTimeout = param.onBalloonTimeout;
	}

#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	Ref<SystemTrayIcon> SystemTrayIcon::create(const SystemTrayIconParam& param)
	{
		return sl_null;
	}
#endif

}
