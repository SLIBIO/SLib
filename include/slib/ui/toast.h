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

#ifndef CHECKHEADER_SLIB_UI_TOAST
#define CHECKHEADER_SLIB_UI_TOAST

#include "definition.h"

#include "view.h"

namespace slib
{

	class Toast
	{
	public:
		Ref<View> parent;
		String text;
		sl_uint32 duration; // milliseconds
		Ref<Font> font;
		Alignment gravity;
		UIEdgeInsets margin;
		
	public:
		Toast();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(Toast)
		
	public:
		void show();
		
	public:
		static void show(const String& text);
		
		static void show(const String& text, sl_uint32 durationMillis);
		
		static void show(const String& text, sl_uint32 durationMillis, const Ref<Font>& font);
		
		static void show(const String& text, sl_uint32 durationMillis, const Ref<Font>& font, const Alignment& gravity);
		
		static void show(const String& text, sl_uint32 durationMillis, const Ref<Font>& font, const Alignment& gravity, const UIEdgeInsets& margin);

	public:
		// milliseconds
		static sl_uint32 getDefaultDuration();
		
		// milliseconds
		static void setDefaultDuration(sl_uint32 duration);
		
		static Ref<Font> getDefaultFont();
		
		static void setDefaultFont(const Ref<Font>& font);
		
	};
	
}

#endif
