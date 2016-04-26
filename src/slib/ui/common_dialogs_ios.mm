#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/common_dialogs.h"
#include "../../../inc/slib/ui/core.h"

#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

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

void AlertDialog::_show()
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
	Ref<Runnable> onOk = this->onOk;
	Ref<Runnable> onCancel = this->onCancel;
	Ref<Runnable> onNo = this->onNo;
	
	UIAlertController* alert = [UIAlertController alertControllerWithTitle:caption message:text preferredStyle:UIAlertControllerStyleAlert];
	if (type == AlertDialogType::OkCancel) {
		UIAlertAction* actionOK = [UIAlertAction actionWithTitle:titleOk style:UIAlertActionStyleDefault handler:
								   ^(UIAlertAction *) {
									   [alert dismissViewControllerAnimated:YES completion:nil];
									   if (onOk.isNotNull()) {
										   onOk->run();
									   }
								   }];
		UIAlertAction* actionCancel = [UIAlertAction actionWithTitle:titleCancel style:UIAlertActionStyleCancel handler:
									   ^(UIAlertAction *) {
										   [alert dismissViewControllerAnimated:YES completion:nil];
										   if (onCancel.isNotNull()) {
											   onCancel->run();
										   }
									   }];
		[alert addAction:actionOK];
		[alert addAction:actionCancel];
	} else if (type == AlertDialogType::YesNo) {
		UIAlertAction* actionYes = [UIAlertAction actionWithTitle:titleYes style:UIAlertActionStyleDefault handler:
									^(UIAlertAction *) {
										[alert dismissViewControllerAnimated:YES completion:nil];
										if (onOk.isNotNull()) {
											onOk->run();
										}
									}];
		UIAlertAction* actionNo = [UIAlertAction actionWithTitle:titleNo style:UIAlertActionStyleDestructive handler:
								   ^(UIAlertAction *) {
									   [alert dismissViewControllerAnimated:YES completion:nil];
									   if (onNo.isNotNull()) {
										   onNo->run();
									   }
								   }];
		[alert addAction:actionYes];
		[alert addAction:actionNo];
	} else if (type == AlertDialogType::YesNoCancel) {
		UIAlertAction* actionYes = [UIAlertAction actionWithTitle:titleYes style:UIAlertActionStyleDefault handler:
									^(UIAlertAction *) {
										[alert dismissViewControllerAnimated:YES completion:nil];
										if (onOk.isNotNull()) {
											onOk->run();
										}
									}];
		UIAlertAction* actionNo = [UIAlertAction actionWithTitle:titleNo style:UIAlertActionStyleDestructive handler:
								   ^(UIAlertAction *) {
									   [alert dismissViewControllerAnimated:YES completion:nil];
									   if (onNo.isNotNull()) {
										   onNo->run();
									   }
								   }];
		UIAlertAction* actionCancel = [UIAlertAction actionWithTitle:titleCancel style:UIAlertActionStyleCancel handler:
									   ^(UIAlertAction *) {
										   [alert dismissViewControllerAnimated:YES completion:nil];
										   if (onCancel.isNotNull()) {
											   onCancel->run();
										   }
									   }];
		[alert addAction:actionYes];
		[alert addAction:actionNo];
		[alert addAction:actionCancel];
	} else {
		UIAlertAction* actionOK = [UIAlertAction actionWithTitle:titleOk style:UIAlertActionStyleCancel handler:
								   ^(UIAlertAction *) {
									   [alert dismissViewControllerAnimated:YES completion:nil];
									   if (onOk.isNotNull()) {
										   onOk->run();
									   }
								   }];
		[alert addAction:actionOK];
	}
	
	UIWindow* window = UIPlatform::getKeyWindow();
	if (window != nil) {
		UIViewController* rootController = [window rootViewController];
		[rootController presentViewController:alert animated:YES completion:nil];
	}
}

DialogResult FileDialog::run()
{
	return DialogResult::Cancel;
}

DialogResult FileDialog::_run()
{
	return DialogResult::Cancel;
}

SLIB_UI_NAMESPACE_END

#endif
