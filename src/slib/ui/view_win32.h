#ifndef CHECKHEADER_SLIB_UI_VIEW_WIN32
#define CHECKHEADER_SLIB_UI_VIEW_WIN32

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/view.h"
#include "../../../inc/slib/ui/platform.h"

//#define _SLIB_UI_WIN32_USE_COMPOSITE_VIEWS
#define _SLIB_UI_WIN32_USE_CLIP_CHILDREN

SLIB_UI_NAMESPACE_BEGIN
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
		return Ref<T>::null();
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

	sl_bool onEventKey(sl_bool flagDown, WPARAM wParam, LPARAM lParam);
	sl_bool onEventMouse(UIEventAction action, WPARAM wParam, LPARAM lParam);
	sl_bool onEventMouseWheel(sl_bool flagVertical, WPARAM wParam, LPARAM lParam);
	sl_bool onEventSetCursor();
	void applyModifiers(UIEvent* ev);

protected:
	HWND m_handle;
	sl_bool m_flagDestroyOnRelease;

	UIEventAction m_actionMouseCapture;

};

class Win32_View_Shared
{
public:
	ATOM wndClass;
	HFONT hFontDefault;


public:
	Win32_View_Shared();

public:
	static Win32_View_Shared* get();
};
SLIB_UI_NAMESPACE_END

#endif

#endif
