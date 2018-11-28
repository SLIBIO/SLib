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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/event.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/core/hash_table.h"
#include "slib/core/safe_static.h"

#import <Carbon/Carbon.h>

namespace slib
{

	class _priv_UI_macOS_KeyMapper
	{
	private:
		HashTable<Keycode, sl_uint32> mapKeyToVK;
		HashTable<sl_uint32, Keycode> mapVKToKey;
		HashTable<Keycode, NSString*> mapKeyToEquivalent;
		
	public:
		_priv_UI_macOS_KeyMapper()
		{
			map(Keycode::Tab, 0x30, '\t');
			map(Keycode::Enter, 0x24, '\r');
			map(Keycode::Escape, 0x35, '\e');
			
			map(Keycode::Space, 0x31, ' ');
			map(Keycode::Grave, 0x32, '`');
			map(Keycode::Equal, 0x18, '=');
			map(Keycode::Semicolon, 0x29, ';');
			map(Keycode::Backslash, 0x2A, '\\');
			map(Keycode::LeftBaracket, 0x21, '[');
			map(Keycode::RightBaracket, 0x1E, ']');
			map(Keycode::Quote, 0x27, '\'');
			map(Keycode::Comma, 0x2B, ',');
			map(Keycode::Minus, 0x1B, '-');
			map(Keycode::Period, 0x2F, '.');
			map(Keycode::Divide, 0x2C, '/');
			
			map(Keycode::Num0, 0x1D, '0');
			map(Keycode::Num1, 0x12, '1');
			map(Keycode::Num2, 0x13, '2');
			map(Keycode::Num3, 0x14, '3');
			map(Keycode::Num4, 0x15, '4');
			map(Keycode::Num5, 0x17, '5');
			map(Keycode::Num6, 0x16, '6');
			map(Keycode::Num7, 0x1A, '7');
			map(Keycode::Num8, 0x1C, '8');
			map(Keycode::Num9, 0x19, '9');
			
			map(Keycode::A, 0x00, 'a');
			map(Keycode::B, 0x0B, 'b');
			map(Keycode::C, 0x08, 'c');
			map(Keycode::D, 0x02, 'd');
			map(Keycode::E, 0x0E, 'e');
			map(Keycode::F, 0x03, 'f');
			map(Keycode::G, 0x05, 'g');
			map(Keycode::H, 0x04, 'h');
			map(Keycode::I, 0x22, 'i');
			map(Keycode::J, 0x26, 'j');
			map(Keycode::K, 0x28, 'k');
			map(Keycode::L, 0x25, 'l');
			map(Keycode::M, 0x2E, 'm');
			map(Keycode::N, 0x2D, 'n');
			map(Keycode::O, 0x1F, 'o');
			map(Keycode::P, 0x23, 'p');
			map(Keycode::Q, 0x0C, 'q');
			map(Keycode::R, 0x0F, 'r');
			map(Keycode::S, 0x01, 's');
			map(Keycode::T, 0x11, 't');
			map(Keycode::U, 0x20, 'u');
			map(Keycode::V, 0x09, 'v');
			map(Keycode::W, 0x0D, 'w');
			map(Keycode::X, 0x07, 'x');
			map(Keycode::Y, 0x10, 'y');
			map(Keycode::Z, 0x06, 'z');
			
			map(Keycode::Numpad0, 0x52);
			map(Keycode::Numpad1, 0x53);
			map(Keycode::Numpad2, 0x54);
			map(Keycode::Numpad3, 0x55);
			map(Keycode::Numpad4, 0x56);
			map(Keycode::Numpad5, 0x57);
			map(Keycode::Numpad6, 0x58);
			map(Keycode::Numpad7, 0x59);
			map(Keycode::Numpad8, 0x5B);
			map(Keycode::Numpad9, 0x5C);
			
			map(Keycode::NumpadDivide, 0x4B);
			map(Keycode::NumpadMultiply, 0x43);
			map(Keycode::NumpadMinus, 0x4E);
			map(Keycode::NumpadPlus, 0x45);
			map(Keycode::NumpadEnter, 0x4C);
			map(Keycode::NumpadDecimal, 0x41);
			
			map(Keycode::F1, 0x7A, NSF1FunctionKey);
			map(Keycode::F2, 0x78, NSF2FunctionKey);
			map(Keycode::F3, 0x63, NSF3FunctionKey);
			map(Keycode::F4, 0x76, NSF4FunctionKey);
			map(Keycode::F5, 0x60, NSF5FunctionKey);
			map(Keycode::F6, 0x61, NSF6FunctionKey);
			map(Keycode::F7, 0x62, NSF7FunctionKey);
			map(Keycode::F8, 0x64, NSF8FunctionKey);
			map(Keycode::F9, 0x65, NSF9FunctionKey);
			map(Keycode::F10, 0x6D, NSF10FunctionKey);
			map(Keycode::F11, 0x67, NSF11FunctionKey);
			map(Keycode::F12, 0x6F, NSF12FunctionKey);
			
			map(Keycode::Backspace, 0x33, '\b');
			map(Keycode::PageUp, 0x74, NSPageUpFunctionKey);
			map(Keycode::PageDown, 0x79, NSPageDownFunctionKey);
			map(Keycode::Home, 0x73, NSHomeFunctionKey);
			map(Keycode::End, 0x77, NSEndFunctionKey);
			map(Keycode::Left, 0x7B, NSLeftArrowFunctionKey);
			map(Keycode::Up, 0x7E, NSUpArrowFunctionKey);
			map(Keycode::Right, 0x7C, NSRightArrowFunctionKey);
			map(Keycode::Down, 0x7D, NSDownArrowFunctionKey);
			map(Keycode::PrintScreen, 0x69, NSPrintScreenFunctionKey);
			map(Keycode::Insert, 0x72, NSInsertFunctionKey);
			map(Keycode::Delete, 0x75, NSDeleteFunctionKey);
			map(Keycode::Sleep, -1);
			map(Keycode::Pause, 0x71, NSPauseFunctionKey);
			
			map(Keycode::GoHome, -1);
			map(Keycode::GoMenu, -1);
			map(Keycode::GoBack, -1);
			map(Keycode::Camera, -1);
			map(Keycode::VolumeMute, 0x4A);
			map(Keycode::VolumeDown, 0x49);
			map(Keycode::VolumeUp, 0x48);
			map(Keycode::MediaPrev, -1);
			map(Keycode::MediaNext, -1);
			map(Keycode::MediaPause, -1);
			map(Keycode::MediaStop, -1);
			map(Keycode::PhoneStar, -1);
			map(Keycode::PhonePound, -1);
			
			map(Keycode::LeftShift, 0x38);
			map(Keycode::RightShift, 0x3C);
			map(Keycode::LeftControl, 0x3B);
			map(Keycode::RightControl, 0x3E);
			map(Keycode::LeftOption, 0x3A);
			map(Keycode::RightOption, 0x3D);
			map(Keycode::RightCommand, 0x37);
			map(Keycode::LeftCommand, 0x37);
			map(Keycode::CapsLock, 0x39);
			map(Keycode::ScrollLock, 0x6B, NSScrollLockFunctionKey);
			map(Keycode::NumLock, 0x47);
			map(Keycode::ContextMenu, 0x6E);
		}
		
	public:
		void map(Keycode key, sl_uint32 vk, unichar code = 0)
		{
			if (vk == -1) {
				return;
			}
			mapKeyToVK.put(key, vk);
			mapVKToKey.put(vk, key);
			if (code) {
				mapKeyToEquivalent.put(key, [NSString stringWithCharacters:&code length:1]);
			}
		}
		
		Keycode vkToKey(sl_uint32 vk)
		{
			Keycode ret;
			if (mapVKToKey.get(vk, &ret)) {
				return ret;
			}
			return Keycode::Unknown;
		}
		
		sl_uint32 keyToVk(Keycode code)
		{
			sl_uint32 ret;
			if (mapKeyToVK.get(code, &ret)) {
				return ret;
			}
			return -1;
		}
		
		NSString* keyToEquivalent(Keycode code)
		{
			NSString* ret;
			if (mapKeyToEquivalent.get(code, &ret)) {
				return ret;
			}
			return @"";
		}
		
	};

	SLIB_SAFE_STATIC_GETTER(_priv_UI_macOS_KeyMapper, _priv_UI_macOS_getKeyMapper)

	sl_uint32 UIEvent::getSystemKeycode(Keycode key)
	{
		_priv_UI_macOS_KeyMapper* mapper = _priv_UI_macOS_getKeyMapper();
		if (mapper) {
			return mapper->keyToVk(key);
		}
		return 0;
	}

	Keycode UIEvent::getKeycodeFromSystemKeycode(sl_uint32 vkey)
	{
		_priv_UI_macOS_KeyMapper* mapper = _priv_UI_macOS_getKeyMapper();
		if (mapper) {
			return mapper->vkToKey(vkey);
		}
		return Keycode::Unknown;
	}

	sl_bool UI::checkKeyPressed(Keycode key)
	{
		sl_uint32 vk = UIEvent::getSystemKeycode(key);
		if (vk != -1) {
			return CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, (CGKeyCode)vk) != FALSE;
		} else {
			return sl_false;
		}
	}

	sl_bool UI::checkCapsLockOn()
	{
		NSUInteger flags = [NSEvent modifierFlags];
		return (flags & NSAlphaShiftKeyMask) != 0;
	}

	sl_bool UI::checkNumLockOn()
	{
		NSUInteger flags = [NSEvent modifierFlags];
		return (flags & NSNumericPadKeyMask) != 0;
	}

	sl_bool UI::checkScrollLockOn()
	{
		return sl_false;
	}


	UIPoint UI::getCursorPos()
	{
		NSScreen* screen = [NSScreen mainScreen];
		NSRect rect = [screen frame];
		NSPoint pt = [NSEvent mouseLocation];
		return UIPoint((sl_ui_pos)(pt.x), (sl_ui_pos)(rect.size.height - pt.y));
	}

	sl_bool UI::checkLeftButtonPressed()
	{
		NSUInteger flags = [NSEvent pressedMouseButtons];
		return (flags & 1) != 0;
	}

	sl_bool UI::checkRightButtonPressed()
	{
		NSUInteger flags = [NSEvent pressedMouseButtons];
		return (flags & 2) != 0;
	}

	sl_bool UI::checkMiddleButtonPressed()
	{
		NSUInteger flags = [NSEvent pressedMouseButtons];
		return (flags & 4) != 0;
	}

	NSString* UIPlatform::getKeyEquivalent(const KeycodeAndModifiers& km, NSUInteger& mask)
	{
		_priv_UI_macOS_KeyMapper* mapper = _priv_UI_macOS_getKeyMapper();
		if (!mapper) {
			return @"";
		}
		mask = 0;
		NSString* ret = mapper->keyToEquivalent(km.getKeycode());
		if (ret == nil) {
			return @"";
		}
		if (ret.length > 0) {
			if (km.isCommandKey()) {
				mask |= NSCommandKeyMask;
			}
			if (km.isControlKey()) {
				mask |= NSControlKeyMask;
			}
			if (km.isShiftKey()) {
				mask |= NSShiftKeyMask;
			}
			if (km.isAltKey()) {
				mask |= NSAlternateKeyMask;
			}
		}
		return ret;
	}

}

#endif
