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

#if defined(SLIB_UI_IS_MACOS)

#include "slib/core/platform_apple.h"

namespace slib
{
	
	sl_bool Clipboard::hasText()
	{
		NSPasteboard* board = [NSPasteboard generalPasteboard];
		if (board != nil) {
			for (NSPasteboardType type in [board types]) {
				if ([type isEqualToString:NSPasteboardTypeString]) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
	String Clipboard::getText()
	{
		NSPasteboard* board = [NSPasteboard generalPasteboard];
		if (board != nil) {
			NSString* text = [board stringForType:NSPasteboardTypeString];
			return Apple::getStringFromNSString(text);
		}
		return sl_null;
	}
	
	void Clipboard::setText(const String& text)
	{
		NSPasteboard* board = [NSPasteboard generalPasteboard];
		if (board != nil) {
			[board clearContents];
			NSString* str = Apple::getNSStringFromString(text);
			[board setString:str forType:NSPasteboardTypeString];
		}
	}
	
}

#endif
