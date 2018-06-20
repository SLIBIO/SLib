/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/common_dialogs.h"

#include "slib/ui/core.h"
#include "slib/ui/mobile_app.h"

#include "slib/ui/platform.h"

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
	
	sl_bool AlertDialog::_show()
	{
		AlertDialogType type = this->type;
		NSString* caption = Apple::getNSStringFromString(this->caption);
		NSString* text = Apple::getNSStringFromString(this->text);
		NSString* titleOk = Apple::getNSStringFromString(this->titleOk);
		if ([titleOk length] == 0) {
			titleOk = @"OK";
		}
		NSString* titleCancel = Apple::getNSStringFromString(this->titleCancel);
		if ([titleCancel length] == 0) {
			titleCancel = @"Cancel";
		}
		NSString* titleYes = Apple::getNSStringFromString(this->titleYes);
		if ([titleYes length] == 0) {
			titleYes = @"Yes";
		}
		NSString* titleNo = Apple::getNSStringFromString(this->titleNo);
		if ([titleNo length] == 0) {
			titleNo = @"No";
		}
		Function<void()> onOk = this->onOk;
		Function<void()> onCancel = this->onCancel;
		Function<void()> onYes = this->onYes;
		Function<void()> onNo = this->onNo;
		
		UIAlertController* alert = [UIAlertController alertControllerWithTitle:caption message:text preferredStyle:UIAlertControllerStyleAlert];
		
		if (alert != nil) {
			
			if (type == AlertDialogType::OkCancel) {
				UIAlertAction* actionOK = [UIAlertAction actionWithTitle:titleOk style:UIAlertActionStyleDefault handler:
										   ^(UIAlertAction *) {
											   onOk();
										   }];
				UIAlertAction* actionCancel = [UIAlertAction actionWithTitle:titleCancel style:UIAlertActionStyleCancel handler:
											   ^(UIAlertAction *) {
												   onCancel();
											   }];
				[alert addAction:actionOK];
				[alert addAction:actionCancel];
			} else if (type == AlertDialogType::YesNo) {
				UIAlertAction* actionYes = [UIAlertAction actionWithTitle:titleYes style:UIAlertActionStyleDefault handler:
											^(UIAlertAction *) {
												onYes();
											}];
				UIAlertAction* actionNo = [UIAlertAction actionWithTitle:titleNo style:UIAlertActionStyleDestructive handler:
										   ^(UIAlertAction *) {
											   onNo();
										   }];
				[alert addAction:actionYes];
				[alert addAction:actionNo];
			} else if (type == AlertDialogType::YesNoCancel) {
				UIAlertAction* actionYes = [UIAlertAction actionWithTitle:titleYes style:UIAlertActionStyleDefault handler:
											^(UIAlertAction *) {
												onYes();
											}];
				UIAlertAction* actionNo = [UIAlertAction actionWithTitle:titleNo style:UIAlertActionStyleDestructive handler:
										   ^(UIAlertAction *) {
											   onNo();
										   }];
				UIAlertAction* actionCancel = [UIAlertAction actionWithTitle:titleCancel style:UIAlertActionStyleCancel handler:
											   ^(UIAlertAction *) {
												   onCancel();
											   }];
				[alert addAction:actionYes];
				[alert addAction:actionNo];
				[alert addAction:actionCancel];
			} else {
				UIAlertAction* actionOK = [UIAlertAction actionWithTitle:titleOk style:UIAlertActionStyleCancel handler:
										   ^(UIAlertAction *) {
											   onOk();
										   }];
				[alert addAction:actionOK];
			}
			
			UIWindow* window = nil;
			Ref<Window> parentWindow = this->parent;
			if (parentWindow.isNull()) {
				Ref<MobileApp> app = MobileApp::getApp();
				if (app.isNotNull()) {
					parentWindow = app->getMainWindow();
				}
			}
			if (parentWindow.isNotNull()) {
				Ref<WindowInstance> instance = parentWindow->getWindowInstance();
				UIView* view = UIPlatform::getWindowHandle(instance.get());
				if ([view isKindOfClass:[UIWindow class]]) {
					window = (UIWindow*)view;
				}
			}
			if (window == nil) {
				window = UIPlatform::getKeyWindow();
			}
			if (window != nil) {
				UIViewController* rootController = [window rootViewController];
				if (rootController != nil) {
					[rootController presentViewController:alert animated:YES completion:nil];
					return sl_true;
				}
			}
		}
		
		return sl_false;
		
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
