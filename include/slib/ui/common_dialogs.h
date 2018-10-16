/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_COMMON_DIALOGS
#define CHECKHEADER_SLIB_UI_COMMON_DIALOGS

#include "definition.h"

#include "constants.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/function.h"

namespace slib
{

	enum class AlertDialogButtons
	{
		Ok = 0,
		OkCancel = 1,
		YesNo = 2,
		YesNoCancel = 3
	};

	enum class AlertDialogIcon
	{
		None = 0,
		Information = 1,
		Warning = 2,
		Question = 3,
		Error = 4
	};
	
	class Window;

	class SLIB_EXPORT AlertDialog : public Referable
	{
	public:
		AlertDialog();
		
		AlertDialog(const AlertDialog& other);
		
		~AlertDialog();

	public:
		DialogResult run();
		
		void show();

	public:
		Ref<Window> parent;
		String caption;
		String text;
		AlertDialogButtons buttons;
		AlertDialogIcon icon;

		String titleOk;
		String titleCancel;
		String titleYes;
		String titleNo;
		
		Function<void()> onOk;
		Function<void()> onCancel;
		Function<void()> onYes;
		Function<void()> onNo;
		
	public:
		DialogResult _run();
		
		sl_bool _show();
		
	protected:
		DialogResult _runOnUiThread();
		
		DialogResult _runByShow();
		
		void _showOnUiThread();
		
		void _showByRun();
		
		AlertDialog* getReferable();

	};

	enum class FileDialogType
	{
		SaveFile = 1,
		OpenFile = 2,
		OpenFiles = 3,
		SelectDirectory = 4
	};

	class SLIB_EXPORT FileDialog : public Referable
	{
	public:
		static List<String> openFiles(const Ref<Window>& parent);

		static String openFile(const Ref<Window>& parent);
		
		static String saveFile(const Ref<Window>& parent);
		
		static String selectDirectory(const Ref<Window>& parent);
		
	public:
		FileDialog();
		
		FileDialog(const FileDialog& other);
		
		~FileDialog();

	public:
		sl_bool run();

		// To specify multiple filter patterns for a single display string, use a semicolon to separate the patterns (for example, "*.TXT;*.DOC;*.BAK").
		void addFilter(const String& title, const String& patterns);
		
	public:
		FileDialogType type;
		Ref<Window> parent;
		String title;
		sl_bool flagShowHiddenFiles;

		String defaultFileExt;
		struct Filter
		{
			String title;

			// To specify multiple filter patterns for a single display string, use a semicolon to separate the patterns (for example, "*.TXT;*.DOC;*.BAK").
			String patterns;
		};
		List<Filter> filters;

		String selectedPath;
		List<String> selectedPaths;
		
	public:
		sl_bool _run();
		
	protected:
		sl_bool _runOnUiThread();
		
	};

}

#endif
