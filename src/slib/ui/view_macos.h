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

#ifndef CHECKHEADER_SLIB_SRC_UI_VIEW_MACOS
#define CHECKHEADER_SLIB_SRC_UI_VIEW_MACOS

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/view_macos.h"

@interface SLIBViewBaseHandle : NSView
{
	@public sl_bool m_flagOpaque;
	@public sl_bool m_flagClipping;
	@public sl_bool m_flagDrawing;
}
@end

@interface SLIBViewHandle : SLIBViewBaseHandle
{
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
}
@end

#define MACOS_VIEW_DEFINE_ON_FOCUS \
	- (BOOL)becomeFirstResponder \
	{ \
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance; \
		if (instance.isNotNull()) { \
			instance->onSetFocus(); \
		} \
		return [super becomeFirstResponder]; \
	}

#define MACOS_VIEW_DEFINE_ON_KEY \
	- (void)keyDown:(NSEvent*)theEvent \
	{ \
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance; \
		if (instance.isNotNull()) { \
			sl_bool flagNoDefault = instance->onEventKey(sl_true, theEvent) & slib::UIEventFlags::PreventDefault; \
			if (flagNoDefault) { \
				return; \
			} \
		} \
		[super keyDown:theEvent]; \
	} \
	- (void)keyUp:(NSEvent*)theEvent \
	{ \
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance; \
		if (instance.isNotNull()) { \
			sl_bool flagNoDefault = instance->onEventKey(sl_false, theEvent) & slib::UIEventFlags::PreventDefault; \
			if (flagNoDefault) { \
				return; \
			} \
		} \
		[super keyUp:theEvent]; \
	}

#endif

#endif
