/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/common_dialogs.h"

#include "slib/ui/core.h"
#include "slib/ui/window.h"

namespace slib
{

/***************************************
			AlertDialog
***************************************/

	AlertDialog::AlertDialog()
	{
		buttons = AlertDialogButtons::Ok;
	}

	AlertDialog::AlertDialog(const AlertDialog& other) = default;

	AlertDialog::~AlertDialog()
	{
	}
	
	class _priv_AlertDialog_RunOnUIThread
	{
	public:
		AlertDialog* alert;
		Ref<Event> event;
		DialogResult result = DialogResult::Cancel;
		
		void run()
		{
			result = alert->_run();
			event->set();
		}
		
	};

	DialogResult AlertDialog::_runOnUiThread()
	{
		if (UI::isUiThread()) {
			return _run();
		}
		Ref<Event> ev = Event::create(sl_false);
		if (ev.isNotNull()) {
			_priv_AlertDialog_RunOnUIThread m;
			m.alert = this;
			m.event = ev;
			UI::dispatchToUiThread(SLIB_FUNCTION_CLASS(_priv_AlertDialog_RunOnUIThread, run, &m));
			ev->wait();
			return m.result;
		}
		return DialogResult::Cancel;
	}

	class _priv_AlertDialog_CallbackRunByShow_UIThread
	{
	public:
		DialogResult result = DialogResult::Cancel;
		
		void onOk()
		{
			result = DialogResult::Ok;
			UI::quitLoop();
		}
	
		void onCancel()
		{
			result = DialogResult::Cancel;
			UI::quitLoop();
		}
		
		void onYes()
		{
			result = DialogResult::Yes;
			UI::quitLoop();
		}
		
		void onNo()
		{
			result = DialogResult::No;
			UI::quitLoop();
		}

	};

	class _priv_AlertDialog_CallbackRunByShow_NonUIThread
	{
	public:
		DialogResult result = DialogResult::Cancel;
		Ref<Event> event;
		
		void onOk()
		{
			result = DialogResult::Ok;
			event->set();
		}
		
		void onCancel()
		{
			result = DialogResult::Cancel;
			event->set();
		}
		
		void onYes()
		{
			result = DialogResult::Yes;
			event->set();
		}

		void onNo()
		{
			result = DialogResult::No;
			event->set();
		}
		
	};
	
	void _priv_AlertDialog_runByShow(AlertDialog* alert, _priv_AlertDialog_CallbackRunByShow_NonUIThread* m)
	{
		if (!(alert->_show())) {
			m->onCancel();
		}
	}

	DialogResult AlertDialog::_runByShow()
	{
		Ref<AlertDialog> alert = getReferable();
		if (alert.isNull()) {
			return DialogResult::Cancel;
		}
		if (UI::isUiThread()) {
			_priv_AlertDialog_CallbackRunByShow_UIThread m;
			alert->onOk = SLIB_FUNCTION_CLASS(_priv_AlertDialog_CallbackRunByShow_UIThread, onOk, &m);
			alert->onCancel = SLIB_FUNCTION_CLASS(_priv_AlertDialog_CallbackRunByShow_UIThread, onCancel, &m);
			alert->onYes = SLIB_FUNCTION_CLASS(_priv_AlertDialog_CallbackRunByShow_UIThread, onYes, &m);
			alert->onNo = SLIB_FUNCTION_CLASS(_priv_AlertDialog_CallbackRunByShow_UIThread, onNo, &m);
			if (alert->_show()) {
				UI::runLoop();
				return m.result;
			}
		} else {
			Ref<Event> ev = Event::create(sl_false);
			if (ev.isNotNull()) {
				_priv_AlertDialog_CallbackRunByShow_NonUIThread m;
				m.event = ev;
				alert->onOk = SLIB_FUNCTION_CLASS(_priv_AlertDialog_CallbackRunByShow_NonUIThread, onOk, &m);
				alert->onCancel = SLIB_FUNCTION_CLASS(_priv_AlertDialog_CallbackRunByShow_NonUIThread, onCancel, &m);
				alert->onYes = SLIB_FUNCTION_CLASS(_priv_AlertDialog_CallbackRunByShow_NonUIThread, onYes, &m);
				alert->onNo = SLIB_FUNCTION_CLASS(_priv_AlertDialog_CallbackRunByShow_NonUIThread, onNo, &m);
				UI::dispatchToUiThread(Function<void()>::bind(&_priv_AlertDialog_runByShow, alert.get(), &m));
				ev->wait();
				return m.result;
			}
		}
		return DialogResult::Cancel;
	}
	
	void _priv_AlertDialog_Show(const Ref<AlertDialog>& alert)
	{
		if (!(alert->_show())) {
			if (alert->buttons == AlertDialogButtons::Ok) {
				alert->onOk();
			} else if (alert->buttons == AlertDialogButtons::YesNo) {
				alert->onNo();
			} else {
				alert->onCancel();
			}
		}
	}

	void AlertDialog::_showOnUiThread()
	{
		Ref<AlertDialog> alert = getReferable();
		if (alert.isNotNull()) {
			if (UI::isUiThread()) {
				_priv_AlertDialog_Show(alert);
			} else {
				UI::dispatchToUiThread(Function<void()>::bind(&_priv_AlertDialog_Show, alert));
			}
		}
	}

	void _priv_AlertDialog_showByRun(const Ref<AlertDialog>& alert)
	{
		DialogResult result = alert->_run();
		switch (result) {
			case DialogResult::Ok:
				alert->onOk();
				break;
			case DialogResult::Yes:
				alert->onYes();
				break;
			case DialogResult::No:
				alert->onNo();
				break;
			default:
				if (alert->buttons == AlertDialogButtons::Ok) {
					alert->onOk();
				} else if (alert->buttons == AlertDialogButtons::YesNo) {
					alert->onNo();
				} else {
					alert->onCancel();
				}
				break;
		}
	}

	void AlertDialog::_showByRun()
	{
		Ref<AlertDialog> alert = getReferable();
		if (alert.isNotNull()) {
			UI::dispatchToUiThread(Function<void()>::bind(&_priv_AlertDialog_showByRun, alert));
		}
	}
	
	AlertDialog* AlertDialog::getReferable()
	{
		if (getReferenceCount() > 0) {
			return this;
		} else {
			return new AlertDialog(*this);
		}
	}

	void AlertDialog::run(const String& text)
	{
		AlertDialog alert;
		alert.text = text;
		alert.run();
	}

	void AlertDialog::run(const Ref<Window>& parent, const String& text)
	{
		AlertDialog alert;
		alert.text = text;
		alert.parent = parent;
		alert.run();
	}

	void AlertDialog::run(const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.caption = caption;
		alert.text = text;
		alert.run();
	}

	void AlertDialog::run(const Ref<Window>& parent, const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.caption = caption;
		alert.text = text;
		alert.parent = parent;
		alert.run();
	}

	void AlertDialog::show(const String& text)
	{
		AlertDialog alert;
		alert.text = text;
		alert.show();
	}

	void AlertDialog::show(const String& text, const Function<void()>& onOk)
	{
		AlertDialog alert;
		alert.text = text;
		alert.onOk = onOk;
		alert.show();
	}

	void AlertDialog::show(const String& caption, const String& text, const Function<void()>& onOk)
	{
		AlertDialog alert;
		alert.caption = caption;
		alert.text = text;
		alert.onOk = onOk;
		alert.show();
	}

	DialogResult AlertDialog::runOkCancel(const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.text = text;
		return alert.run();
	}

	DialogResult AlertDialog::runOkCancel(const Ref<Window>& parent, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.text = text;
		alert.parent = parent;
		return alert.run();
	}

	DialogResult AlertDialog::runOkCancel(const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.caption = caption;
		alert.text = text;
		return alert.run();
	}

	DialogResult AlertDialog::runOkCancel(const Ref<Window>& parent, const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.caption = caption;
		alert.text = text;
		alert.parent = parent;
		return alert.run();
	}

	void AlertDialog::showOkCancel(const String& text, const Function<void()>& onOk, const Function<void()>& onCancel)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.text = text;
		alert.onOk = onOk;
		alert.onCancel = onCancel;
		alert.show();
	}

	void AlertDialog::showOkCancel(const String& caption, const String& text, const Function<void()>& onOk, const Function<void()>& onCancel)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.caption = caption;
		alert.text = text;
		alert.onOk = onOk;
		alert.onCancel = onCancel;
		alert.show();
	}

	void AlertDialog::showOkCancel(const String& text, const Function<void()>& onOk)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		
		alert.text = text;
		alert.onOk = onOk;
		alert.show();
	}

	void AlertDialog::showOkCancel(const String& caption, const String& text, const Function<void()>& onOk)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::OkCancel;
		alert.caption = caption;
		alert.text = text;
		alert.onOk = onOk;
		alert.show();
	}

	DialogResult AlertDialog::runYesNo(const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNo;
		alert.text = text;
		return alert.run();
	}

	DialogResult AlertDialog::runYesNo(const Ref<Window>& parent, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNo;
		alert.text = text;
		alert.parent = parent;
		return alert.run();
	}

	DialogResult AlertDialog::runYesNo(const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNo;
		alert.caption = caption;
		alert.text = text;
		return alert.run();
	}

	DialogResult AlertDialog::runYesNo(const Ref<Window>& parent, const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNo;
		alert.caption = caption;
		alert.text = text;
		alert.parent = parent;
		return alert.run();
	}

	void AlertDialog::showYesNo(const String& text, const Function<void()>& onYes, const Function<void()>& onNo)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNo;
		alert.text = text;
		alert.onYes = onYes;
		alert.onNo = onNo;
		alert.show();
	}

	void AlertDialog::showYesNo(const String& caption, const String& text, const Function<void()>& onYes, const Function<void()>& onNo)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNo;
		alert.caption = caption;
		alert.text = text;
		alert.onYes = onYes;
		alert.onNo = onNo;
		alert.show();
	}

	void AlertDialog::showYesNo(const String& text, const Function<void()>& onYes)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNo;
		
		alert.text = text;
		alert.onYes = onYes;
		alert.show();
	}

	void AlertDialog::showYesNo(const String& caption, const String& text, const Function<void()>& onYes)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNo;
		alert.caption = caption;
		alert.text = text;
		alert.onYes = onYes;
		alert.show();
	}

	DialogResult AlertDialog::runYesNoCancel(const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNoCancel;
		alert.text = text;
		return alert.run();
	}

	DialogResult AlertDialog::runYesNoCancel(const Ref<Window>& parent, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNoCancel;
		alert.text = text;
		alert.parent = parent;
		return alert.run();
	}

	DialogResult AlertDialog::runYesNoCancel(const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNoCancel;
		alert.caption = caption;
		alert.text = text;
		return alert.run();
	}

	DialogResult AlertDialog::runYesNoCancel(const Ref<Window>& parent, const String& caption, const String& text)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNoCancel;
		alert.caption = caption;
		alert.text = text;
		alert.parent = parent;
		return alert.run();
	}

	void AlertDialog::showYesNoCancel(const String& text, const Function<void()>& onYes, const Function<void()>& onNo, const Function<void()>& onCancel)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNoCancel;
		alert.text = text;
		alert.onYes = onYes;
		alert.onNo = onNo;
		alert.onCancel = onCancel;
		alert.show();
	}

	void AlertDialog::showYesNoCancel(const String& caption, const String& text, const Function<void()>& onYes, const Function<void()>& onNo, const Function<void()>& onCancel)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNoCancel;
		alert.caption = caption;
		alert.text = text;
		alert.onYes = onYes;
		alert.onNo = onNo;
		alert.onCancel = onCancel;
		alert.show();
	}

	void AlertDialog::showYesNoCancel(const String& text, const Function<void()>& onYes, const Function<void()>& onNo)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNoCancel;
		alert.text = text;
		alert.onYes = onYes;
		alert.onNo = onNo;
		alert.show();
	}

	void AlertDialog::showYesNoCancel(const String& caption, const String& text, const Function<void()>& onYes, const Function<void()>& onNo)
	{
		AlertDialog alert;
		alert.buttons = AlertDialogButtons::YesNoCancel;
		alert.caption = caption;
		alert.text = text;
		alert.onYes = onYes;
		alert.onNo = onNo;
		alert.show();
	}


/***************************************
		FileDialog
***************************************/

	FileDialog::FileDialog()
	{
		type = FileDialogType::OpenFile;
		flagShowHiddenFiles = sl_true;
	}

	FileDialog::FileDialog(const FileDialog& other) = default;

	FileDialog::~FileDialog()
	{
	}

	void FileDialog::addFilter(const String& title, const String& patterns)
	{
		Filter filter;
		filter.title = title;
		filter.patterns = patterns;
		filters.add(filter);
	}

	class _priv_FileDialog_RunOnUIThread
	{
	public:
		FileDialog* dlg;
		Ref<Event> event;
		sl_bool result = sl_false;
		
		void run()
		{
			result = dlg->_run();
			event->set();
		}
		
	};

	sl_bool FileDialog::_runOnUiThread()
	{
		if (UI::isUiThread()) {
			return _run();
		}
		Ref<Event> ev = Event::create(sl_false);
		if (ev.isNotNull()) {
			_priv_FileDialog_RunOnUIThread m;
			m.dlg = this;
			m.event = ev;
			UI::dispatchToUiThread(SLIB_FUNCTION_CLASS(_priv_FileDialog_RunOnUIThread, run, &m));
			ev->wait();
			return m.result;
		}
		return sl_false;
	}

	List<String> FileDialog::openFiles(const Ref<Window>& parent)
	{
		FileDialog dlg;
		dlg.type = FileDialogType::OpenFiles;
		dlg.parent = parent;
		if (dlg.run()) {
			return dlg.selectedPaths;
		}
		return sl_null;
	}

	String FileDialog::openFile(const Ref<Window>& parent)
	{
		FileDialog dlg;
		dlg.type = FileDialogType::OpenFile;
		dlg.parent = parent;
		if (dlg.run()) {
			return dlg.selectedPath;
		}
		return sl_null;
	}

	String FileDialog::saveFile(const Ref<Window>& parent)
	{
		FileDialog dlg;
		dlg.type = FileDialogType::SaveFile;
		dlg.parent = parent;
		if (dlg.run()) {
			return dlg.selectedPath;
		}
		return sl_null;
	}

	String FileDialog::selectDirectory(const Ref<Window>& parent)
	{
		FileDialog dlg;
		dlg.type = FileDialogType::SelectDirectory;
		dlg.parent = parent;
		if (dlg.run()) {
			return dlg.selectedPath;
		}
		return sl_null;
	}

}
