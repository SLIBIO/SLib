#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/common_dialogs.h"
#include "../../../inc/slib/core/file.h"

#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

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
	
	DialogResult result2 = DialogResult::Cancel;
	DialogResult result3 = DialogResult::Cancel;

	NSAlert* alert = [[NSAlert alloc] init];
	[alert setMessageText:text];
	[alert setAlertStyle:NSInformationalAlertStyle];
	[[alert window] setTitle:caption];
	if (type == AlertDialogType::OkCancel) {
		[alert addButtonWithTitle:titleOk];
		NSButton* btnCancel = [alert addButtonWithTitle:titleCancel];
		[btnCancel setKeyEquivalent:@"Cancel"];
		result2 = DialogResult::Cancel;
	} else if (type == AlertDialogType::YesNo) {
		[alert addButtonWithTitle:titleYes];
		NSButton* btnNo = [alert addButtonWithTitle:titleNo];
		[btnNo setKeyEquivalent:@"Don't Save"];
		result2 = DialogResult::No;
	} else if (type == AlertDialogType::YesNoCancel) {
		[alert addButtonWithTitle:titleYes];
		NSButton* btnCancel = [alert addButtonWithTitle:titleCancel];
		[btnCancel setKeyEquivalent:@"Cancel"];
		NSButton* btnNo = [alert addButtonWithTitle:titleNo];
		[btnNo setKeyEquivalent:@"Don't Save"];
		result2 = DialogResult::Cancel;
		result3 = DialogResult::No;
	} else {
		[alert addButtonWithTitle:titleOk];
	}
	
	NSWindow* hParent = UIPlatform::getWindowHandle(parent.ptr);
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
		return DialogResult::Ok;
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

void AlertDialog::_show()
{
}


/***************************************
		FileDialog
***************************************/

DialogResult FileDialog::run()
{
	return _runOnUiThread();
}

DialogResult FileDialog::_run()
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
		
		NSWindow* hParent = UIPlatform::getWindowHandle(parent.ptr);
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
					selectedPath = paths.getItemValue(0, String::null());
					selectedPaths = paths;
					return DialogResult::Ok;
				}
			} else {
				String path = Apple::getFilePathFromNSURL([panel URL]);
				if (path.isNotEmpty()) {
					selectedPath = path;
					selectedPaths = List<String>::createFromElement(path);
					return DialogResult::Ok;
				}
			}
		}
	}
	return DialogResult::Cancel;
}

SLIB_UI_NAMESPACE_END

#endif
