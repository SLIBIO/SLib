/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_GTK)

#include "slib/ui/event.h"

#include "slib/ui/core.h"
#include "slib/ui/platform.h"
#include "slib/core/hash_table.h"
#include "slib/core/safe_static.h"

#include "gdk/gdkkeysyms.h"

namespace slib
{

	class _priv_UI_GTK_KeyMapper
	{
	private:
		HashTable<Keycode, sl_uint32> mapKeyToVK;
		HashTable<sl_uint32, Keycode> mapVKToKey;
		
	public:
		_priv_UI_GTK_KeyMapper()
		{
			map(Keycode::Tab, GDK_KEY_Tab);
			mapVKToKey.put(GDK_KEY_ISO_Left_Tab, Keycode::Tab);
			map(Keycode::Enter, GDK_KEY_Return);
			map(Keycode::Escape, GDK_KEY_Escape);
			
			map(Keycode::Space, GDK_KEY_space);
			map(Keycode::Grave, GDK_KEY_grave);
			mapVKToKey.put(GDK_KEY_asciitilde, Keycode::Grave);
			map(Keycode::Equal, GDK_KEY_equal);
			mapVKToKey.put(GDK_KEY_plus, Keycode::Equal);
			map(Keycode::Semicolon, GDK_KEY_semicolon);
			mapVKToKey.put(GDK_KEY_colon, Keycode::Semicolon);
			map(Keycode::Backslash, GDK_KEY_backslash);
			mapVKToKey.put(GDK_KEY_bar, Keycode::Backslash);
			map(Keycode::LeftBaracket, GDK_KEY_bracketleft);
			mapVKToKey.put(GDK_KEY_braceleft, Keycode::LeftBaracket);
			map(Keycode::RightBaracket, GDK_KEY_bracketright);
			mapVKToKey.put(GDK_KEY_braceright, Keycode::RightBaracket);
			map(Keycode::Quote, GDK_KEY_apostrophe);
			mapVKToKey.put(GDK_KEY_quotedbl, Keycode::Quote);
			map(Keycode::Comma, GDK_KEY_comma);
			mapVKToKey.put(GDK_KEY_less, Keycode::Comma);
			map(Keycode::Minus, GDK_KEY_minus);
			mapVKToKey.put(GDK_KEY_underscore, Keycode::Minus);
			map(Keycode::Period, GDK_KEY_period);
			mapVKToKey.put(GDK_KEY_greater, Keycode::Period);
			map(Keycode::Divide, GDK_KEY_slash);
			mapVKToKey.put(GDK_KEY_question, Keycode::Divide);

			map(Keycode::Num0, GDK_KEY_0);
			mapVKToKey.put(GDK_KEY_parenright, Keycode::Num0);
			map(Keycode::Num1, GDK_KEY_1);
			mapVKToKey.put(GDK_KEY_exclam, Keycode::Num1);
			map(Keycode::Num2, GDK_KEY_2);
			mapVKToKey.put(GDK_KEY_at, Keycode::Num2);
			map(Keycode::Num3, GDK_KEY_3);
			mapVKToKey.put(GDK_KEY_numbersign, Keycode::Num3);
			map(Keycode::Num4, GDK_KEY_4);
			mapVKToKey.put(GDK_KEY_dollar, Keycode::Num4);
			map(Keycode::Num5, GDK_KEY_5);
			mapVKToKey.put(GDK_KEY_percent, Keycode::Num5);
			map(Keycode::Num6, GDK_KEY_6);
			mapVKToKey.put(GDK_KEY_asciicircum, Keycode::Num6);
			map(Keycode::Num7, GDK_KEY_7);
			mapVKToKey.put(GDK_KEY_ampersand, Keycode::Num7);
			map(Keycode::Num8, GDK_KEY_8);
			mapVKToKey.put(GDK_KEY_asterisk, Keycode::Num8);
			map(Keycode::Num9, GDK_KEY_9);
			mapVKToKey.put(GDK_KEY_parenleft, Keycode::Num9);

			for (int vk = 0; vk <= (GDK_KEY_z - GDK_KEY_a); vk++) {
				map((Keycode)((int)(Keycode::A) + vk), GDK_KEY_a + vk);
				mapVKToKey.put(GDK_KEY_A + vk, (Keycode)((int)(Keycode::A) + vk));
			}
			
			map(Keycode::Numpad0, GDK_KEY_KP_0);
			mapVKToKey.put(GDK_KEY_KP_Insert, Keycode::Numpad0);
			map(Keycode::Numpad1, GDK_KEY_KP_1);
			mapVKToKey.put(GDK_KEY_KP_End, Keycode::Numpad1);
			map(Keycode::Numpad2, GDK_KEY_KP_2);
			mapVKToKey.put(GDK_KEY_KP_Down, Keycode::Numpad2);
			map(Keycode::Numpad3, GDK_KEY_KP_3);
			mapVKToKey.put(GDK_KEY_KP_Page_Down, Keycode::Numpad3);
			map(Keycode::Numpad4, GDK_KEY_KP_4);
			mapVKToKey.put(GDK_KEY_KP_Left, Keycode::Numpad4);
			map(Keycode::Numpad5, GDK_KEY_KP_5);
			mapVKToKey.put(GDK_KEY_KP_Begin, Keycode::Numpad5);
			map(Keycode::Numpad6, GDK_KEY_KP_6);
			mapVKToKey.put(GDK_KEY_KP_Right, Keycode::Numpad6);
			map(Keycode::Numpad7, GDK_KEY_KP_7);
			mapVKToKey.put(GDK_KEY_KP_Home, Keycode::Numpad7);
			map(Keycode::Numpad8, GDK_KEY_KP_8);
			mapVKToKey.put(GDK_KEY_KP_Up, Keycode::Numpad8);
			map(Keycode::Numpad9, GDK_KEY_KP_9);
			mapVKToKey.put(GDK_KEY_KP_Page_Up, Keycode::Numpad9);

			map(Keycode::NumpadDivide, GDK_KEY_KP_Divide);
			map(Keycode::NumpadMultiply, GDK_KEY_KP_Multiply);
			map(Keycode::NumpadMinus, GDK_KEY_KP_Subtract);
			map(Keycode::NumpadPlus, GDK_KEY_KP_Add);
			map(Keycode::NumpadEnter, GDK_KEY_KP_Enter);
			map(Keycode::NumpadDecimal, GDK_KEY_KP_Decimal);
			mapVKToKey.put(GDK_KEY_KP_Delete, Keycode::NumpadDecimal);
			
			map(Keycode::F1, GDK_KEY_F1);
			map(Keycode::F2, GDK_KEY_F2);
			map(Keycode::F3, GDK_KEY_F3);
			map(Keycode::F4, GDK_KEY_F4);
			map(Keycode::F5, GDK_KEY_F5);
			map(Keycode::F6, GDK_KEY_F6);
			map(Keycode::F7, GDK_KEY_F7);
			map(Keycode::F8, GDK_KEY_F8);
			map(Keycode::F9, GDK_KEY_F9);
			map(Keycode::F10, GDK_KEY_F10);
			map(Keycode::F11, GDK_KEY_F11);
			map(Keycode::F12, GDK_KEY_F12);
			
			map(Keycode::Backspace, GDK_KEY_BackSpace);
			map(Keycode::PageUp, GDK_KEY_Page_Up);
			map(Keycode::PageDown, GDK_KEY_Page_Down);
			map(Keycode::Home, GDK_KEY_Home);
			map(Keycode::End, GDK_KEY_End);
			map(Keycode::Left, GDK_KEY_Left);
			map(Keycode::Up, GDK_KEY_Up);
			map(Keycode::Right, GDK_KEY_Right);
			map(Keycode::Down, GDK_KEY_Down);
			map(Keycode::PrintScreen, GDK_KEY_Print);
			map(Keycode::Insert, GDK_KEY_Insert);
			map(Keycode::Delete, GDK_KEY_Delete);
			map(Keycode::Sleep, GDK_KEY_Sleep);
			map(Keycode::Pause, GDK_KEY_Pause);
			
			map(Keycode::GoHome, -1);
			map(Keycode::GoMenu, -1);
			map(Keycode::GoBack, -1);
			map(Keycode::Camera, -1);
			map(Keycode::VolumeMute, GDK_KEY_AudioMute);
			map(Keycode::VolumeDown, GDK_KEY_AudioLowerVolume);
			map(Keycode::VolumeUp, GDK_KEY_AudioRaiseVolume);
			map(Keycode::MediaPrev, GDK_KEY_AudioPrev);
			map(Keycode::MediaNext, GDK_KEY_AudioNext);
			map(Keycode::MediaPause, GDK_KEY_AudioPlay);
			map(Keycode::MediaStop, GDK_KEY_AudioStop);
			map(Keycode::PhoneStar, -1);
			map(Keycode::PhonePound, -1);
			
			map(Keycode::LeftShift, GDK_KEY_Shift_L);
			map(Keycode::RightShift, GDK_KEY_Shift_R);
			map(Keycode::LeftControl, GDK_KEY_Control_L);
			map(Keycode::RightControl, GDK_KEY_Control_R);
			map(Keycode::LeftAlt, GDK_KEY_Alt_L);
			map(Keycode::RightAlt, GDK_KEY_Alt_R);
			map(Keycode::LeftWin, GDK_KEY_Super_L);
			map(Keycode::RightWin, GDK_KEY_Super_R);
			map(Keycode::CapsLock, GDK_KEY_Caps_Lock);
			map(Keycode::ScrollLock, GDK_KEY_Scroll_Lock);
			map(Keycode::NumLock, GDK_KEY_Num_Lock);
			map(Keycode::ContextMenu, GDK_KEY_Menu);
		}
		
	public:
		void map(Keycode key, sl_uint32 vk)
		{
			if (vk == -1) {
				return;
			}
			mapKeyToVK.put(key, vk);
			mapVKToKey.put(vk, key);
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
		
	};

	SLIB_SAFE_STATIC_GETTER(_priv_UI_GTK_KeyMapper, _priv_UI_GTK_getKeyMapper)

	sl_uint32 UIEvent::getSystemKeycode(Keycode key)
	{
		_priv_UI_GTK_KeyMapper* mapper = _priv_UI_GTK_getKeyMapper();
		if (mapper) {
			return mapper->keyToVk(key);
		}
		return 0;
	}

	Keycode UIEvent::getKeycodeFromSystemKeycode(sl_uint32 vkey)
	{
		_priv_UI_GTK_KeyMapper* mapper = _priv_UI_GTK_getKeyMapper();
		if (mapper) {
			return mapper->vkToKey(vkey);
		}
		return Keycode::Unknown;
	}

	sl_bool UI::checkCapsLockOn()
	{
		GdkKeymap* keymap = gdk_keymap_get_default();
		if (keymap) {
			return gdk_keymap_get_caps_lock_state(keymap);
		}
		return sl_false;
	}

	UIPoint UI::getCursorPos()
	{
		GdkDisplay* display = gdk_display_get_default();
		if (display) {
			gint x = 0;
			gint y = 0;
			gdk_display_get_pointer(display, sl_null, &x, &y, sl_null);
			return UIPoint(x, y);
		}
		return UIPoint::zero();
	}
	
}

#endif
