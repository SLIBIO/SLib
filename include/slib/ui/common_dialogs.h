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

#ifndef CHECKHEADER_SLIB_UI_COMMON_DIALOGS
#define CHECKHEADER_SLIB_UI_COMMON_DIALOGS

#include "definition.h"

#include "constants.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/function.h"

#include "../ui/window.h"
#include "../ui/view.h"

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
	
	class SLIB_EXPORT AlertDialog : public Referable
	{
	public:
		AlertDialog();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AlertDialog)
		
	public:
		DialogResult run();
		
		void show();

	public:
		Ref<Window> parent;
		String caption;
		String text;
		sl_bool flagHyperText;
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
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(FileDialog)

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

	
	class Toast
	{
	public:
		Ref<View> parent;
		String text;
		float duration; // Seconds
		Ref<Font> font;
		
	public:
		Toast();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(Toast)
		
	public:
		void show();
		
	public:
		static void show(const String& text);
		
	public:
		static float getDefaultDuration();
		
		static void setDefaultDuration(float duration);
		
		static Ref<Font> getDefaultFont();
		
		static void setDefaultFont(const Ref<Font>& font);
		
	};
	
}

#endif
