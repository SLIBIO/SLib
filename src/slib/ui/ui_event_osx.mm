#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/event.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/core/hashtable.h"

#import <Carbon/Carbon.h>

SLIB_UI_NAMESPACE_BEGIN

class _UI_OSX_KeyMapper
{
private:
	HashTable<sl_uint32, sl_uint32> mapKeyToVK;
	HashTable<sl_uint32, sl_uint32> mapVKToKey;
	
public:
	_UI_OSX_KeyMapper()
	{
		map(Keycode::Tab, 0x30);
		map(Keycode::Enter, 0x24);
		map(Keycode::Escape, 0x35);
		
		map(Keycode::Space, 0x31);
		map(Keycode::Grave, 0x32);
		map(Keycode::Equal, 0x18);
		map(Keycode::Semicolon, 0x29);
		map(Keycode::Backslash, 0x2A);
		map(Keycode::LeftBaracket, 0x21);
		map(Keycode::RightBaracket, 0x1E);
		map(Keycode::Quote, 0x27);
		map(Keycode::Comma, 0x2B);
		map(Keycode::Minus, 0x1B);
		map(Keycode::Period, 0x2F);
		map(Keycode::Divide, 0x2C);
		
		map(Keycode::Num0, 0x1D);
		map(Keycode::Num1, 0x12);
		map(Keycode::Num2, 0x13);
		map(Keycode::Num3, 0x14);
		map(Keycode::Num4, 0x15);
		map(Keycode::Num5, 0x17);
		map(Keycode::Num6, 0x16);
		map(Keycode::Num7, 0x1A);
		map(Keycode::Num8, 0x1C);
		map(Keycode::Num9, 0x19);
		
		map(Keycode::A, 0x00);
		map(Keycode::B, 0x0B);
		map(Keycode::C, 0x08);
		map(Keycode::D, 0x02);
		map(Keycode::E, 0x0E);
		map(Keycode::F, 0x03);
		map(Keycode::G, 0x05);
		map(Keycode::H, 0x04);
		map(Keycode::I, 0x22);
		map(Keycode::J, 0x26);
		map(Keycode::K, 0x28);
		map(Keycode::L, 0x25);
		map(Keycode::M, 0x2E);
		map(Keycode::N, 0x2D);
		map(Keycode::O, 0x1F);
		map(Keycode::P, 0x23);
		map(Keycode::Q, 0x0C);
		map(Keycode::R, 0x0F);
		map(Keycode::S, 0x01);
		map(Keycode::T, 0x11);
		map(Keycode::U, 0x20);
		map(Keycode::V, 0x09);
		map(Keycode::W, 0x0D);
		map(Keycode::X, 0x07);
		map(Keycode::Y, 0x10);
		map(Keycode::Z, 0x06);
		
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
		
		map(Keycode::F1, 0x7A);
		map(Keycode::F2, 0x78);
		map(Keycode::F3, 0x63);
		map(Keycode::F4, 0x76);
		map(Keycode::F5, 0x60);
		map(Keycode::F6, 0x61);
		map(Keycode::F7, 0x62);
		map(Keycode::F8, 0x64);
		map(Keycode::F9, 0x65);
		map(Keycode::F10, 0x6D);
		map(Keycode::F11, 0x67);
		map(Keycode::F12, 0x6F);
		
		map(Keycode::Backspace, 0x33);
		map(Keycode::PageUp, 0x74);
		map(Keycode::PageDown, 0x79);
		map(Keycode::Home, 0x73);
		map(Keycode::End, 0x77);
		map(Keycode::Left, 0x7B);
		map(Keycode::Up, 0x7E);
		map(Keycode::Right, 0x7C);
		map(Keycode::Down, 0x7D);
		map(Keycode::PrintScreen, 0x69);
		map(Keycode::Insert, 0x72);
		map(Keycode::Delete, 0x75);
		map(Keycode::Sleep, -1);
		map(Keycode::Pause, 0x71);
		
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
		map(Keycode::ScrollLock, 0x6B);
		map(Keycode::NumLock, 0x47);
		map(Keycode::ContextMenu, 0x6E);
	}
	
public:
	void map(Keycode key, sl_uint32 vk)
	{
		if (vk == -1) {
			return;
		}
		mapKeyToVK.put((sl_uint32)key, vk);
		mapVKToKey.put(vk, (sl_uint32)key);
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
		if (mapKeyToVK.get((sl_uint32)code, &ret)) {
			return ret;
		} else {
			return -1;
		}
	}
	
	static _UI_OSX_KeyMapper& instance()
	{
		SLIB_SAFE_STATIC(_UI_OSX_KeyMapper, mapper);
		return mapper;
	}
};

sl_uint32 UIEvent::getSystemKeycode(Keycode key)
{
	return _UI_OSX_KeyMapper::instance().keyToVk(key);
}

Keycode UIEvent::getKeycodeFromSystemKeycode(sl_uint32 vkey)
{
	return _UI_OSX_KeyMapper::instance().vkToKey(vkey);
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


Point UI::getCursorPos()
{
	NSScreen* screen = [NSScreen mainScreen];
	NSRect rect = [screen frame];
	NSPoint pt = [NSEvent mouseLocation];
	return Point(pt.x, rect.size.height - pt.y);
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


SLIB_UI_NAMESPACE_END

#endif
