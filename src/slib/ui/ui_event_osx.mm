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
		map(keyTab, 0x30);
		map(keyEnter, 0x24);
		map(keyEscape, 0x35);
		
		map(keySpace, 0x31);
		map(keyGrave, 0x32);
		map(keyEqual, 0x18);
		map(keySemicolon, 0x29);
		map(keyBackslash, 0x2A);
		map(keyLeftBaracket, 0x21);
		map(keyRightBaracket, 0x1E);
		map(keyQuote, 0x27);
		map(keyComma, 0x2B);
		map(keyMinus, 0x1B);
		map(keyPeriod, 0x2F);
		map(keyDivide, 0x2C);
		
		map(key0, 0x1D);
		map(key1, 0x12);
		map(key2, 0x13);
		map(key3, 0x14);
		map(key4, 0x15);
		map(key5, 0x17);
		map(key6, 0x16);
		map(key7, 0x1A);
		map(key8, 0x1C);
		map(key9, 0x19);
		
		map(keyA, 0x00);
		map(keyB, 0x0B);
		map(keyC, 0x08);
		map(keyD, 0x02);
		map(keyE, 0x0E);
		map(keyF, 0x03);
		map(keyG, 0x05);
		map(keyH, 0x04);
		map(keyI, 0x22);
		map(keyJ, 0x26);
		map(keyK, 0x28);
		map(keyL, 0x25);
		map(keyM, 0x2E);
		map(keyN, 0x2D);
		map(keyO, 0x1F);
		map(keyP, 0x23);
		map(keyQ, 0x0C);
		map(keyR, 0x0F);
		map(keyS, 0x01);
		map(keyT, 0x11);
		map(keyU, 0x20);
		map(keyV, 0x09);
		map(keyW, 0x0D);
		map(keyX, 0x07);
		map(keyY, 0x10);
		map(keyZ, 0x06);
		
		map(keyNumpad0, 0x52);
		map(keyNumpad1, 0x53);
		map(keyNumpad2, 0x54);
		map(keyNumpad3, 0x55);
		map(keyNumpad4, 0x56);
		map(keyNumpad5, 0x57);
		map(keyNumpad6, 0x58);
		map(keyNumpad7, 0x59);
		map(keyNumpad8, 0x5B);
		map(keyNumpad9, 0x5C);
		
		map(keyNumpadDivide, 0x4B);
		map(keyNumpadMultiply, 0x43);
		map(keyNumpadMinus, 0x4E);
		map(keyNumpadPlus, 0x45);
		map(keyNumpadEnter, 0x4C);
		map(keyNumpadDecimal, 0x41);
		
		map(keyF1, 0x7A);
		map(keyF2, 0x78);
		map(keyF3, 0x63);
		map(keyF4, 0x76);
		map(keyF5, 0x60);
		map(keyF6, 0x61);
		map(keyF7, 0x62);
		map(keyF8, 0x64);
		map(keyF9, 0x65);
		map(keyF10, 0x6D);
		map(keyF11, 0x67);
		map(keyF12, 0x6F);
		
		map(keyBackspace, 0x33);
		map(keyPageUp, 0x74);
		map(keyPageDown, 0x79);
		map(keyHome, 0x73);
		map(keyEnd, 0x77);
		map(keyLeft, 0x7B);
		map(keyUp, 0x7E);
		map(keyRight, 0x7C);
		map(keyDown, 0x7D);
		map(keyPrintScreen, 0x69);
		map(keyInsert, 0x72);
		map(keyDelete, 0x75);
		map(keySleep, -1);
		map(keyPause, 0x71);
		
		map(keyGoHome, -1);
		map(keyGoMenu, -1);
		map(keyGoBack, -1);
		map(keyCamera, -1);
		map(keyVolumeMute, 0x4A);
		map(keyVolumeDown, 0x49);
		map(keyVolumeUp, 0x48);
		map(keyMediaPrev, -1);
		map(keyMediaNext, -1);
		map(keyMediaPause, -1);
		map(keyMediaStop, -1);
		map(keyPhoneStar, -1);
		map(keyPhonePound, -1);
		
		map(keyLeftShift, 0x38);
		map(keyRightShift, 0x3C);
		map(keyLeftControl, 0x3B);
		map(keyRightControl, 0x3E);
		map(keyLeftOption, 0x3A);
		map(keyRightOption, 0x3D);
		map(keyLeftCommand, 0x37);
		mapKeyToVK.put(keyRightCommand, 0x37);
		map(keyCapsLock, 0x39);
		map(keyScrollLock, 0x6B);
		map(keyNumLock, 0x47);
		map(keyContextMenu, 0x6E);
	}
	
	void map(sl_uint32 key, sl_uint32 vk)
	{
		if (vk == -1) {
			return;
		}
		mapKeyToVK.put(key, vk);
		mapVKToKey.put(vk, key);
	}
	
	Keycode vkToKey(sl_uint32 vk)
	{
		sl_uint32 ret;
		if (mapVKToKey.get(vk, &ret)) {
			return (Keycode)ret;
		} else {
			return keyUnknown;
		}
	}
	
	sl_uint32 keyToVk(Keycode code)
	{
		sl_uint32 ret;
		if (mapKeyToVK.get(code, &ret)) {
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
