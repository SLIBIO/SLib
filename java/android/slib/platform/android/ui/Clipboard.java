/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

package slib.platform.android.ui;

import android.app.Activity;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;

import slib.platform.android.Logger;

public class Clipboard {

	public static boolean hasText(final Activity activity) {
		try {
			ClipboardManager clipboard = (ClipboardManager)(activity.getSystemService(Context.CLIPBOARD_SERVICE));
			if (clipboard != null) {
				return clipboard.hasPrimaryClip();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

	public static String getText(final Activity activity) {
		try {
			ClipboardManager clipboard = (ClipboardManager)(activity.getSystemService(Context.CLIPBOARD_SERVICE));
			if (clipboard != null) {
				ClipData data = clipboard.getPrimaryClip();
				if (data != null) {
					if (data.getItemCount() > 0) {
						ClipData.Item item = data.getItemAt(0);
						if (item != null) {
							return item.getText().toString();
						}
					}
				}
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
		return null;
	}

	public static void setText(final Activity activity, String text) {
		try {
			ClipboardManager clipboard = (ClipboardManager)(activity.getSystemService(Context.CLIPBOARD_SERVICE));
			if (clipboard != null) {
				ClipData clip = ClipData.newPlainText("", text);
				clipboard.setPrimaryClip(clip);
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

}