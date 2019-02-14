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

#include "slib/ui/clipboard.h"

#if defined(SLIB_UI_IS_GTK)

#include "slib/ui/platform.h"

namespace slib
{
	
	sl_bool Clipboard::hasText()
	{
		GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
		if (clipboard) {
			return gtk_clipboard_wait_is_text_available(clipboard) ? sl_true : sl_false;
		}
		return sl_false;
	}
	
	String Clipboard::getText()
	{
		GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
		if (clipboard) {
			gchar* sz = gtk_clipboard_wait_for_text(clipboard);
			if (sz) {
				String ret = sz;
				g_free(sz);
				return ret;
			}
		}
		return sl_null;
	}
	
	void Clipboard::setText(const String& text)
	{
		GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
		if (clipboard) {
			gtk_clipboard_clear(clipboard);
			gtk_clipboard_set_text(clipboard, text.getData(), text.getLength());
		}
	}

}

#endif
