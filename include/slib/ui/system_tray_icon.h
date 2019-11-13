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

#ifndef CHECKHEADER_SLIB_UI_SYSTEM_TRAY_ICON
#define CHECKHEADER_SLIB_UI_SYSTEM_TRAY_ICON

#include "definition.h"

#include "menu.h"
#include "event.h"

namespace slib
{

	class SystemTrayIcon;

	class SLIB_EXPORT SystemTrayIconParam
	{
	public:
		String iconName;
		Ref<Bitmap> icon;
		String toolTip;
		sl_bool flagVisible;
		sl_bool flagHighlight;
		Ref<Menu> menu;
		
		Function<void()> action;
		Function<void(SystemTrayIcon*, UIEvent*)> onMouseEvent;
		
	public:
		SystemTrayIconParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(SystemTrayIconParam)
		
	};

	class SLIB_EXPORT SystemTrayIcon : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		SystemTrayIcon();
		
		~SystemTrayIcon();
		
	public:
		static Ref<SystemTrayIcon> create(const SystemTrayIconParam& param);
		
	public:
		Ref<Bitmap> getIcon();
		
		void setIcon(const Ref<Bitmap>& icon);
		
		String getIconName();
		
		void setIconName(const String& name);
		
		String getToolTip();
		
		void setToolTip(const String& toolTip);
		
		sl_bool isVisible();
		
		void setVisible(sl_bool flagVisible);
		
		Ref<Menu> getMenu();
		
		void setMenu(const Ref<Menu>& menu);
		
		// Screen Coordinate
		virtual UIRect getFrame() = 0;
		
		void showMessage(const String& title, const String& message, const Ref<Bitmap>& icon, sl_uint32 millisecondsTimeout = 0);
		
		void showMessage(const String& title, const String& message, sl_uint32 millisecondsTimeout = 0);
		
	public:
		SLIB_PROPERTY_FUNCTION(void(), Action)

	public:
		void dispatchAction();
		
		void dispatchMouseEvent(UIEvent* ev);
		
	protected:
		Ref<Bitmap> m_icon;
		String m_iconName;
		AtomicString m_toolTip;
		sl_bool m_flagVisible;
		AtomicRef<Menu> m_menu;
		Function<void(SystemTrayIcon*, UIEvent*)> m_onMouseEvent;
		
	protected:
		void _init(const SystemTrayIconParam& param);
		
	protected:
		virtual void setIcon_NI(const Ref<Bitmap>& icon, const String& name) = 0;
		
		virtual void setToolTip_NI(const String& toolTip) = 0;
		
		virtual void setVisible_NI(sl_bool flag) = 0;
		
		virtual void setMenu_NI(const Ref<Menu>& menu) = 0;
		
		virtual void showMessage_NI(const String& title, const String& message, const Ref<Bitmap>& icon, sl_uint32 milliseconsTimeout) = 0;
		
	};

}

#endif
