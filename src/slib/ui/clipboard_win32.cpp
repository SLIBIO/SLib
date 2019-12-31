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

#include "slib/ui/clipboard.h"

#if defined(SLIB_UI_IS_WIN32)

#include "slib/core/safe_static.h"
#include "slib/core/platform_windows.h"

namespace slib
{

	namespace priv
	{
		namespace clipboard
		{

			LRESULT CALLBACK OwnerWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
			{
				switch (message) {
				case WM_RENDERFORMAT:
				case WM_RENDERALLFORMATS:
				case WM_DRAWCLIPBOARD:
				case WM_DESTROY:
				case WM_CHANGECBCHAIN:
					return 0;
				}
				return DefWindowProc(hwnd, message, wparam, lparam);
			}

			class ClipboardManager
			{
			public:
				HWND owner;

			public:
				ClipboardManager()
				{
					WNDCLASSEXW wc;
					Base::zeroMemory(&wc, sizeof(wc));
					wc.cbSize = sizeof(wc);
					wc.lpszClassName = L"SLibClipboardOwner";
					wc.lpfnWndProc = OwnerWndProc;
					wc.hInstance = GetModuleHandle(NULL);
					RegisterClassEx(&wc);
					owner = CreateWindowW(L"SLibClipboardOwner", L"", 0, 0, 0, 0, 0, HWND_MESSAGE, 0, 0, 0);
				}

				~ClipboardManager()
				{
					DestroyWindow(owner);
					owner = NULL;
				}

			};

			SLIB_SAFE_STATIC_GETTER(ClipboardManager, GetClipboardManager)

			class ClipboardScope
			{
			public:
				BOOL bOpened;

			public:
				ClipboardScope()
				{
					bOpened = FALSE;
					ClipboardManager* manager = GetClipboardManager();
					if (manager) {
						if (manager->owner) {
							bOpened = OpenClipboard(manager->owner);
						}
					}
				}

				~ClipboardScope()
				{
					if (bOpened) {
						ClipboardManager* manager = GetClipboardManager();
						if (manager) {
							if (manager->owner) {
								CloseClipboard();
							}
						}
					}
				}

			};

		}
	}

	using namespace priv::clipboard;

	sl_bool Clipboard::hasText()
	{
		ClipboardScope scope;
		return IsClipboardFormatAvailable(CF_UNICODETEXT) ? sl_true : sl_false;
	}
	
	String Clipboard::getText()
	{
		ClipboardScope scope;
		HANDLE data = GetClipboardData(CF_UNICODETEXT);
		if (data) {
			void* src = GlobalLock(data);
			if (src) {
				String ret = String::create((sl_char16*)src);
				GlobalUnlock(src);
				return ret;
			}
		}
		return sl_null;
	}
	
	void Clipboard::setText(const StringParam& _text)
	{
		ClipboardScope scope;
		EmptyClipboard();
		StringCstr16 text(_text);
		HGLOBAL handle = Windows::createGlobalData(text.getData(), (text.getLength() + 1) << 1);
		if (handle) {
			SetClipboardData(CF_UNICODETEXT, handle);
			GlobalFree(handle);
		}
	}

}

#endif