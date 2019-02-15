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

#include "slib/ui/common_dialogs.h"

#include "slib/ui/core.h"
#include "slib/ui/mobile_app.h"
#include "slib/ui/label_view.h"

#include "slib/core/safe_static.h"

namespace slib
{

/***************************************
			AlertDialog
***************************************/

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AlertDialog)
	
	AlertDialog::AlertDialog()
	{
		flagHyperText = sl_false;
		buttons = AlertDialogButtons::Ok;
		icon = AlertDialogIcon::None;
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


/***************************************
		FileDialog
***************************************/

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(FileDialog)
	
	FileDialog::FileDialog()
	{
		type = FileDialogType::OpenFile;
		flagShowHiddenFiles = sl_true;
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

/***************************************
 			Toast
***************************************/

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(Toast)

	Toast::Toast()
	{
		duration = getDefaultDuration();
		font = getDefaultFont();
	}
	
	class _priv_ToastManager
	{
	public:
		Mutex lock;
		Ref<LabelView> currentToast;
		Ref<Animation> animation;
		
	public:
		void show(Toast* toast)
		{
			MutexLocker locker(&lock);
			if (currentToast.isNotNull()) {
				currentToast->removeFromParent();
				currentToast.setNull();
				animation.setNull();
			}
			Ref<Font> font = toast->font;
			if (font.isNull()) {
				return;
			}
			Ref<View> parent = toast->parent;
			if (parent.isNull()) {
				Ref<MobileApp> app = MobileApp::getApp();
				if (app.isNotNull()) {
					parent = app->getContentView();
				} else {
					Ref<UIApp> ui = UIApp::getApp();
					if (ui.isNotNull()) {
						Ref<Window> window = ui->getMainWindow();
						if (window.isNotNull()) {
							parent = window->getContentView();
						}
					}
				}
			}
			if (parent.isNull()) {
				return;
			}
			Ref<LabelView> view = new LabelView;
			if (view.isNull()) {
				return;
			}
			view->setText(toast->text, UIUpdateMode::Init);
			view->setMultiLine(MultiLineMode::WordWrap, UIUpdateMode::Init);
			view->setWidthWrapping(UIUpdateMode::Init);
			view->setHeightWrapping(UIUpdateMode::Init);
			view->setMaximumWidth((sl_ui_len)(parent->getWidth() * 0.9f), UIUpdateMode::Init);
			view->setFont(font, UIUpdateMode::Init);
			view->setTextColor(Color::White, UIUpdateMode::Init);
			view->setBackgroundColor(Color(0, 0, 0, 160), UIUpdateMode::Init);
			view->setBoundRadius(font->getFontHeight() / 3, UIUpdateMode::Init);
			view->setPadding((sl_ui_pos)(font->getFontHeight() / 3), UIUpdateMode::Init);
			view->setCenterInParent(UIUpdateMode::Init);
			view->setClipping(sl_true, UIUpdateMode::Init);
			currentToast = view;
			parent->addChild(view);
			animation = view->startAlphaAnimation(0, 1, 0.3f, sl_null, AnimationCurve::Linear, AnimationFlags::NotSelfAlive);
			
			auto weak = ToWeakRef(view);
			UI::dispatchToUiThread([this, weak]() {
				auto view = ToRef(weak);
				if (view.isNull()) {
					return;
				}
				MutexLocker locker(&lock);
				if (currentToast.isNotNull()) {
					animation = currentToast->startAlphaAnimation(1, 0, 0.3f, [this, weak]() {
						auto view = ToRef(weak);
						if (view.isNull()) {
							return;
						}
						MutexLocker locker(&lock);
						if (currentToast.isNotNull()) {
							currentToast->removeFromParent();
							currentToast.setNull();
							animation.setNull();
						}
					}, AnimationCurve::Linear, AnimationFlags::NotSelfAlive);
				}
			}, (sl_uint32)(toast->duration * 1000));
		}
		
	};
	SLIB_SAFE_STATIC_GETTER(_priv_ToastManager, _priv_getToastManager)
	
	void Toast::show()
	{
		_priv_ToastManager* manager = _priv_getToastManager();
		if (manager) {
			manager->show(this);
		}
	}
	
	void Toast::show(const String& text)
	{
		Toast toast;
		toast.text = text;
		toast.show();
	}
	
	float _g_priv_Toast_defaultDuration = 2.0f;
	
	float Toast::getDefaultDuration()
	{
		return _g_priv_Toast_defaultDuration;
	}
	
	void Toast::setDefaultDuration(float duration)
	{
		_g_priv_Toast_defaultDuration = duration;
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicRef<Font>, _g_priv_Toast_defaultFont)
	
	Ref<Font> Toast::getDefaultFont()
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_Toast_defaultFont)) {
			return sl_null;
		}
		if (_g_priv_Toast_defaultFont.isNull()) {
			_g_priv_Toast_defaultFont = Font::create("Arial", UI::dpToPixel(20));
		}
		return _g_priv_Toast_defaultFont;
	}
	
	void Toast::setDefaultFont(const Ref<Font>& font)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_Toast_defaultFont)) {
			return;
		}
		_g_priv_Toast_defaultFont = font;
	}
	
}
