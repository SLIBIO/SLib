#include "../../../inc/slib/ui/common_dialogs.h"

#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

/***************************************
			AlertDialog
***************************************/

AlertDialog::AlertDialog()
{
	type = AlertDialogType::Ok;
}

AlertDialog::AlertDialog(const AlertDialog& other) = default;

class _AlertDialog_RunOnUIThread
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
		_AlertDialog_RunOnUIThread m;
		m.alert = this;
		m.event = ev;
		UI::dispatchToUiThread(SLIB_CALLBACK_CLASS(_AlertDialog_RunOnUIThread, run, &m));
		ev->wait();
		return m.result;
	}
	return DialogResult::Cancel;
}

class _AlertDialog_CallbackRunByShow_UIThread
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
	
	void onNo()
	{
		result = DialogResult::No;
		UI::quitLoop();
	}

};

class _AlertDialog_CallbackRunByShow_NonUIThread
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
	
	void onNo()
	{
		result = DialogResult::No;
		event->set();
	}
	
};

void _AlertDialog_runByShow(AlertDialog* box)
{
	box->_show();
}

DialogResult AlertDialog::_runByShow()
{
	AlertDialog alert(*this);
	if (UI::isUiThread()) {
		_AlertDialog_CallbackRunByShow_UIThread m;
		alert.onOk = SLIB_CALLBACK_CLASS(_AlertDialog_CallbackRunByShow_UIThread, onOk, &m);
		alert.onCancel = SLIB_CALLBACK_CLASS(_AlertDialog_CallbackRunByShow_UIThread, onCancel, &m);
		alert.onNo = SLIB_CALLBACK_CLASS(_AlertDialog_CallbackRunByShow_UIThread, onNo, &m);
		alert._show();
		UI::runLoop();
		return m.result;
	} else {
		Ref<Event> ev = Event::create(sl_false);
		if (ev.isNotNull()) {
			_AlertDialog_CallbackRunByShow_NonUIThread m;
			m.event = ev;
			alert.onOk = SLIB_CALLBACK_CLASS(_AlertDialog_CallbackRunByShow_NonUIThread, onOk, &m);
			alert.onCancel = SLIB_CALLBACK_CLASS(_AlertDialog_CallbackRunByShow_NonUIThread, onCancel, &m);
			alert.onNo = SLIB_CALLBACK_CLASS(_AlertDialog_CallbackRunByShow_NonUIThread, onNo, &m);
			UI::dispatchToUiThread(SLIB_CALLBACK(_AlertDialog_runByShow, &alert));
			ev->wait();
			return m.result;
		}
	}
	return DialogResult::Cancel;
}

void _AlertDialog_showOnUiThread(Ref<AlertDialog> box)
{
	box->_show();
}

void AlertDialog::_showOnUiThread()
{
	if (UI::isUiThread()) {
		_show();
		return;
	}
	Ref<AlertDialog> alert = new AlertDialog(*this);
	if (alert.isNotNull()) {
		UI::dispatchToUiThread(SLIB_CALLBACK(_AlertDialog_showOnUiThread, alert));
	}
}

void _AlertDialog_showByRun(Ref<AlertDialog> alert)
{
	DialogResult result = alert->_run();
	if (result == DialogResult::Ok) {
		if (alert->onOk.isNotNull()) {
			alert->onOk->run();
		}
	} else if (result == DialogResult::Cancel) {
		if (alert->onCancel.isNotNull()) {
			alert->onCancel->run();
		}
	} else if (result == DialogResult::No) {
		if (alert->onNo.isNotNull()) {
			alert->onNo->run();
		}
	}
}

void AlertDialog::_showByRun()
{
	Ref<AlertDialog> alert = new AlertDialog(*this);
	if (alert.isNotNull()) {
		UI::dispatchToUiThread(SLIB_CALLBACK(_AlertDialog_showByRun, alert));
	}
}

void AlertDialog::run(const String& text)
{
	AlertDialog alert;
	alert.text = text;
	alert.run();
}

void AlertDialog::run(const String& caption, const String& text)
{
	AlertDialog alert;
	alert.caption = caption;
	alert.text = text;
	alert.run();
}

void AlertDialog::show(const String& text, const Ref<Runnable>& onOk)
{
	AlertDialog alert;
	alert.text = text;
	alert.onOk = onOk;
	alert.show();
}

void AlertDialog::show(const String& caption, const String& text, const Ref<Runnable>& onOk)
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
	alert.type = AlertDialogType::OkCancel;
	alert.text = text;
	return alert.run();
}

DialogResult AlertDialog::runOkCancel(const String& caption, const String& text)
{
	AlertDialog alert;
	alert.type = AlertDialogType::OkCancel;
	alert.caption = caption;
	alert.text = text;
	return alert.run();
}

void AlertDialog::showOkCancel(const String& text, const Ref<Runnable>& onOk, const Ref<Runnable>& onCancel)
{
	AlertDialog alert;
	alert.type = AlertDialogType::OkCancel;
	alert.text = text;
	alert.onOk = onOk;
	alert.onCancel = onCancel;
	alert.show();
}

void AlertDialog::showOkCancel(const String& caption, const String& text, const Ref<Runnable>& onOk, const Ref<Runnable>& onCancel)
{
	AlertDialog alert;
	alert.type = AlertDialogType::OkCancel;
	alert.caption = caption;
	alert.text = text;
	alert.onOk = onOk;
	alert.onCancel = onCancel;
	alert.show();
}

void AlertDialog::showOkCancel(const String& text, const Ref<Runnable>& onOk)
{
	AlertDialog alert;
	alert.type = AlertDialogType::OkCancel;
	
	alert.text = text;
	alert.onOk = onOk;
	alert.show();
}

void AlertDialog::showOkCancel(const String& caption, const String& text, const Ref<Runnable>& onOk)
{
	AlertDialog alert;
	alert.type = AlertDialogType::OkCancel;
	alert.caption = caption;
	alert.text = text;
	alert.onOk = onOk;
	alert.show();
}

DialogResult AlertDialog::runYesNo(const String& text)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNo;
	alert.text = text;
	return alert.run();
}

DialogResult AlertDialog::runYesNo(const String& caption, const String& text)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNo;
	alert.caption = caption;
	alert.text = text;
	return alert.run();
}

void AlertDialog::showYesNo(const String& text, const Ref<Runnable>& onYes, const Ref<Runnable>& onNo)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNo;
	alert.text = text;
	alert.onOk = onYes;
	alert.onNo = onNo;
	alert.show();
}

void AlertDialog::showYesNo(const String& caption, const String& text, const Ref<Runnable>& onYes, const Ref<Runnable>& onNo)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNo;
	alert.caption = caption;
	alert.text = text;
	alert.onOk = onYes;
	alert.onNo = onNo;
	alert.show();
}

void AlertDialog::showYesNo(const String& text, const Ref<Runnable>& onYes)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNo;
	
	alert.text = text;
	alert.onOk = onYes;
	alert.show();
}

void AlertDialog::showYesNo(const String& caption, const String& text, const Ref<Runnable>& onYes)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNo;
	alert.caption = caption;
	alert.text = text;
	alert.onOk = onYes;
	alert.show();
}

DialogResult AlertDialog::runYesNoCancel(const String& text)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNoCancel;
	alert.text = text;
	return alert.run();
}

DialogResult AlertDialog::runYesNoCancel(const String& caption, const String& text)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNoCancel;
	alert.caption = caption;
	alert.text = text;
	return alert.run();
}

void AlertDialog::showYesNoCancel(const String& text, const Ref<Runnable>& onYes, const Ref<Runnable>& onNo, const Ref<Runnable>& onCancel)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNoCancel;
	alert.text = text;
	alert.onOk = onYes;
	alert.onNo = onNo;
	alert.onCancel = onCancel;
	alert.show();
}

void AlertDialog::showYesNoCancel(const String& caption, const String& text, const Ref<Runnable>& onYes, const Ref<Runnable>& onNo, const Ref<Runnable>& onCancel)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNoCancel;
	alert.caption = caption;
	alert.text = text;
	alert.onOk = onYes;
	alert.onNo = onNo;
	alert.onCancel = onCancel;
	alert.show();
}

void AlertDialog::showYesNoCancel(const String& text, const Ref<Runnable>& onYes, const Ref<Runnable>& onNo)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNoCancel;
	alert.text = text;
	alert.onOk = onYes;
	alert.onNo = onNo;
	alert.show();
}

void AlertDialog::showYesNoCancel(const String& caption, const String& text, const Ref<Runnable>& onYes, const Ref<Runnable>& onNo)
{
	AlertDialog alert;
	alert.type = AlertDialogType::YesNoCancel;
	alert.caption = caption;
	alert.text = text;
	alert.onOk = onYes;
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

void FileDialog::addFilter(const String& title, const String& patterns)
{
	Filter filter;
	filter.title = title;
	filter.patterns = patterns;
	filters.add(filter);
}

class _FileDialog_RunOnUIThread
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
		_FileDialog_RunOnUIThread m;
		m.dlg = this;
		m.event = ev;
		UI::dispatchToUiThread(SLIB_CALLBACK_CLASS(_FileDialog_RunOnUIThread, run, &m));
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
	return List<String>::null();
}

String FileDialog::openFile(const Ref<Window>& parent)
{
	FileDialog dlg;
	dlg.type = FileDialogType::OpenFile;
	dlg.parent = parent;
	if (dlg.run()) {
		return dlg.selectedPath;
	}
	return String::null();
}

String FileDialog::saveFile(const Ref<Window>& parent)
{
	FileDialog dlg;
	dlg.type = FileDialogType::SaveFile;
	dlg.parent = parent;
	if (dlg.run()) {
		return dlg.selectedPath;
	}
	return String::null();
}

String FileDialog::selectDirectory(const Ref<Window>& parent)
{
	FileDialog dlg;
	dlg.type = FileDialogType::SelectDirectory;
	dlg.parent = parent;
	if (dlg.run()) {
		return dlg.selectedPath;
	}
	return String::null();
}

SLIB_UI_NAMESPACE_END
