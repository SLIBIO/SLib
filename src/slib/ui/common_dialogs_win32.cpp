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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_WIN32)

#include "slib/ui/common_dialogs.h"
#include "slib/ui/window.h"
#include "slib/core/scoped.h"
#include "slib/core/file.h"

#include "slib/ui/platform.h"

#include "ui_core_win32.h"

#pragma warning(disable: 4091)
#include <ShlObj.h>

namespace slib
{

/***************************************
		AlertDialog
***************************************/

	DialogResult AlertDialog::run()
	{
		return _runOnUiThread();
	}

	void _priv_Win32_processCustomMsgBox(WPARAM wParam, LPARAM lParam)
	{
		HWND hWndMsg = ::FindWindowW(NULL, L"CustomizedMsgBox");
		if (hWndMsg == NULL) {
			return;
		}
		AlertDialog* alert = (AlertDialog*)lParam;
		String16 caption = alert->caption;
		::SetWindowTextW(hWndMsg, (LPCWSTR)(caption.getData()));

		switch (alert->buttons) {
		case AlertDialogButtons::Ok:
			if (alert->titleOk.isNotNull()) {
				String16 titleOk = alert->titleOk;
				::SetDlgItemTextW(hWndMsg, 2, (LPCWSTR)(titleOk.getData()));
			}
			break;
		case AlertDialogButtons::OkCancel:
			if (alert->titleOk.isNotNull()) {
				String16 titleOk = alert->titleOk;
				::SetDlgItemTextW(hWndMsg, 1, (LPCWSTR)(titleOk.getData()));
			}
			if (alert->titleCancel.isNotNull()) {
				String16 titleCancel = alert->titleCancel;
				::SetDlgItemTextW(hWndMsg, 2, (LPCWSTR)(titleCancel.getData()));
			}
			break;
		case AlertDialogButtons::YesNo:
			if (alert->titleYes.isNotNull()) {
				String16 titleYes = alert->titleYes;
				::SetDlgItemTextW(hWndMsg, 6, (LPCWSTR)(titleYes.getData()));
			}
			if (alert->titleNo.isNotNull()) {
				String16 titleNo = alert->titleNo;
				::SetDlgItemTextW(hWndMsg, 7, (LPCWSTR)(titleNo.getData()));
			}
			break;
		case AlertDialogButtons::YesNoCancel:
			if (alert->titleYes.isNotNull()) {
				String16 titleYes = alert->titleYes;
				::SetDlgItemTextW(hWndMsg, 6, (LPCWSTR)(titleYes.getData()));
			}
			if (alert->titleNo.isNotNull()) {
				String16 titleNo = alert->titleNo;
				::SetDlgItemTextW(hWndMsg, 7, (LPCWSTR)(titleNo.getData()));
			}
			if (alert->titleCancel.isNotNull()) {
				String16 titleCancel = alert->titleCancel;
				::SetDlgItemTextW(hWndMsg, 2, (LPCWSTR)(titleCancel.getData()));
			}
			break;
		}
	}

	DialogResult AlertDialog::_run()
	{
		int style;

		switch (buttons) {
		case AlertDialogButtons::OkCancel:
			style = MB_OKCANCEL;
			break;
		case AlertDialogButtons::YesNo:
			style = MB_YESNO;
			break;
		case AlertDialogButtons::YesNoCancel:
			style = MB_YESNOCANCEL;
			break;
		default:
			style = MB_OK;
			break;
		}

		switch (icon) {
		case AlertDialogIcon::Error:
			style |= MB_ICONERROR;
			break;
		case AlertDialogIcon::Warning:
			style |= MB_ICONWARNING;
			break;
		case AlertDialogIcon::Question:
			style |= MB_ICONQUESTION;
			break;
		case AlertDialogIcon::Information:
			style |= MB_ICONINFORMATION;
			break;
		default:
			break;
		}

		HWND hWndParent = UIPlatform::getWindowHandle(parent.get());
		if (!hWndParent) {
			style |= MB_TASKMODAL;
		}

		int result;

		String16 text = this->text;
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (shared) {
			::PostMessage(shared->hWndMessage, SLIB_UI_MESSAGE_CUSTOM_MSGBOX, 0, (LPARAM)(this));
			result = ::MessageBoxW(hWndParent, (LPCWSTR)(text.getData()), L"CustomizedMsgBox", style);
		} else {
			String16 caption = this->caption;
			result = ::MessageBoxW(hWndParent, (LPCWSTR)(text.getData()), (LPCWSTR)(caption.getData()), style);
		}

		switch (result) {
		case IDOK:
			return DialogResult::Ok;
		case IDCANCEL:
			return DialogResult::Cancel;
		case IDYES:
			return DialogResult::Yes;
		case IDNO:
			return DialogResult::No;
		}
		return DialogResult::Error;
	}

	void AlertDialog::show()
	{
		_showByRun();
	}

	sl_bool AlertDialog::_show()
	{
		return sl_false;
	}


/***************************************
		FileDialog
***************************************/

	sl_bool FileDialog::run()
	{
		return _runOnUiThread();
	}

	struct _priv_FileDialog_FilterW
	{
		String16 title;
		String16 patterns;
	};

	static int CALLBACK _priv_FileDialog_BrowseDirCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData)
	{
		switch (uMsg) {
			case BFFM_INITIALIZED:
				if (pData) {
					SendMessageW(hwnd, BFFM_SETSELECTION, TRUE, pData);
				}
				break;
		}
		return 0;
	}

	sl_bool FileDialog::_run()
	{
		if (type == FileDialogType::SelectDirectory) {
			IMalloc* pMalloc;
			sl_bool result = sl_false;
			if (SUCCEEDED(SHGetMalloc(&pMalloc))) {
				BROWSEINFOW bi;
				Base::zeroMemory(&bi, sizeof(bi));
				bi.hwndOwner = UIPlatform::getWindowHandle(parent.get());
				String16 _title = title;
				if (_title.isEmpty()) {
					bi.lpszTitle = L"Browse for folder...";
				} else {
					bi.lpszTitle = (LPWSTR)(_title.getData());
				}
				bi.ulFlags = BIF_NEWDIALOGSTYLE;
				bi.lpfn = _priv_FileDialog_BrowseDirCallback;
				String16 initialDir;
				if (File::isDirectory(selectedPath)) {
					initialDir = selectedPath;
				}
				if (initialDir.isNotEmpty()) {
					bi.lParam = (LPARAM)(initialDir.getData());
				}
				LPITEMIDLIST pidl = ::SHBrowseForFolderW(&bi);
				if (pidl) {
					WCHAR szPath[MAX_PATH + 1];
					if (SHGetPathFromIDListW(pidl, szPath)) {
						selectedPath = szPath;
						selectedPaths = List<String>::createFromElement(selectedPath);
						result = sl_true;
					}
					pMalloc->Free(pidl);
				}
				pMalloc->Release();
			}
			return result;
		} else {
			OPENFILENAMEW ofn;
			Base::zeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = UIPlatform::getWindowHandle(parent.get());

			String16 _defaultFileExt = defaultFileExt;
			ofn.lpstrDefExt = (LPCWSTR)(_defaultFileExt.getData());

			sl_size lenSzFilters = 0;
			CList<_priv_FileDialog_FilterW> wfilters;
			{
				_priv_FileDialog_FilterW wfilter;
				ListLocker<Filter> list(filters);
				for (sl_size i = 0; i < list.count; i++) {
					wfilter.title = list[i].title;
					wfilter.patterns = list[i].patterns;
					lenSzFilters += wfilter.title.getLength();
					lenSzFilters++;
					lenSzFilters += wfilter.patterns.getLength();
					lenSzFilters++;
					wfilters.add_NoLock(wfilter);
				}
				lenSzFilters++;
			}
			SLIB_SCOPED_BUFFER(WCHAR, 1024, szFilters, lenSzFilters);
			{
				sl_size pos = 0;
				sl_size len;
				ListElements<_priv_FileDialog_FilterW> list(wfilters);
				for (sl_size i = 0; i < list.count; i++) {
					len = list[i].title.getLength();
					Base::copyMemory(szFilters + pos, list[i].title.getData(), len * 2);
					pos += len;
					szFilters[pos] = 0;
					pos++;
					len = list[i].patterns.getLength();
					Base::copyMemory(szFilters + pos, list[i].patterns.getData(), len * 2);
					pos += len;
					szFilters[pos] = 0;
					pos++;
				}
				szFilters[pos] = 0;
			}
			ofn.lpstrFilter = szFilters;

			WCHAR szFile[4096];
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
			szFile[0] = 0;
			
			String16 initialDir;
			String16 fileName;
			if (File::isDirectory(selectedPath)) {
				initialDir = selectedPath;
			} else {
				String path = File::getParentDirectoryPath(selectedPath);
				if (File::isDirectory(path)) {
					initialDir = path;
				}
				fileName = File::getFileName(selectedPath);
			}
			if (initialDir.isNotEmpty()) {
				ofn.lpstrInitialDir = (LPCWSTR)(initialDir.getData());
			}
			if (fileName.isNotEmpty()) {
				sl_size n = fileName.getLength();
				if (n > 1024) {
					n = 1024;
				}
				Base::copyMemory(szFile, fileName.getData(), 2 * n + 2);
			}

			String16 _title = title;
			if (_title.isNotEmpty()) {
				ofn.lpstrTitle = (LPCWSTR)(_title.getData());
			}

			ofn.Flags = OFN_DONTADDTORECENT | OFN_EXPLORER;
			if (flagShowHiddenFiles) {
				ofn.Flags |= OFN_FORCESHOWHIDDEN;
			}
			if (type == FileDialogType::OpenFile) {
				ofn.Flags |= OFN_FILEMUSTEXIST;
				if (::GetOpenFileNameW(&ofn)) {
					selectedPath = ofn.lpstrFile;
					selectedPaths = List<String>::createFromElement(selectedPath);
					return sl_true;
				}
			} else if (type == FileDialogType::OpenFiles) {
				ofn.Flags |= OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
				if (::GetOpenFileNameW(&ofn)) {
					WCHAR* sz = ofn.lpstrFile;
					DWORD attr = ::GetFileAttributesW(sz);
					if (attr != INVALID_FILE_ATTRIBUTES) {
						if (attr & FILE_ATTRIBUTE_DIRECTORY) {
							sl_size len = Base::getStringLength2((sl_char16*)sz);
							if (len > 0) {
								List<String> files;
								sz[len] = '/';
								String dir(sz, len + 1);
								sz += (len + 1);
								while (1) {
									len = Base::getStringLength2((sl_char16*)sz);
									if (len == 0) {
										break;
									}
									String fileName(sz, len);
									files.add_NoLock(dir + fileName);
									sz += (len + 1);
								}
								if (files.isNotEmpty()) {
									selectedPaths = files;
									selectedPath = files.getValueAt(0);
									return sl_true;
								}
							}
						} else {
							selectedPath = ofn.lpstrFile;
							selectedPaths = List<String>::createFromElement(selectedPath);
							return sl_true;
						}
					}
				}
			} else {
				ofn.Flags |= OFN_OVERWRITEPROMPT;
				if (::GetSaveFileNameW(&ofn)) {
					selectedPath = ofn.lpstrFile;
					selectedPaths = List<String>::createFromElement(selectedPath);
					return sl_true;
				}
			}
		}
		return sl_false;
	}

}

#endif
