/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_VIEW_WIN32
#define CHECKHEADER_SLIB_UI_VIEW_WIN32

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/ui/view.h"

#include "ui_core_win32.h"

namespace slib
{

	class Win32_ViewInstance : public ViewInstance
	{
	public:
		Win32_ViewInstance();
		
		~Win32_ViewInstance();

	public:
		template <class T>
		static Ref<T> create(HWND hWnd, sl_bool flagDestroyOnRelease)
		{
			Ref<T> ret;
			if (hWnd) {
				ret = new T;
				if (ret.isNotNull()) {
					ret->m_handle = hWnd;
					ret->m_flagDestroyOnRelease = flagDestroyOnRelease;
					UIPlatform::registerViewInstance(hWnd, ret.get());
				} else {
					if (flagDestroyOnRelease) {
						::PostMessageW(hWnd, SLIB_UI_MESSAGE_CLOSE, 0, 0);
					}
				}
			}
			return ret;
		}

		template <class T>
		static Ref<T> create(
			View* view, ViewInstance* parent
			, LPCWSTR wndClass, LPCWSTR text
			, int style, int styleEx)
		{
			HWND handle = createHandle(view, parent, wndClass, text, style, styleEx);
			if (handle) {
				return create<T>(handle, sl_true);
			}
			return sl_null;
		}

		static HWND createHandle(View* view, ViewInstance* parent
			, LPCWSTR wndClass, LPCWSTR text
			, int style, int styleEx);

		HWND getHandle();

	public:
		virtual sl_bool preprocessWindowMessage(MSG& msg);

		virtual sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result);

		virtual sl_bool processCommand(SHORT code, LRESULT& result);

		virtual sl_bool processNotify(NMHDR* nmhdr, LRESULT& result);

		virtual sl_bool processControlColor(UINT msg, HDC hDC, HBRUSH& result);

		virtual void processPostControlColor(UINT msg, HDC hDC, HBRUSH& result);

	public:
		// override
		sl_bool isValid();

		// override
		void setFocus();

		// override
		void invalidate();

		// override
		void invalidate(const UIRect& rect);

		// override
		UIRect getFrame();

		// override
		void setFrame(const UIRect& frame);

		// override
		void setTransform(const Matrix3& transform);

		// override
		void setVisible(sl_bool flag);

		// override
		void setEnabled(sl_bool flag);

		// override
		void setOpaque(sl_bool flag);

		// override
		void setAlpha(sl_real alpha);

		// override
		void setClipping(sl_bool flag);

		// override
		void setDrawing(sl_bool flag);

		// override
		UIPointf convertCoordinateFromScreenToView(const UIPointf& ptScreen);

		// override
		UIPointf convertCoordinateFromViewToScreen(const UIPointf& ptView);

		// override
		void addChildInstance(const Ref<ViewInstance>& instance);

		// override
		void removeChildInstance(const Ref<ViewInstance>& instance);

		// override
		void bringToFront();

	public:
		sl_bool onEventKey(sl_bool flagDown, WPARAM wParam, LPARAM lParam);

		sl_bool onEventMouse(UIAction action, WPARAM wParam, LPARAM lParam);

		sl_bool onEventMouseWheel(sl_bool flagVertical, WPARAM wParam, LPARAM lParam);

		sl_bool onEventSetCursor();

		void applyModifiers(UIEvent* ev);

	protected:
		HWND m_handle;
		sl_bool m_flagDestroyOnRelease;

		UIAction m_actionMouseCapture;

	};

}

#endif

#endif
