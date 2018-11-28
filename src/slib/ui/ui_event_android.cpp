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

#if defined(SLIB_UI_IS_ANDROID)

#include "slib/ui/event.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/core/hash_table.h"
#include "slib/core/safe_static.h"
#include "slib/core/platform_android.h"

namespace slib
{

	class _priv_UI_Android_KeyMapper
	{
	private:
		HashTable<sl_uint32, sl_uint32> mapKeyToVK;
		HashTable<sl_uint32, sl_uint32> mapVKToKey;
		
	public:
		_priv_UI_Android_KeyMapper()
		{
			map(Keycode::Tab, 61);
			map(Keycode::Enter, 66);
			map(Keycode::Escape, 111);
			
			map(Keycode::Space, 62);
			map(Keycode::Grave, 68);
			map(Keycode::Equal, 70);
			map(Keycode::Semicolon, 74);
			map(Keycode::Backslash, 73);
			map(Keycode::LeftBaracket, 71);
			map(Keycode::RightBaracket, 72);
			map(Keycode::Quote, 75);
			map(Keycode::Comma, 55);
			map(Keycode::Minus, 69);
			map(Keycode::Period, 56);
			map(Keycode::Divide, 76);
			
			map(Keycode::Num0, 7);
			map(Keycode::Num1, 8);
			map(Keycode::Num2, 9);
			map(Keycode::Num3, 10);
			map(Keycode::Num4, 11);
			map(Keycode::Num5, 12);
			map(Keycode::Num6, 13);
			map(Keycode::Num7, 14);
			map(Keycode::Num8, 15);
			map(Keycode::Num9, 16);
			
			map(Keycode::A, 29);
			map(Keycode::B, 30);
			map(Keycode::C, 31);
			map(Keycode::D, 32);
			map(Keycode::E, 33);
			map(Keycode::F, 34);
			map(Keycode::G, 35);
			map(Keycode::H, 36);
			map(Keycode::I, 37);
			map(Keycode::J, 38);
			map(Keycode::K, 39);
			map(Keycode::L, 40);
			map(Keycode::M, 41);
			map(Keycode::N, 42);
			map(Keycode::O, 43);
			map(Keycode::P, 44);
			map(Keycode::Q, 45);
			map(Keycode::R, 46);
			map(Keycode::S, 47);
			map(Keycode::T, 48);
			map(Keycode::U, 49);
			map(Keycode::V, 50);
			map(Keycode::W, 51);
			map(Keycode::X, 52);
			map(Keycode::Y, 53);
			map(Keycode::Z, 54);
			
			map(Keycode::Numpad0, 144);
			map(Keycode::Numpad1, 145);
			map(Keycode::Numpad2, 146);
			map(Keycode::Numpad3, 147);
			map(Keycode::Numpad4, 148);
			map(Keycode::Numpad5, 149);
			map(Keycode::Numpad6, 150);
			map(Keycode::Numpad7, 151);
			map(Keycode::Numpad8, 152);
			map(Keycode::Numpad9, 153);
			
			map(Keycode::NumpadDivide, 154);
			map(Keycode::NumpadMultiply, 155);
			map(Keycode::NumpadMinus, 156);
			map(Keycode::NumpadPlus, 157);
			map(Keycode::NumpadEnter, 160);
			map(Keycode::NumpadDecimal, 158);
			
			map(Keycode::F1, 131);
			map(Keycode::F2, 132);
			map(Keycode::F3, 133);
			map(Keycode::F4, 134);
			map(Keycode::F5, 135);
			map(Keycode::F6, 136);
			map(Keycode::F7, 137);
			map(Keycode::F8, 138);
			map(Keycode::F9, 139);
			map(Keycode::F10, 140);
			map(Keycode::F11, 141);
			map(Keycode::F12, 142);
			
			map(Keycode::Backspace, 67);
			map(Keycode::PageUp, 92);
			map(Keycode::PageDown, 93);
			map(Keycode::Home, 122);
			map(Keycode::End, 123);
			map(Keycode::Left, 21);
			map(Keycode::Up, 19);
			map(Keycode::Right, 22);
			map(Keycode::Down, 20);
			map(Keycode::PrintScreen, 120);
			map(Keycode::Insert, 124);
			map(Keycode::Delete, 112);
			map(Keycode::Sleep, -1);
			map(Keycode::Pause, 121);
			
			map(Keycode::GoHome, -1);
			map(Keycode::GoMenu, 82);
			map(Keycode::GoBack, 4);
			map(Keycode::Camera, 27);
			map(Keycode::VolumeMute, 164);
			map(Keycode::VolumeDown, 25);
			map(Keycode::VolumeUp, 24);
			map(Keycode::MediaPrev, 88);
			map(Keycode::MediaNext, 87);
			map(Keycode::MediaPause, 85);
			map(Keycode::MediaStop, 86);
			map(Keycode::PhoneStar, 17);
			map(Keycode::PhonePound, 18);

			map(Keycode::LeftShift, 59);
			map(Keycode::RightShift, 60);
			map(Keycode::LeftControl, 113);
			map(Keycode::RightControl, 114);
			map(Keycode::LeftAlt, 57);
			map(Keycode::RightAlt, 58);
			map(Keycode::LeftWin, 117);
			map(Keycode::RightWin, 118);
			map(Keycode::CapsLock, 115);
			map(Keycode::ScrollLock, 116);
			map(Keycode::NumLock, 143);
			map(Keycode::ContextMenu, -1);
		}
		
	public:
		void map(Keycode key, sl_uint32 vk)
		{
			if (vk == -1) {
				return;
			}
			mapKeyToVK.put((int)key, vk);
			mapVKToKey.put(vk, (int)key);
		}
		
		Keycode vkToKey(sl_uint32 vk)
		{
			sl_uint32 ret;
			if (mapVKToKey.get(vk, &ret)) {
				return (Keycode)ret;
			} else {
				return Keycode::Unknown;
			}
		}
		
		sl_uint32 keyToVk(Keycode code)
		{
			sl_uint32 ret;
			if (mapKeyToVK.get((int)code, &ret)) {
				return ret;
			} else {
				return -1;
			}
		}
		
	};

	SLIB_SAFE_STATIC_GETTER(_priv_UI_Android_KeyMapper, _priv_UI_Android_getKeyMapper)

	sl_uint32 UIEvent::getSystemKeycode(Keycode key)
	{
		_priv_UI_Android_KeyMapper* mapper = _priv_UI_Android_getKeyMapper();
		if (mapper) {
			return mapper->keyToVk(key);
		}
		return 0;
	}

	Keycode UIEvent::getKeycodeFromSystemKeycode(sl_uint32 vkey)
	{
		_priv_UI_Android_KeyMapper* mapper = _priv_UI_Android_getKeyMapper();
		if (mapper) {
			return mapper->vkToKey(vkey);
		}
		return Keycode::Unknown;
	}

}

#endif
