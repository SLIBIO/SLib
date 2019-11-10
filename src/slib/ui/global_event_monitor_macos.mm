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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/global_event_monitor.h"

#include "slib/ui/platform.h"

namespace slib
{
	
	namespace priv
	{
		namespace global_event_monitor
		{
			
			id g_monitorGlobal = nil;
			id g_monitorLocal = nil;
			
			class GlobalEventMonitorHelper : public GlobalEventMonitor
			{
			public:
				using GlobalEventMonitor::_onEvent;
			};
			
			static void ProcessMouseEvent(UIAction action, NSEvent* event)
			{
				NSPoint pt = event.locationInWindow;
				NSWindow* window = event.window;
				if (window != nil) {
					pt = [window convertPointToScreen:pt];
				}
				sl_ui_posf x = (sl_ui_posf)(pt.x);
				sl_ui_posf y = (sl_ui_posf)(pt.y);
				Time t;
				t.setSecondsCountf(event.timestamp);
				Ref<UIEvent> ev = UIEvent::createMouseEvent(action, x, y, t);
				if (ev.isNotNull()) {
					UIPlatform::applyEventModifiers(ev.get(), event);
					GlobalEventMonitorHelper::_onEvent(ev.get());
				}
			}
			
			static void ProcessMouseWheelEvent(NSEvent* event)
			{
				sl_real deltaX = (sl_real)([event deltaX]);
				sl_real deltaY = (sl_real)([event deltaY]);
				if (Math::isAlmostZero(deltaX) && Math::isAlmostZero(deltaY)) {
					return;
				}
				NSPoint pt = event.locationInWindow;
				NSWindow* window = event.window;
				if (window != nil) {
					pt = [window convertPointToScreen:pt];
				}
				sl_ui_posf x = (sl_ui_posf)(pt.x);
				sl_ui_posf y = (sl_ui_posf)(pt.y);
				Time t;
				t.setSecondsCountf([event timestamp]);
				Ref<UIEvent> ev = UIEvent::createMouseWheelEvent(x, y, deltaX, deltaY, t);
				if (ev.isNotNull()) {
					UIPlatform::applyEventModifiers(ev.get(), event);
					GlobalEventMonitorHelper::_onEvent(ev.get());
				}
			}
			
			static void ProcessKeyEvent(UIAction action, NSEvent* event)
			{
				sl_uint32 vkey = [event keyCode];
				Keycode key = UIEvent::getKeycodeFromSystemKeycode(vkey);
				Time t;
				t.setSecondsCountf([event timestamp]);
				Ref<UIEvent> ev = UIEvent::createKeyEvent(action, key, vkey, t);
				if (ev.isNotNull()) {
					UIPlatform::applyEventModifiers(ev.get(), event);
					GlobalEventMonitorHelper::_onEvent(ev.get());
				}
			}
			
			static void ProcessEvent(NSEvent* ev)
			{
				NSEventType type = ev.type;
				switch (type) {
					case NSLeftMouseDown:
						ProcessMouseEvent(UIAction::LeftButtonDown, ev);
						break;
					case NSLeftMouseUp:
						ProcessMouseEvent(UIAction::LeftButtonUp, ev);
						break;
					case NSLeftMouseDragged:
						ProcessMouseEvent(UIAction::LeftButtonDrag, ev);
						break;
					case NSRightMouseDown:
						ProcessMouseEvent(UIAction::RightButtonDown, ev);
						break;
					case NSRightMouseUp:
						ProcessMouseEvent(UIAction::RightButtonUp, ev);
						break;
					case NSRightMouseDragged:
						ProcessMouseEvent(UIAction::RightButtonDrag, ev);
						break;
					case NSOtherMouseDown:
						ProcessMouseEvent(UIAction::MiddleButtonDown, ev);
						break;
					case NSOtherMouseUp:
						ProcessMouseEvent(UIAction::MiddleButtonUp, ev);
						break;
					case NSOtherMouseDragged:
						ProcessMouseEvent(UIAction::MiddleButtonDrag, ev);
						break;
					case NSMouseMoved:
						ProcessMouseEvent(UIAction::MouseMove, ev);
						break;
					case NSScrollWheel:
						ProcessMouseWheelEvent(ev);
						break;
					case NSKeyDown:
						ProcessKeyEvent(UIAction::KeyDown, ev);
						break;
					case NSKeyUp:
						ProcessKeyEvent(UIAction::KeyUp, ev);
						break;
					default:
						break;
				}
			}
			
		}
	}
	
	using namespace priv::global_event_monitor;
	
	sl_bool GlobalEventMonitor::_updateMonitor(sl_uint32 _mask)
	{
		if (g_monitorGlobal != nil) {
			[NSEvent removeMonitor:g_monitorGlobal];
			g_monitorGlobal = nil;
		}
		if (g_monitorLocal != nil) {
			[NSEvent removeMonitor:g_monitorLocal];
			g_monitorLocal = nil;
		}
		if (_mask) {
			NSEventMask mask = 0;
			if (_mask & MASK_MOUSE) {
				mask |= NSLeftMouseDownMask;
				mask |= NSLeftMouseUpMask;
				mask |= NSLeftMouseDraggedMask;
				mask |= NSRightMouseDownMask;
				mask |= NSRightMouseUpMask;
				mask |= NSRightMouseDraggedMask;
				mask |= NSOtherMouseDownMask;
				mask |= NSOtherMouseUpMask;
				mask |= NSOtherMouseDraggedMask;
				mask |= NSMouseMovedMask;
				mask |= NSScrollWheelMask;
			}
			if (_mask & MASK_KEYBOARD) {
				mask |= NSKeyDownMask;
				mask |= NSKeyUpMask;
			}
			id monitorGlobal = [NSEvent addGlobalMonitorForEventsMatchingMask:mask handler:^(NSEvent* ev) {
				ProcessEvent(ev);
			}];
			if (monitorGlobal != nil) {
				id monitorLocal = [NSEvent addLocalMonitorForEventsMatchingMask:mask handler:^(NSEvent* ev) {
					ProcessEvent(ev);
					return ev;
				}];
				if (monitorLocal != nil) {
					g_monitorGlobal = monitorGlobal;
					g_monitorLocal = monitorLocal;
					return sl_true;
				}
				[NSEvent removeMonitor:monitorGlobal];
			}
		} else {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool GlobalEventMonitor::isKeyboardAccessEnabled()
	{
		return AXIsProcessTrustedWithOptions(NULL) != FALSE;
	}
	
	void GlobalEventMonitor::authenticateKeyboardAccess()
	{
		NSDictionary *options = @{(__bridge NSString*)kAXTrustedCheckOptionPrompt: @YES};
		AXIsProcessTrustedWithOptions((CFDictionaryRef)options);
	}
	
	void GlobalEventMonitor::openSystemPreferencesForKeyboardAccess()
	{
		[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"x-apple.systempreferences:com.apple.preference.security?Privacy_Accessibility"]];
	}

}

#endif
