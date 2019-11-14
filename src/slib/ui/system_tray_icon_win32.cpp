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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/system_tray_icon.h"

#include "slib/core/safe_static.h"
#include "slib/ui/platform.h"

#include "ui_core_win32.h"

namespace slib
{

	namespace priv
	{

		namespace ui_core
		{
			extern WNDPROC g_wndProc_SystemTrayIcon;
		}

		namespace system_tray_icon
		{

			typedef HashMap< UINT, WeakRef<SystemTrayIcon> > InstanceMap;

			SLIB_SAFE_STATIC_GETTER(InstanceMap, GetInstanceMap)

			class SystemTrayIconImpl : public SystemTrayIcon
			{
			public:
				UINT m_id;
				HICON m_hIcon;

			public:
				SystemTrayIconImpl()
				{
					m_id = 0;
					m_hIcon = sl_null;
				}

				~SystemTrayIconImpl()
				{
					NOTIFYICONDATAW nid;
					if (prepareNID(nid)) {
						nid.uID = m_id;
						Shell_NotifyIconW(NIM_DELETE, &nid);
					}
					if (m_hIcon) {
						DeleteObject(m_hIcon);
					}
					InstanceMap* map = GetInstanceMap();
					if (map) {
						map->remove(m_id);
					}
				}

			public:
				static Ref<SystemTrayIconImpl> create(const SystemTrayIconParam& param)
				{
					InstanceMap* map = GetInstanceMap();
					if (!map) {
						return sl_null;
					}
					NOTIFYICONDATAW nid;
					if (!(prepareNID(nid))) {
						return sl_null;
					}

					priv::ui_core::g_wndProc_SystemTrayIcon = &messageProc;

					static sl_int32 uId = 0x1000;
					nid.uID = (UINT)(Base::interlockedIncrement32(&uId));
					nid.hIcon = createIcon(param.iconName, param.icon);
					nid.uFlags = NIF_MESSAGE;
					nid.uCallbackMessage = SLIB_UI_MESSAGE_SYSTEM_TRAY_ICON;
					if (nid.hIcon) {
						nid.uFlags |= NIF_ICON;
					}
					if (setTip(nid, param.toolTip)) {
						nid.uFlags |= NIF_TIP;
					}
					if (Shell_NotifyIconW(NIM_ADD, &nid)) {
						Ref<SystemTrayIconImpl> ret = new SystemTrayIconImpl;
						if (ret.isNotNull()) {
							ret->m_id = nid.uID;
							ret->m_hIcon = nid.hIcon;
							ret->_init(param);
							map->put(nid.uID, ret);
							return ret;
						}
					}
					return sl_null;
				}

			public:
				static HICON createIcon(const String16& name, const Ref<Bitmap>& bitmap)
				{
					Win32_UI_Shared* shared = Win32_UI_Shared::get();
					if (!shared) {
						return sl_null;
					}
					if (name.isNotEmpty()) {
						HICON hIcon = LoadIconW(shared->hInstance, (LPCWSTR)(name.getData()));
						if (hIcon) {
							return hIcon;
						}
					}
					if (bitmap.isNotNull()) {
						return GraphicsPlatform::createIconFromBitmap(bitmap);
					}
					return sl_null;
				}

				static sl_bool prepareNID(NOTIFYICONDATAW& nid)
				{
					Win32_UI_Shared* shared = Win32_UI_Shared::get();
					if (!shared) {
						return sl_false;
					}
					Base::zeroMemory(&nid, sizeof(nid));
					nid.hWnd = shared->hWndMessage;
					static sl_uint32 size = 0;
					if (!size) {
						SLIB_STATIC_STRING16(dllName, "Shell32.dll")
						WindowsDllVersion dllVer = Windows::getDllVersion(dllName);
						sl_uint32 ver = (dllVer.major << 24) | (dllVer.minor << 16) | (dllVer.build);
						if (ver >= SLIB_MAKE_DWORD(6, 0, 0, 6)) {
							size = sizeof(nid);
						} else if (ver >= SLIB_MAKE_DWORD(6, 0, 0, 0)) {
							size = NOTIFYICONDATA_V3_SIZE;
						} else if (ver >= SLIB_MAKE_DWORD(5, 0, 0, 0)) {
							size = NOTIFYICONDATA_V2_SIZE;
						} else {
							size = NOTIFYICONDATA_V1_SIZE;
						}
					}
					nid.cbSize = size;
					return sl_true;
				}

				static sl_bool setTip(NOTIFYICONDATAW& nid, const String16& toolTip)
				{
					sl_size n = toolTip.getLength() << 1;
					if (n > sizeof(nid.szTip)) {
						n = sizeof(nid.szTip);
					}
					if (n) {
						Base::copyString2((sl_char16*)(nid.szTip), toolTip.getData(), n);
						return sl_true;
					}
					return sl_false;
				}

				void setIcon_NI(const Ref<Bitmap>& icon, const String& name) override
				{
					NOTIFYICONDATAW nid;
					if (prepareNID(nid)) {
						nid.uID = m_id;
						nid.hIcon = createIcon(name, icon);
						nid.uFlags = NIF_ICON;
						if (Shell_NotifyIconW(NIM_MODIFY, &nid)) {
							if (m_hIcon) {
								DeleteObject(m_hIcon);
							}
							m_hIcon = nid.hIcon;
						}
					}					
				}

				void setToolTip_NI(const String& toolTip) override
				{
					NOTIFYICONDATAW nid;
					if (prepareNID(nid)) {
						nid.uID = m_id;
						setTip(nid, toolTip);
						nid.uFlags = NIF_TIP;
						Shell_NotifyIconW(NIM_MODIFY, &nid);
					}
				}

				void setMenu_NI(const Ref<Menu>& menu) override
				{
				}

				void onMessage(LPARAM lParam)
				{
					sl_bool flagAction = sl_false;
					UIAction action;
					switch (lParam) {
					case WM_LBUTTONDOWN:
						action = UIAction::LeftButtonDown;
						flagAction = sl_true;
						break;
					case WM_LBUTTONUP:
						action = UIAction::LeftButtonUp;
						break;
					case WM_LBUTTONDBLCLK:
						action = UIAction::LeftButtonDoubleClick;
						break;
					case WM_RBUTTONDOWN:
						action = UIAction::RightButtonDown;
						flagAction = sl_true;
						break;
					case WM_RBUTTONUP:
						action = UIAction::RightButtonUp;
						break;
					case WM_RBUTTONDBLCLK:
						action = UIAction::RightButtonDoubleClick;
						break;
					case WM_MBUTTONDOWN:
						action = UIAction::MiddleButtonDown;
						flagAction = sl_true;
						break;
					case WM_MBUTTONUP:
						action = UIAction::MiddleButtonUp;
						break;
					case WM_MBUTTONDBLCLK:
						action = UIAction::MiddleButtonDoubleClick;
						break;
					case WM_MOUSEMOVE:
						if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
							action = UIAction::LeftButtonDrag;
						} else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
							action = UIAction::RightButtonDrag;
						} else if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
							action = UIAction::MiddleButtonDrag;
						} else {
							action = UIAction::MouseMove;
						}
					default:
						return;
					}
					Time t;
					t.setMillisecondsCount(GetMessageTime());
					POINT pt;
					GetCursorPos(&pt);
					sl_ui_posf x = (sl_ui_posf)(pt.x);
					sl_ui_posf y = (sl_ui_posf)(pt.y);
					Ref<UIEvent> ev = UIEvent::createMouseEvent(action, x, y, t);
					if (ev.isNotNull()) {
						UIPlatform::applyEventModifiers(ev.get());
						dispatchEvent(ev.get());
						if (ev->isPreventedDefault()) {
							return;
						}
						if (flagAction) {
							Ref<Menu> menu = m_menu;
							if (menu.isNotNull()) {
								menu->show((sl_ui_pos)(pt.x), (sl_ui_pos)(pt.y));
							}
						}
					}
				}

				static LRESULT messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					InstanceMap* map = GetInstanceMap();
					if (map) {
						Ref<SystemTrayIcon> instance = map->getValue((UINT)wParam);
						if (instance.isNotNull()) {
							((SystemTrayIconImpl*)(instance.get()))->onMessage(lParam);
						}
					}
					return 0;
				}

			};

		}
	}

	using namespace priv::system_tray_icon;

	Ref<SystemTrayIcon> SystemTrayIcon::create(const SystemTrayIconParam& param)
	{
		return Ref<SystemTrayIcon>::from(SystemTrayIconImpl::create(param));
	}

}

#endif
