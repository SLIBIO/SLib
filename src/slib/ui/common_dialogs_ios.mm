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
#include "slib/ui/resource.h"
#include "slib/ui/platform.h"

#include "../resources.h"

@interface SLIBAlertDialogLabelHelper : NSObject
{
}
+ (SLIBAlertDialogLabelHelper*)sharedInstance;
- (void)handleTapOnLabel:(UITapGestureRecognizer *)tapGesture;
@end

namespace slib
{
	
	namespace priv
	{
		namespace alert_dialog
		{
			
			static void EnableLink(UIView* view)
			{
				for (UIView* subview in view.subviews) {
					if ([subview isKindOfClass:[UILabel class]]) {
						UILabel* label = (UILabel*)subview;
						label.userInteractionEnabled = YES;
						SLIBAlertDialogLabelHelper* target = [SLIBAlertDialogLabelHelper sharedInstance];
						[label addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:target action:@selector(handleTapOnLabel:)]];
					} else {
						priv::alert_dialog::EnableLink(subview);
					}
				}
			}
			
		}
	}
	
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
		UIViewController* controller = UIPlatform::getCurrentViewController(parent);
		if (controller == nil) {
			return sl_false;
		}
		if (controller.presentedViewController != nil) {
			return sl_false;
		}

		AlertDialogButtons buttons = this->buttons;
		NSString* caption = Apple::getNSStringFromString(this->caption);
		NSString* text = Apple::getNSStringFromString(this->text);
		NSString* titleOk = Apple::getNSStringFromString(this->titleOk);
		if ([titleOk length] == 0) {
			titleOk = Apple::getSystemLocalizedNSString(@"OK");
		}
		NSString* titleCancel = Apple::getNSStringFromString(this->titleCancel);
		if ([titleCancel length] == 0) {
			titleCancel = Apple::getSystemLocalizedNSString(@"Cancel");
		}
		NSString* titleYes = Apple::getNSStringFromString(this->titleYes);
		if ([titleYes length] == 0) {
			titleYes = Apple::getNSStringFromString(string::yes::get());
		}
		NSString* titleNo = Apple::getNSStringFromString(this->titleNo);
		if ([titleNo length] == 0) {
			titleNo = Apple::getSystemLocalizedNSString(@"No");
		}
		Function<void()> onOk = SLIB_BIND_REF(void(), AlertDialog, _onResult, this, DialogResult::Ok);
		Function<void()> onCancel = SLIB_BIND_REF(void(), AlertDialog, _onResult, this, DialogResult::Cancel);
		Function<void()> onYes = SLIB_BIND_REF(void(), AlertDialog, _onResult, this, DialogResult::Yes);
		Function<void()> onNo = SLIB_BIND_REF(void(), AlertDialog, _onResult, this, DialogResult::No);
		
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
			
			if (flagHyperText) {
				@try {
					NSMutableAttributedString* hyperCaption = [[NSMutableAttributedString alloc] initWithString:caption attributes:@{ NSFontAttributeName: [UIFont fontWithName:@"Helvetica Neue" size:20] }];
					if (hyperCaption != nil) {
						[alert setValue:hyperCaption forKey: @"attributedTitle"];
					}

					NSString* htmlText = [NSString stringWithFormat:@"<span style=\"font-family: Helvetica Neue; font-size: 16\">%@</span>", text];
					NSMutableAttributedString* hyperText = [[NSMutableAttributedString alloc] initWithData:[htmlText dataUsingEncoding:NSUTF8StringEncoding] options:@{NSDocumentTypeDocumentAttribute: NSHTMLTextDocumentType, NSCharacterEncodingDocumentAttribute: @(NSUTF8StringEncoding)} documentAttributes:nil error:nil];
					if (hyperText != nil) {
						[alert setValue:hyperText forKey: @"attributedMessage"];
						UIView* view = alert.view;
						if (view != nil) {
							priv::alert_dialog::EnableLink(view);
						}
					}
				} @catch (NSException*) {}
			}
			[controller presentViewController:alert animated:YES completion:nil];
			return sl_true;
		}
		
		return sl_false;
		
	}
	
}

@implementation SLIBAlertDialogLabelHelper

+ (SLIBAlertDialogLabelHelper*)sharedInstance
{
	static SLIBAlertDialogLabelHelper* instance = [[SLIBAlertDialogLabelHelper alloc] init];
	return instance;
}

- (void)handleTapOnLabel:(UITapGestureRecognizer *)tapGesture
{
	UILabel* label = (UILabel*)(tapGesture.view);
	
	NSAttributedString* text = label.attributedText;
	CGSize sizeLabel = label.bounds.size;

	NSLayoutManager *layoutManager = [[NSLayoutManager alloc] init];
	NSTextContainer *textContainer = [[NSTextContainer alloc] initWithSize:CGSizeZero];
	NSTextStorage *textStorage = [[NSTextStorage alloc] initWithAttributedString:text];
	
	[layoutManager addTextContainer:textContainer];
	[textStorage addLayoutManager:layoutManager];
	
	textContainer.lineFragmentPadding = 0.0;
	textContainer.lineBreakMode = label.lineBreakMode;
	textContainer.maximumNumberOfLines = label.numberOfLines;
	textContainer.size = sizeLabel;

	CGPoint pos = [tapGesture locationInView:label];
	CGRect rcText = [layoutManager usedRectForTextContainer:textContainer];
	pos.x -= (sizeLabel.width - rcText.size.width) / 2;
	pos.y -= (sizeLabel.height - rcText.size.height) / 2;
	NSUInteger index = [layoutManager characterIndexForPoint:pos inTextContainer:textContainer fractionOfDistanceBetweenInsertionPoints:nil];
	
	NSDictionary* attrs = [text attributesAtIndex:index effectiveRange:nil];
	NSURL* url = attrs[NSLinkAttributeName];
	if (url != nil) {
#ifdef SLIB_PLATFORM_IS_IOS_CATALYST
		[[UIApplication sharedApplication] openURL:url options:@{} completionHandler:nil];
#else
		[[UIApplication sharedApplication] openURL:url];
#endif
	}
}

@end

#endif
