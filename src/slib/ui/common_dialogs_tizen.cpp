/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "../../../inc/slib/ui/common_dialogs.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/platform.h"

#include <Elementary.h>
#include <efl_extension.h>

namespace slib
{

	DialogResult AlertDialog::run()
	{
		return _runByShow();
	}
	
	DialogResult AlertDialog::_run()
	{
		return DialogResult::Cancel;
	}
	
	void AlertDialog::show()
	{
		_showOnUiThread();
	}

	struct _AlertDialog_Container
	{
		Evas_Object* popup;
		Ref<AlertDialog> alert;
		DialogResult result;
	};

	static void _AlertDialog_ok_cb(void *data, Evas_Object *obj, void *event_info)
	{
		_AlertDialog_Container* container = reinterpret_cast<_AlertDialog_Container*>(data);
		container->result = DialogResult::Ok;
		::elm_popup_dismiss(container->popup);
	}

	static void _AlertDialog_yes_cb(void *data, Evas_Object *obj, void *event_info)
	{
		_AlertDialog_Container* container = reinterpret_cast<_AlertDialog_Container*>(data);
		container->result = DialogResult::Yes;
		::elm_popup_dismiss(container->popup);
	}

	static void _AlertDialog_cancel_cb(void *data, Evas_Object *obj, void *event_info)
	{
		_AlertDialog_Container* container = reinterpret_cast<_AlertDialog_Container*>(data);
		container->result = DialogResult::Cancel;
		::elm_popup_dismiss(container->popup);
	}

	static void _AlertDialog_no_cb(void *data, Evas_Object *obj, void *event_info)
	{
		_AlertDialog_Container* container = reinterpret_cast<_AlertDialog_Container*>(data);
		container->result = DialogResult::No;
		::elm_popup_dismiss(container->popup);
	}
	
	static void _AlertDialog_dismissed_cb(void *data, Evas_Object *obj, void *event_info)
	{
		_AlertDialog_Container* container = reinterpret_cast<_AlertDialog_Container*>(data);
		::evas_object_del(container->popup);
		switch (container->result) {
			case DialogResult::Ok:
				container->alert->onOk();
				break;
			case DialogResult::Yes:
				container->alert->onYes();
				break;
			case DialogResult::No:
				container->alert->onNo();
				break;
			default:
				container->alert->onCancel();
				break;
		}
		delete container;
	}

	sl_bool AlertDialog::_show()
	{
		AlertDialogType type = this->type;

		Evas_Object* win;
		Ref<Window> parent = this->parent;
		Ref<WindowInstance> instance;
		if (parent.isNotNull()) {
			if (parent.isNotNull()) {
				instance = parent->getWindowInstance();
			}
			win = UIPlatform::getWindowHandle(instance.get());
		} else {
			win = UIPlatform::getMainWindow();
		}

		if (!win) {
			return sl_false;
		}

		Evas_Object* popup = ::elm_popup_add(win);
		if (!popup) {
			return sl_false;
		}

		_AlertDialog_Container* container = new _AlertDialog_Container;
		if (!container) {
			return sl_false;
		}
		container->popup = popup;
		container->alert = this;
		container->result = DialogResult::Cancel;

		::evas_object_layer_set(popup, EVAS_LAYER_MAX);
		::elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
		::evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

		::elm_object_part_text_set(popup, "title,text", caption.getData());
		::elm_object_text_set(popup, text.getData());

		String titleOk = this->titleOk;
		if (titleOk.isEmpty()) {
			titleOk = "OK";
		}
		String titleCancel = this->titleCancel;
		if (titleCancel.isEmpty()) {
			titleCancel = "Cancel";
		}
		String titleYes = this->titleYes;
		if (titleYes.isEmpty()) {
			titleYes = "Yes";
		}
		String titleNo = this->titleNo;
		if (titleNo.isEmpty()) {
			titleNo = "No";
		}
		
		if (type == AlertDialogType::OkCancel) {
			Evas_Object* button1 = ::elm_button_add(popup);
			::elm_object_text_set(button1, titleOk.getData());
			::evas_object_smart_callback_add(button1, "clicked", _AlertDialog_ok_cb, container);
			::elm_object_part_content_set(popup, "button1", button1);
			Evas_Object* button2 = ::elm_button_add(popup);
			::elm_object_text_set(button2, titleCancel.getData());
			::evas_object_smart_callback_add(button2, "clicked", _AlertDialog_cancel_cb, container);
			::elm_object_part_content_set(popup, "button2", button2);
			::eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _AlertDialog_cancel_cb, container);
			::evas_object_smart_callback_add(popup, "block,clicked", _AlertDialog_cancel_cb, container);
		} else if (type == AlertDialogType::YesNo) {
			Evas_Object* button1 = ::elm_button_add(popup);
			::elm_object_text_set(button1, titleYes.getData());
			::evas_object_smart_callback_add(button1, "clicked", _AlertDialog_yes_cb, container);
			::elm_object_part_content_set(popup, "button1", button1);
			Evas_Object* button2 = ::elm_button_add(popup);
			::elm_object_text_set(button2, titleNo.getData());
			::evas_object_smart_callback_add(button2, "clicked", _AlertDialog_no_cb, container);
			::elm_object_part_content_set(popup, "button2", button2);
		} else if (type == AlertDialogType::YesNoCancel) {
			Evas_Object* button1 = ::elm_button_add(popup);
			::elm_object_text_set(button1, titleYes.getData());
			::evas_object_smart_callback_add(button1, "clicked", _AlertDialog_yes_cb, container);
			::elm_object_part_content_set(popup, "button1", button1);
			Evas_Object* button2 = ::elm_button_add(popup);
			::elm_object_text_set(button2, titleNo.getData());
			::evas_object_smart_callback_add(button2, "clicked", _AlertDialog_no_cb, container);
			::elm_object_part_content_set(popup, "button2", button2);
			Evas_Object* button3 = ::elm_button_add(popup);
			::elm_object_text_set(button3, titleCancel.getData());
			::evas_object_smart_callback_add(button3, "clicked", _AlertDialog_cancel_cb, container);
			::elm_object_part_content_set(popup, "button3", button3);
			::eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _AlertDialog_cancel_cb, container);
			::evas_object_smart_callback_add(popup, "block,clicked", _AlertDialog_cancel_cb, container);
		} else {
			Evas_Object* button1 = ::elm_button_add(popup);
			::elm_object_text_set(button1, titleOk.getData());
			::evas_object_smart_callback_add(button1, "clicked", _AlertDialog_ok_cb, container);
			::elm_object_part_content_set(popup, "button1", button1);
			::eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _AlertDialog_ok_cb, container);
			::evas_object_smart_callback_add(popup, "block,clicked", _AlertDialog_ok_cb, container);
		}
		
		::evas_object_smart_callback_add(popup, "dismissed", _AlertDialog_dismissed_cb, &container);
		::evas_object_show(popup);

		return sl_true;

	}
	
	sl_bool FileDialog::run()
	{
		return sl_false;
	}
	
	sl_bool FileDialog::_run()
	{
		return sl_false;
	}

}

#endif
