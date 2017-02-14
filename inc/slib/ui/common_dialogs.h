#ifndef CHECKHEADER_SLIB_UI_COMMON_DIALOGS
#define CHECKHEADER_SLIB_UI_COMMON_DIALOGS

#include "definition.h"

#include "constants.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/function.h"

namespace slib
{

	enum class AlertDialogType
	{
		Ok = 0,
		OkCancel = 1,
		YesNo = 2,
		YesNoCancel = 3
	};

	class Window;

	class SLIB_EXPORT AlertDialog : public Referable
	{
	public:
		static void run(const String& text);
		
		static void run(const Ref<Window>& parent, const String& text);
		
		static void run(const String& caption, const String& text);
		
		static void run(const Ref<Window>& parent, const String& caption, const String& text);
		
		static void show(const String& text);

		static void show(const String& text, const Function<void()>& onOk);
		
		static void show(const String& caption, const String& text, const Function<void()>& onOk);

		static DialogResult runOkCancel(const String& text);
		
		static DialogResult runOkCancel(const Ref<Window>& parent, const String& text);
		
		static DialogResult runOkCancel(const String& caption, const String& text);
		
		static DialogResult runOkCancel(const Ref<Window>& parent, const String& caption, const String& text);
		
		static void showOkCancel(const String& text, const Function<void()>& onOk, const Function<void()>& onCancel);
		
		static void showOkCancel(const String& caption, const String& text, const Function<void()>& onOk, const Function<void()>& onCancel);
		
		static void showOkCancel(const String& text, const Function<void()>& onOk);
		
		static void showOkCancel(const String& caption, const String& text, const Function<void()>& onOk);
		
		static DialogResult runYesNo(const String& text);
		
		static DialogResult runYesNo(const Ref<Window>& parent, const String& text);
		
		static DialogResult runYesNo(const String& caption, const String& text);
		
		static DialogResult runYesNo(const Ref<Window>& parent, const String& caption, const String& text);
		
		static void showYesNo(const String& text, const Function<void()>& onYes, const Function<void()>& onNo);
		
		static void showYesNo(const String& caption, const String& text, const Function<void()>& onYes, const Function<void()>& onNo);
		
		static void showYesNo(const String& text, const Function<void()>& onYes);
		
		static void showYesNo(const String& caption, const String& text, const Function<void()>& onYes);
		
		static DialogResult runYesNoCancel(const String& text);
		
		static DialogResult runYesNoCancel(const Ref<Window>& parent, const String& text);
		
		static DialogResult runYesNoCancel(const String& caption, const String& text);
		
		static DialogResult runYesNoCancel(const Ref<Window>& parent, const String& caption, const String& text);
		
		static void showYesNoCancel(const String& text, const Function<void()>& onYes, const Function<void()>& onNo, const Function<void()>& onCancel);
		
		static void showYesNoCancel(const String& caption, const String& text, const Function<void()>& onYes, const Function<void()>& onNo, const Function<void()>& onCancel);
		
		static void showYesNoCancel(const String& text, const Function<void()>& onYes, const Function<void()>& onNo);
		
		static void showYesNoCancel(const String& caption, const String& text, const Function<void()>& onYes, const Function<void()>& onNo);
		
	public:
		AlertDialog();
		
		AlertDialog(const AlertDialog& other);
		
	public:
		DialogResult run();
		
		void show();

	public:
		AlertDialogType type;
		Ref<Window> parent;
		String caption;
		String text;
		
		String titleOk;
		String titleCancel;
		String titleYes;
		String titleNo;
		
		Function<void()> onOk;
		Function<void()> onCancel;
		Function<void()> onNo;
		
	public:
		DialogResult _run();
		
		void _show();
		
	protected:
		DialogResult _runOnUiThread();
		
		DialogResult _runByShow();
		
		void _showOnUiThread();
		
		void _showByRun();
		
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
		
	public:
		sl_bool run();

		// To specify multiple filter patterns for a single display string, use a semicolon to separate the patterns (for example, "*.TXT;*.DOC;*.BAK").
		void addFilter(const String& title, const String& patterns);
		
	public:
		FileDialogType type;
		Ref<Window> parent;
		String title;
		sl_bool flagShowHiddenFiles;

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
