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

#ifndef CHECKHEADER_SLIB_UI_GLOBAL_EVENT_MONITOR
#define CHECKHEADER_SLIB_UI_GLOBAL_EVENT_MONITOR

#include "definition.h"

#include "event.h"

namespace slib
{
	
	class GlobalEventMonitor
	{
	public:
		static void addMonitor(const Function<void(UIEvent*)>& callback);
		
		static void removeMonitor(const Function<void(UIEvent*)>& callback);
		
		static void addMouseMonitor(const Function<void(UIEvent*)>& callback);
		
		static void removeMouseMonitor(const Function<void(UIEvent*)>& callback);
		
		static void addKeyboardMonitor(const Function<void(UIEvent*)>& callback);
		
		static void removeKeyboardMonitor(const Function<void(UIEvent*)>& callback);
		
		static sl_bool isKeyboardAccessEnabled();
		
		static void authenticateKeyboardAccess();
		
	protected:
		enum {
			MASK_MOUSE = 0x0001,
			MASK_KEYBOARD = 0x0002
		};
		
		static sl_bool _updateMonitor(sl_uint32 mask);
		
		static void _onEvent(UIEvent*);
		
	};
	
}

#endif
