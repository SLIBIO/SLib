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

#ifndef CHECKHEADER_SLIB_UI_VIEW_WIN32
#define CHECKHEADER_SLIB_UI_VIEW_WIN32

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/view.h"

#include "slib/ui/platform.h"

namespace slib
{

	class Win32_LayeredViewContext;

	class Win32_ViewInstance : public ViewInstance
	{
		SLIB_DECLARE_OBJECT

	public:
		Win32_ViewInstance();
		
		~Win32_ViewInstance();

	public:
		template <class T>
		static Ref<T> create(HWND hWnd, sl_bool flagDestroyOnRelease)
		{
			if (hWnd) {
				Ref<T> ret = new T;
				if (ret.isNotNull()) {
					ret->initialize(hWnd, flagDestroyOnRelease);
					return ret;
				} else {
					if (flagDestroyOnRelease) {
						PostMessageW(hWnd, SLIB_UI_MESSAGE_CLOSE, 0, 0);
					}
				}
			}
			return sl_null;
		}

		template <class T>
		static Ref<T> create(
			View* view, ViewInstance* parent,
			LPCWSTR wndClass, const StringParam& _text,
			int style, int styleEx, int styleRemove = 0)
		{
			String16 text = String16::from(_text);
			UIRect frame = view->getFrameInInstance();
			Matrix3 transform = view->getFinalTransformInInstance();
			HWND handle = createHandle(view, parent, wndClass, (LPCWSTR)(text.getData()), frame, transform, style, styleEx, styleRemove);
			if (handle) {
				Ref<T> ret = new T;
				if (ret.isNotNull()) {
					ret->initialize(handle, view, text, frame, transform);
					return ret;
				}
				DestroyWindow(handle);
			}
			return sl_null;
		}

		static HWND createHandle(View* view, ViewInstance* parent,
			LPCWSTR wndClass, LPCWSTR text,
			const UIRect& rect, const Matrix3& transform,
			int style, int styleEx, int styleRemove);

		void initialize(HWND hWnd, sl_bool flagDestroyOnRelease);

		void initialize(HWND hWnd, View* view, const String16& text, const UIRect& rect, const Matrix3& transform);

		HWND getHandle();

	public:
		virtual LRESULT processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);

		virtual LRESULT processSubclassMessage(UINT msg, WPARAM wParam, LPARAM lParam);

		virtual sl_bool processCommand(SHORT code, LRESULT& result);

		virtual sl_bool processNotify(NMHDR* nmhdr, LRESULT& result);

		virtual sl_bool processControlColor(UINT msg, HDC hDC, HBRUSH& result);

		virtual void processPostControlColor(UINT msg, HDC hDC, HBRUSH& result);

	public:
		sl_bool isValid(View* view) override;

		void setFocus(View* view, sl_bool flag) override;

		void invalidate(View* view) override;

		void invalidate(View* view, const UIRect& rect) override;

		void setFrame(View* view, const UIRect& frame) override;

		void setTransform(View* view, const Matrix3& transform) override;

		void setVisible(View* view, sl_bool flag) override;

		void setEnabled(View* view, sl_bool flag) override;

		void setOpaque(View* view, sl_bool flag) override;

		void setAlpha(View* view, sl_real alpha) override;

		void setClipping(View* view, sl_bool flag) override;

		void setDrawing(View* view, sl_bool flag) override;

		UIPointf convertCoordinateFromScreenToView(View* view, const UIPointf& ptScreen) override;

		UIPointf convertCoordinateFromViewToScreen(View* view, const UIPointf& ptView) override;

		void addChildInstance(View* view, const Ref<ViewInstance>& instance) override;

		void removeChildInstance(View* view, const Ref<ViewInstance>& instance) override;

		void bringToFront(View* view) override;

		void setFont(View* view, const Ref<Font>& font) override;

		void setBorder(View* view, sl_bool flag) override;

		void setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical) override;

	public:
		void setText(const StringParam& text);

		void setLayered(sl_bool flagLayered);

		void updateLayered();

	public:
		void onPaint(Canvas* canvas);

		void onPaint();

		void onDrawLayered();

		sl_bool onEventKey(sl_bool flagDown, WPARAM wParam, LPARAM lParam);

		sl_bool onEventMouse(UIAction action, WPARAM wParam, LPARAM lParam);

		sl_bool onEventMouseWheel(sl_bool flagVertical, WPARAM wParam, LPARAM lParam);

		sl_bool onEventSetCursor();

		void setGenericView(sl_bool flag);

	protected:
		HWND m_handle;
		sl_bool m_flagGenericView;
		sl_bool m_flagDestroyOnRelease;

		UIAction m_actionMouseCapture;
		
		UIRect m_frame;
		Vector2 m_translation;

		String16 m_text;
		Ref<Font> m_font;

		Ref<Win32_LayeredViewContext> m_layered;

	};

	class Win32_LayeredViewContext : public Referable
	{
	public:
		sl_bool flagInvalidated;

		HDC hdcCache;
		HBITMAP hbmCache;
		HGDIOBJ hbmOld;
		Gdiplus::Graphics* graphicsCache;
		Gdiplus::Bitmap* bitmapCache;
		sl_uint32 widthCache;
		sl_uint32 heightCache;

	public:
		Win32_LayeredViewContext();

		~Win32_LayeredViewContext();

	public:
		sl_bool prepare(sl_uint32 width, sl_uint32 height);

		void clear();

		void sync(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);

	};

}

#endif

#endif
