#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/event.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/core/hashtable.h"

#include "../../../inc/slib/core/platform_android.h"

SLIB_UI_NAMESPACE_BEGIN

class _UI_Android_KeyMapper
{
private:
	HashTable<sl_uint32, sl_uint32> mapKeyToVK;
	HashTable<sl_uint32, sl_uint32> mapVKToKey;
	
public:
	_UI_Android_KeyMapper()
	{
		map(keyTab, 61);
		map(keyEnter, 66);
		map(keyEscape, 111);
		
		map(keySpace, 62);
		map(keyGrave, 68);
		map(keyEqual, 70);
		map(keySemicolon, 74);
		map(keyBackslash, 73);
		map(keyLeftBaracket, 71);
		map(keyRightBaracket, 72);
		map(keyQuote, 75);
		map(keyComma, 55);
		map(keyMinus, 69);
		map(keyPeriod, 56);
		map(keyDivide, 76);
		
		map(key0, 7);
		map(key1, 8);
		map(key2, 9);
		map(key3, 10);
		map(key4, 11);
		map(key5, 12);
		map(key6, 13);
		map(key7, 14);
		map(key8, 15);
		map(key9, 16);
		
		map(keyA, 29);
		map(keyB, 30);
		map(keyC, 31);
		map(keyD, 32);
		map(keyE, 33);
		map(keyF, 34);
		map(keyG, 35);
		map(keyH, 36);
		map(keyI, 37);
		map(keyJ, 38);
		map(keyK, 39);
		map(keyL, 40);
		map(keyM, 41);
		map(keyN, 42);
		map(keyO, 43);
		map(keyP, 44);
		map(keyQ, 45);
		map(keyR, 46);
		map(keyS, 47);
		map(keyT, 48);
		map(keyU, 49);
		map(keyV, 50);
		map(keyW, 51);
		map(keyX, 52);
		map(keyY, 53);
		map(keyZ, 54);
		
		map(keyNumpad0, 144);
		map(keyNumpad1, 145);
		map(keyNumpad2, 146);
		map(keyNumpad3, 147);
		map(keyNumpad4, 148);
		map(keyNumpad5, 149);
		map(keyNumpad6, 150);
		map(keyNumpad7, 151);
		map(keyNumpad8, 152);
		map(keyNumpad9, 153);
		
		map(keyNumpadDivide, 154);
		map(keyNumpadMultiply, 155);
		map(keyNumpadMinus, 156);
		map(keyNumpadPlus, 157);
		map(keyNumpadEnter, 160);
		map(keyNumpadDecimal, 158);
		
		map(keyF1, 131);
		map(keyF2, 132);
		map(keyF3, 133);
		map(keyF4, 134);
		map(keyF5, 135);
		map(keyF6, 136);
		map(keyF7, 137);
		map(keyF8, 138);
		map(keyF9, 139);
		map(keyF10, 140);
		map(keyF11, 141);
		map(keyF12, 142);
		
		map(keyBackspace, 67);
		map(keyPageUp, 92);
		map(keyPageDown, 93);
		map(keyHome, 122);
		map(keyEnd, 123);
		map(keyLeft, 21);
		map(keyUp, 19);
		map(keyRight, 22);
		map(keyDown, 20);
		map(keyPrintScreen, 120);
		map(keyInsert, 124);
		map(keyDelete, 112);
		map(keySleep, -1);
		map(keyPause, 121);
		
		map(keyGoHome, -1);
		map(keyGoMenu, 82);
		map(keyGoBack, 4);
		map(keyCamera, 27);
		map(keyVolumeMute, 164);
		map(keyVolumeDown, 25);
		map(keyVolumeUp, 24);
		map(keyMediaPrev, 88);
		map(keyMediaNext, 87);
		map(keyMediaPause, 85);
		map(keyMediaStop, 86);
		map(keyPhoneStar, 17);
		map(keyPhonePound, 18);

		map(keyLeftShift, 59);
		map(keyRightShift, 60);
		map(keyLeftControl, 113);
		map(keyRightControl, 114);
		map(keyLeftAlt, 57);
		map(keyRightAlt, 58);
		map(keyLeftWin, 117);
		map(keyRightWin, 118);
		map(keyCapsLock, 115);
		map(keyScrollLock, 116);
		map(keyNumLock, 143);
		map(keyContextMenu, -1);
	}
	
public:
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
	
	static _UI_Android_KeyMapper& instance()
	{
		SLIB_SAFE_STATIC(_UI_Android_KeyMapper, mapper);
		return mapper;
	}
};

sl_uint32 UIEvent::getSystemKeycode(Keycode key)
{
	return _UI_Android_KeyMapper::instance().keyToVk(key);
}

Keycode UIEvent::getKeycodeFromSystemKeycode(sl_uint32 vkey)
{
	return _UI_Android_KeyMapper::instance().vkToKey(vkey);
}

sl_bool UI::checkKeyPressed(Keycode key)
{
	return sl_false;
}

sl_bool UI::checkCapsLockOn()
{
	return sl_false;
}

sl_bool UI::checkNumLockOn()
{
	return sl_false;
}

sl_bool UI::checkScrollLockOn()
{
	return sl_false;
}

Point UI::getCursorPos()
{
	return Point(0, 0);
}

sl_bool UI::checkLeftButtonPressed()
{
	return sl_false;
}

sl_bool UI::checkRightButtonPressed()
{
	return sl_false;
}

sl_bool UI::checkMiddleButtonPressed()
{
	return sl_false;
}

SLIB_UI_NAMESPACE_END

#endif
