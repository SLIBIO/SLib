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

#if defined(SLIB_UI_IS_MACOS)

#include "slib/core/file.h"
#include "slib/ui/core.h"
#include "slib/ui/common_dialogs.h"
#include "slib/ui/platform.h"

#include "../resources.h"

namespace slib
{

/***************************************
		AlertDialog
***************************************/

	DialogResult AlertDialog::run()
	{
		return _runOnUiThread();
	}

	DialogResult AlertDialog::_run()
	{
		NSString* caption = Apple::getNSStringFromString(this->caption);
		NSString* text = Apple::getNSStringFromString(this->text);
		NSString* titleOk = Apple::getNSStringFromString(this->titleOk);
		if ([titleOk length] == 0) {
			titleOk = Apple::getNSStringFromString(string::ok::get());
		}
		NSString* titleCancel = Apple::getNSStringFromString(this->titleCancel);
		if ([titleCancel length] == 0) {
			titleCancel = Apple::getNSStringFromString(string::cancel::get());
		}
		NSString* titleYes = Apple::getNSStringFromString(this->titleYes);
		if ([titleYes length] == 0) {
			titleYes = Apple::getNSStringFromString(string::yes::get());
		}
		NSString* titleNo = Apple::getNSStringFromString(this->titleNo);
		if ([titleNo length] == 0) {
			titleNo = Apple::getNSStringFromString(string::no::get());
		}
		
		DialogResult result1 = DialogResult::Cancel;
		DialogResult result2 = DialogResult::Cancel;
		DialogResult result3 = DialogResult::Cancel;

		NSAlert* alert = [[NSAlert alloc] init];
		[alert setMessageText:text];
		NSAlertStyle style;
		switch (icon) {
			case AlertDialogIcon::Error:
				style = NSAlertStyleCritical;
				break;
			case AlertDialogIcon::Warning:
				style = NSAlertStyleWarning;
				break;
			default:
				style = NSAlertStyleInformational;
				break;
		}
		[alert setAlertStyle:style];
		[[alert window] setTitle:caption];
		if (buttons == AlertDialogButtons::OkCancel) {
			[alert addButtonWithTitle:titleOk];
			NSButton* btnCancel = [alert addButtonWithTitle:titleCancel];
			[btnCancel setKeyEquivalent:@"Cancel"];
			result1 = DialogResult::Ok;
			result2 = DialogResult::Cancel;
		} else if (buttons == AlertDialogButtons::YesNo) {
			[alert addButtonWithTitle:titleYes];
			NSButton* btnNo = [alert addButtonWithTitle:titleNo];
			[btnNo setKeyEquivalent:@"Don't Save"];
			result1 = DialogResult::Yes;
			result2 = DialogResult::No;
		} else if (buttons == AlertDialogButtons::YesNoCancel) {
			[alert addButtonWithTitle:titleYes];
			NSButton* btnCancel = [alert addButtonWithTitle:titleCancel];
			[btnCancel setKeyEquivalent:@"Cancel"];
			NSButton* btnNo = [alert addButtonWithTitle:titleNo];
			[btnNo setKeyEquivalent:@"Don't Save"];
			result1 = DialogResult::Yes;
			result2 = DialogResult::Cancel;
			result3 = DialogResult::No;
		} else {
			[alert addButtonWithTitle:titleOk];
			result1 = DialogResult::Ok;
		}
		
		NSWindow* hParent = UIPlatform::getWindowHandle(parent.get());
		__block NSInteger result;
		if (hParent != nil) {
			[alert beginSheetModalForWindow:hParent completionHandler:^(NSInteger n){
				result = n;
				UI::quitLoop();
			}];
			UI::runLoop();
		} else {
			result = [alert runModal];
		}
		if (result == NSAlertFirstButtonReturn) {
			return result1;
		} else if (result == NSAlertSecondButtonReturn) {
			return result2;
		} else if (result == NSAlertThirdButtonReturn) {
			return result3;
		}
		return DialogResult::Cancel;
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

	sl_bool FileDialog::_run()
	{
		NSSavePanel* panel;
		if (type == FileDialogType::SelectDirectory) {
			NSOpenPanel* openPanel = [NSOpenPanel openPanel];
			if (openPanel != nil) {
				[openPanel setCanChooseDirectories:YES];
				[openPanel setCanChooseFiles:NO];
				[openPanel setAllowsMultipleSelection:NO];
			}
			panel = openPanel;
		} else if (type == FileDialogType::OpenFile) {
			NSOpenPanel* openPanel = [NSOpenPanel openPanel];
			if (openPanel != nil) {
				[openPanel setCanChooseDirectories:NO];
				[openPanel setCanChooseFiles:YES];
				[openPanel setAllowsMultipleSelection:NO];
			}
			panel = openPanel;
		} else if (type == FileDialogType::OpenFiles) {
			NSOpenPanel* openPanel = [NSOpenPanel openPanel];
			if (openPanel != nil) {
				[openPanel setCanChooseDirectories:NO];
				[openPanel setCanChooseFiles:YES];
				[openPanel setAllowsMultipleSelection:YES];
			}
			panel = openPanel;
		} else if (type == FileDialogType::SaveFile) {
			panel = [NSSavePanel savePanel];
		}
		if (panel != nil) {
			
			[panel setCanCreateDirectories:YES];
			if (title.isNotEmpty()) {
				[panel setTitle:Apple::getNSStringFromString(title)];
			}
			[panel setShowsHiddenFiles:(flagShowHiddenFiles?YES:NO)];
			[panel setAllowsOtherFileTypes:YES];
			[panel setTreatsFilePackagesAsDirectories:YES];
			[panel setExtensionHidden:NO];
			
			String initialDir;
			String fileName;
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
				NSString* nsInitialDir = Apple::getNSStringFromString(initialDir);
				NSURL* url = [NSURL fileURLWithPath:nsInitialDir isDirectory:YES];
				[panel setDirectoryURL:url];
			}
			if (fileName.isNotEmpty()) {
				[panel setNameFieldStringValue:(Apple::getNSStringFromString(fileName))];
			}
			
			NSWindow* hParent = UIPlatform::getWindowHandle(parent.get());
			__block NSInteger result;
			if (hParent != nil) {
				[panel beginSheetModalForWindow:hParent completionHandler:^(NSInteger n){
					result = n;
					UI::quitLoop();
				}];
				UI::runLoop();
			} else {
				result = [panel runModal];
			}
			if (result == NSFileHandlingPanelOKButton) {
				if ([panel isKindOfClass:[NSOpenPanel class]]) {
					NSOpenPanel* openPanel = (NSOpenPanel*)panel;
					NSArray* urls = [openPanel URLs];
					List<String> paths;
					sl_size n = [urls count];
					for (sl_size i = 0; i < n; i++) {
						String path = Apple::getFilePathFromNSURL([urls objectAtIndex:i]);
						if (path.isNotEmpty()) {
							paths.add(path);
						}
					}
					if (paths.getCount() > 0) {
						selectedPath = paths.getValueAt(0);
						selectedPaths = paths;
						return sl_true;
					}
				} else {
					String path = Apple::getFilePathFromNSURL([panel URL]);
					if (path.isNotEmpty()) {
						selectedPath = path;
						selectedPaths = List<String>::createFromElement(path);
						return sl_true;
					}
				}
			}
		}
		return sl_false;
	}

}

#endif
