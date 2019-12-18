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

#define NTDDI_VERSION NTDDI_WIN7

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/system_tray_icon.h"

#include "slib/core/safe_static.h"
#include "slib/graphics/image.h"
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
						DestroyIcon(m_hIcon);
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

					priv::ui_core::g_wndProc_SystemTrayIcon = &messageProc;

					NOTIFYICONDATAW nid;
					if (!(prepareNID(nid))) {
						return sl_null;
					}

					static sl_int32 uId = 0x1000;
					nid.uID = (UINT)((sl_uint16)(Base::interlockedIncrement32(&uId)));
					sl_bool flagLoadedIcon = sl_false;
					nid.hIcon = createIcon(param.iconName, param.icon, flagLoadedIcon);
					nid.uFlags = NIF_MESSAGE;
					nid.uCallbackMessage = SLIB_UI_MESSAGE_SYSTEM_TRAY_ICON;
					if (nid.hIcon) {
						nid.uFlags |= NIF_ICON;
					}
					if (setTip(nid, param.toolTip)) {
						nid.uFlags |= NIF_TIP | NIF_SHOWTIP;
					}
					if (Shell_NotifyIconW(NIM_ADD, &nid)) {
						nid.uVersion = 4;
						Shell_NotifyIconW(NIM_SETVERSION, &nid);
						Ref<SystemTrayIconImpl> ret = new SystemTrayIconImpl;
						if (ret.isNotNull()) {
							ret->m_id = nid.uID;
							if (!flagLoadedIcon) {
								ret->m_hIcon = nid.hIcon;
							}
							ret->_init(param);
							map->put(nid.uID, ret);
							return ret;
						}
					}
					return sl_null;
				}

			public:
				static HICON createIcon(const String16& name, const Ref<Drawable>& drawable, sl_bool& flagLoadIcon)
				{
					flagLoadIcon = sl_false;
					Win32_UI_Shared* shared = Win32_UI_Shared::get();
					if (!shared) {
						return sl_null;
					}
					if (name.isNotEmpty()) {
						HICON hIcon = LoadIconW(shared->hInstance, (LPCWSTR)(name.getData()));
						if (hIcon) {
							flagLoadIcon = sl_true;
							return hIcon;
						}
						sl_int32 res = 0;
						if (name.parseInt32(10, &res)) {
							hIcon = LoadIconW(shared->hInstance, MAKEINTRESOURCEW(res));
							if (hIcon) {
								flagLoadIcon = sl_true;
								return hIcon;
							}
						}
					}
					if (drawable.isNotNull()) {
						return GraphicsPlatform::createHICON(drawable);
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
							size = NOTIFYICONDATAW_V3_SIZE;
						} else if (ver >= SLIB_MAKE_DWORD(5, 0, 0, 0)) {
							size = NOTIFYICONDATAW_V2_SIZE;
						} else {
							size = NOTIFYICONDATAW_V1_SIZE;
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
						Base::copyMemory(nid.szTip, toolTip.getData(), n);
						return sl_true;
					}
					return sl_false;
				}

				static sl_bool setInfo(NOTIFYICONDATAW& nid, const String16& str)
				{
					sl_size n = str.getLength() << 1;
					if (n > sizeof(nid.szInfo)) {
						n = sizeof(nid.szInfo);
					}
					if (n) {
						Base::copyMemory(nid.szInfo, str.getData(), n);
						return sl_true;
					}
					return sl_false;
				}

				static sl_bool setInfoTitle(NOTIFYICONDATAW& nid, const String16& str)
				{
					sl_size n = str.getLength() << 1;
					if (n > sizeof(nid.szInfoTitle)) {
						n = sizeof(nid.szInfoTitle);
					}
					if (n) {
						Base::copyMemory(nid.szInfoTitle, str.getData(), n);
						return sl_true;
					}
					return sl_false;
				}

				void notify_NI(const SystemTrayIconNotifyParam& param) override
				{
					NOTIFYICONDATAW nid;
					if (!(prepareNID(nid))) {
						return;
					}

					nid.uID = m_id;
					nid.uFlags = NIF_INFO;
					nid.uTimeout = param.timeout;
					setInfo(nid, param.message);
					setInfoTitle(nid, param.title);

					sl_bool flagLoadedIcon = sl_false;
					if (param.iconType == NotifyIcon::Error) {
						nid.dwInfoFlags = NIIF_ERROR;
					} else if (param.iconType == NotifyIcon::Warning) {
						nid.dwInfoFlags = NIIF_WARNING;
					} else if (param.iconType == NotifyIcon::Information) {
						nid.dwInfoFlags = NIIF_INFO;
					} else {
						nid.dwInfoFlags = NIIF_NONE;
						Ref<Drawable> icon = param.icon;
						if (icon.isNotNull()) {
							sl_uint32 w, h;
							if (param.flagLargeIcon) {
								w = GetSystemMetrics(SM_CXICON);
								h = GetSystemMetrics(SM_CYICON);
							} else {
								w = GetSystemMetrics(SM_CXSMICON);
								h = GetSystemMetrics(SM_CYSMICON);
							}
							if ((sl_uint32)(icon->getDrawableWidth()) != w || (sl_uint32)(icon->getDrawableHeight()) != h) {
								Ref<Image> image = icon->toImage();
								if (image.isNotNull()) {
									icon = image->stretch(w, h);
								}
							}
						}
						nid.hBalloonIcon = nid.hIcon = createIcon(param.iconName, icon, flagLoadedIcon);
						if (nid.hBalloonIcon) {
							nid.dwInfoFlags = NIIF_USER;
						}
					}
					if (param.flagLargeIcon) {
						nid.dwInfoFlags |= NIIF_LARGE_ICON;
					}
					if (!(param.flagSound)) {
						nid.dwInfoFlags |= NIIF_NOSOUND;
					}
					Shell_NotifyIconW(NIM_MODIFY, &nid);
					if (!flagLoadedIcon && nid.hBalloonIcon) {
						DestroyIcon(nid.hBalloonIcon);
					}
				}

				void setIcon_NI(const Ref<Drawable>& icon, const String& name) override
				{
					NOTIFYICONDATAW nid;
					if (prepareNID(nid)) {
						nid.uID = m_id;
						sl_bool flagLoadedIcon = sl_false;
						nid.hIcon = createIcon(name, icon, flagLoadedIcon);
						nid.uFlags = NIF_ICON;
						if (Shell_NotifyIconW(NIM_MODIFY, &nid)) {
							if (m_hIcon) {
								DestroyIcon(m_hIcon);
							}
							if (flagLoadedIcon) {
								m_hIcon = sl_null;
							} else {
								m_hIcon = nid.hIcon;
							}
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

				void onMessage(sl_uint32 msg, sl_int32 x, sl_int32 y)
				{
					Win32_UI_Shared* shared = Win32_UI_Shared::get();
					if (!shared) {
						return;
					}
					sl_bool flagNotify = sl_false;
					UIAction action;
					switch (msg) {
					case WM_LBUTTONDOWN:
						action = UIAction::LeftButtonDown;
						break;
					case WM_LBUTTONUP:
						action = UIAction::LeftButtonUp;
						break;
					case WM_LBUTTONDBLCLK:
						action = UIAction::LeftButtonDoubleClick;
						break;
					case WM_RBUTTONDOWN:
						action = UIAction::RightButtonDown;
						break;
					case WM_RBUTTONUP:
						action = UIAction::RightButtonUp;
						break;
					case WM_RBUTTONDBLCLK:
						action = UIAction::RightButtonDoubleClick;
						break;
					case WM_MBUTTONDOWN:
						action = UIAction::MiddleButtonDown;
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
						break;
					case NIN_SELECT:
						action = UIAction::LeftButtonDown;
						flagNotify = sl_true;
						break;
					case NIN_KEYSELECT:
						action = UIAction::KeyDown;
						flagNotify = sl_true;
						break;
					case WM_CONTEXTMENU:
						action = UIAction::RightButtonDown;
						flagNotify = sl_true;
						break;
					case NIN_BALLOONSHOW:
						dispatchShowBalloon();
						return;
					case NIN_BALLOONHIDE:
						dispatchHideBalloon();
						return;
					case NIN_BALLOONUSERCLICK:
						dispatchClickBalloon();
						return;
					case NIN_BALLOONTIMEOUT:
						dispatchBalloonTimeout();
						return;
					default:
						return;
					}
					Time t;
					t.setMillisecondsCount(GetMessageTime());
					Ref<UIEvent> ev;
					if (action == UIAction::KeyDown) {
						ev = UIEvent::createKeyEvent(action, Keycode::Enter, 0, t);
					} else {
						ev = UIEvent::createMouseEvent(action, (sl_ui_posf)x, (sl_ui_posf)y, t);
					}
					if (ev.isNotNull()) {
						UIPlatform::applyEventModifiers(ev.get());
						if (flagNotify) {
							if (action == UIAction::LeftButtonDown) {
								dispatchClick(ev.get());
							} else if (action == UIAction::RightButtonDown) {
								dispatchRightClick(ev.get());
							} else if (action == UIAction::KeyDown) {
								dispatchKeySelect(ev.get());
							}
							if (!(ev->isPreventedDefault())) {
								Ref<Menu> menu = m_menu;
								if (menu.isNotNull()) {
									SetForegroundWindow(shared->hWndMessage);
									menu->show((sl_ui_pos)(x), (sl_ui_pos)(y));
								}
							}
						} else {
							dispatchEvent(ev.get());
						}
					}
				}

				static LRESULT WINAPI messageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					InstanceMap* map = GetInstanceMap();
					if (map) {
						Ref<SystemTrayIcon> instance = map->getValue(HIWORD(lParam));
						if (instance.isNotNull()) {
							((SystemTrayIconImpl*)(instance.get()))->onMessage(LOWORD(lParam), (short)(LOWORD(wParam)), (short)(HIWORD(wParam)));
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
