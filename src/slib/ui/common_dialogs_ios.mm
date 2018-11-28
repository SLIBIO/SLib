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
		AlertDialogButtons buttons = this->buttons;
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
			
			if (buttons == AlertDialogButtons::OkCancel) {
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
			} else if (buttons == AlertDialogButtons::YesNo) {
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
			} else if (buttons == AlertDialogButtons::YesNoCancel) {
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
