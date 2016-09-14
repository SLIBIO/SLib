#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/split_view.h"
#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN

class _SplitView : public SplitView
{
public:
	sl_bool processMessage(Win32_ViewInstance* instance, HWND hWnd, MSG& msg)
	{
		DWORD lParam = ::GetMessagePos();
		POINT pt;
		pt.x = (short)(lParam & 0xffff);
		pt.y = (short)((lParam >> 16) & 0xffff);
		::ScreenToClient(hWnd, &pt);
		sl_int32 index = _getDividerIndexAtPoint(UIPoint((sl_ui_pos)(pt.x), (sl_ui_pos)(pt.y)));
		if (index < 0) {
			return sl_false;
		}
		Ref<UIEvent> ev;
		if (msg.message == WM_LBUTTONDOWN) {
			lParam = POINTTOPOINTS(pt);
			LRESULT res;
			instance->processWindowMessage(WM_LBUTTONDOWN, msg.wParam, lParam, res);
		} else if (msg.message == WM_NCLBUTTONDOWN) {
			lParam = POINTTOPOINTS(pt);
			LRESULT res;
			instance->processWindowMessage(WM_LBUTTONDOWN, 0, lParam, res);
		}
		return sl_true;
	}
};

class _SplitViews
{
public:
	CList<HWND> handles;

};

SLIB_SAFE_STATIC_GETTER(_SplitViews, _SplitViews_get)

Ref<ViewInstance> SplitView::createGenericInstance(ViewInstance* parent)
{
	Ref<ViewInstance> instance = View::createGenericInstance(parent);
	_SplitViews* views = _SplitViews_get();
	if (views) {
		HWND hWnd = UIPlatform::getViewHandle(instance.ptr);
		if (hWnd) {
			views->handles.addIfNotExist(hWnd);
		}
	}
	return instance;
}

void _SplitView_unregisterWin32Handle(View* view)
{
	_SplitViews* views = _SplitViews_get();
	if (views) {
		HWND hWnd = UIPlatform::getViewHandle(view);
		if (hWnd) {
			views->handles.removeValue(hWnd);
		}
	}
}

sl_bool _SplitView_preprocessMessage(MSG& msg)
{
	if (msg.message != WM_LBUTTONDOWN && msg.message != WM_NCLBUTTONDOWN) {
		return sl_false;
	}
	_SplitViews* views = _SplitViews_get();
	if (views) {
		if (views->handles.getCount() > 0) {
			ListLocker<HWND> handles(views->handles);
			HWND hWnd = Windows::getParentWindow(msg.hwnd);
			while (hWnd) {
				for (sl_size i = 0; i < handles.count; i++) {
					if (hWnd == handles[i]) {
						Ref<ViewInstance> instance = UIPlatform::getViewInstance(hWnd);
						if (instance.isNotNull()) {
							Ref<View> view = instance->getView();
							if (SplitView::checkInstance(view.ptr)) {
								_SplitView* sv = (_SplitView*)(view.ptr);
								return sv->processMessage((Win32_ViewInstance*)(instance.ptr), hWnd, msg);
							}
						}
						return sl_false;
					}
				}
				hWnd = Windows::getParentWindow(hWnd);
			}
		}
	}
	return sl_false;
}

SLIB_UI_NAMESPACE_END

#endif
