#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/drop_down_list.h"

#include "view_win32.h"

SLIB_UI_NAMESPACE_BEGIN

class _DropDownList : public DropDownList
{
public:
	void __applyItemsCount(HWND hWnd)
	{
		ObjectLocker lock(this);
		sl_uint32 nOrig = (sl_uint32)(::SendMessageW(hWnd, CB_GETCOUNT, 0, 0));
		sl_uint32 nNew = (sl_uint32)(m_titles.getCount());
		if (nOrig == nNew) {
			return;
		}
		if (nOrig > nNew) {
			if (nNew > 0) {
				for (sl_uint32 i = nOrig; i > nNew; i--) {
					::SendMessageW(hWnd, CB_DELETESTRING, (WPARAM)(i - 1), 0);
				}
			} else {
				::SendMessageW(hWnd, CB_RESETCONTENT, 0, 0);
			}
		} else {
			for (sl_uint32 i = nOrig; i < nNew; i++) {
				String16 s = m_titles.getItemValue(i, String::null());
				::SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)(s.getData()));
			}
		}
	}

	void __copyItems(HWND hWnd)
	{
		::SendMessageW(hWnd, CB_RESETCONTENT, 0, 0);
		__applyItemsCount(hWnd);
		sl_uint32 n = (sl_uint32)(m_titles.getCount());
		if (m_indexSelected >= n) {
			m_indexSelected = 0;
		}
		if (n > 0) {
			::SendMessageW(hWnd, CB_SETCURSEL, (WPARAM)m_indexSelected, 0);
		}
	}

	void __setItemTitle(HWND hWnd, sl_uint32 index, const String& title)
	{
		String16 s = title;
		::SendMessageW(hWnd, CB_DELETESTRING, (WPARAM)index, 0);
		::SendMessageW(hWnd, CB_INSERTSTRING, (WPARAM)index, (LPARAM)(s.getData()));
	}

	sl_uint32 __getSelectedIndex(HWND hWnd)
	{
		return (sl_uint32)(::SendMessageW(hWnd, CB_GETCURSEL, 0, 0));
	}

	void __onSelectItem(HWND hWnd)
	{
		onSelectItem(__getSelectedIndex(hWnd));
	}

};

class _Win32_DropDownListInstance : public Win32_ViewInstance
{
public:
    // override
	sl_bool processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		return sl_false;
	}

    // override
	sl_bool processCommand(SHORT code, LRESULT& result)
	{
		if (code == CBN_SELCHANGE) {
			Ref<View> view = getView();
			if (DropDownList::checkInstance(view.ptr)) {
				((_DropDownList*)(view.ptr))->__onSelectItem(m_handle);
			}
			return sl_true;
		}
		return sl_false;
	}
};

Ref<ViewInstance> DropDownList::createInstance(ViewInstance* parent)
{
	UINT style = CBS_DROPDOWNLIST | WS_TABSTOP;
	Ref<_Win32_DropDownListInstance> ret = Win32_ViewInstance::create<_Win32_DropDownListInstance>(this, parent, L"COMBOBOX", L"", style, 0);
	
	if (ret.isNotNull()) {

		HWND handle = ret->getHandle();

		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		HFONT hFont = UIPlatform::getGdiFont(font.ptr, fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
		m_fontInstance = fontInstance;

		((_DropDownList*)this)->__copyItems(handle);
	}
	return ret;
}

sl_uint32 DropDownList::getSelectedIndex()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		return ((_DropDownList*)this)->__getSelectedIndex(handle);
	}
	return m_indexSelected;
}

void DropDownList::_select(sl_uint32 index)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		::SendMessageW(handle, CB_SETCURSEL, (WPARAM)index, 0);
	}
	m_indexSelected = index;
}

void DropDownList::_refreshItemsCount()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_DropDownList*)this)->__applyItemsCount(handle);
	}
}

void DropDownList::_refreshItemsContent()
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_DropDownList*)this)->__copyItems(handle);
	}
}

void DropDownList::_setItemTitle(sl_uint32 index, const String& title)
{
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		((_DropDownList*)this)->__setItemTitle(handle, index, title);
	}
}

void DropDownList::setFont(const Ref<Font>& font)
{
	Ref<FontInstance> fontInstance;
	HWND handle = UIPlatform::getViewHandle(this);
	if (handle) {
		HFONT hFont = UIPlatform::getGdiFont(font.ptr, fontInstance);
		if (hFont) {
			::SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
		}
	}
	m_font = font;
	m_fontInstance = fontInstance;
}

SLIB_UI_NAMESPACE_END

#endif
