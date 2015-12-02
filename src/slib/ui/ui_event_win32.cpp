#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/event.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/core/hashtable.h"

SLIB_UI_NAMESPACE_BEGIN
class _UI_Windows_KeyMapper
{
private:
	HashTable<sl_uint32, sl_uint32> mapKeyToVK;
	HashTable<sl_uint32, sl_uint32> mapVKToKey;
	
public:
	_UI_Windows_KeyMapper()
	{
		map(keyTab, VK_TAB);
		map(keyEnter, VK_RETURN);
		map(keyEscape, VK_ESCAPE);
		
		map(keySpace, VK_SPACE);
		map(keyGrave, VK_OEM_3);
		map(keyEqual, VK_OEM_PLUS);
		map(keySemicolon, VK_OEM_1);
		map(keyBackslash, VK_OEM_5);
		map(keyLeftBaracket, VK_OEM_4);
		map(keyRightBaracket, VK_OEM_6);
		map(keyQuote, VK_OEM_7);
		map(keyComma, VK_OEM_COMMA);
		map(keyMinus, VK_OEM_MINUS);
		map(keyPeriod, VK_OEM_PERIOD);
		map(keyDivide, VK_OEM_2);
		
		map(key0, '0');
		map(key1, '1');
		map(key2, '2');
		map(key3, '3');
		map(key4, '4');
		map(key5, '5');
		map(key6, '6');
		map(key7, '7');
		map(key8, '8');
		map(key9, '9');
		
		map(keyA, 'A');
		map(keyB, 'B');
		map(keyC, 'C');
		map(keyD, 'D');
		map(keyE, 'E');
		map(keyF, 'F');
		map(keyG, 'G');
		map(keyH, 'H');
		map(keyI, 'I');
		map(keyJ, 'J');
		map(keyK, 'K');
		map(keyL, 'L');
		map(keyM, 'M');
		map(keyN, 'N');
		map(keyO, 'O');
		map(keyP, 'P');
		map(keyQ, 'Q');
		map(keyR, 'R');
		map(keyS, 'S');
		map(keyT, 'T');
		map(keyU, 'U');
		map(keyV, 'V');
		map(keyW, 'W');
		map(keyX, 'X');
		map(keyY, 'Y');
		map(keyZ, 'Z');
		
		map(keyNumpad0, VK_NUMPAD0);
		map(keyNumpad1, VK_NUMPAD1);
		map(keyNumpad2, VK_NUMPAD2);
		map(keyNumpad3, VK_NUMPAD3);
		map(keyNumpad4, VK_NUMPAD4);
		map(keyNumpad5, VK_NUMPAD5);
		map(keyNumpad6, VK_NUMPAD6);
		map(keyNumpad7, VK_NUMPAD7);
		map(keyNumpad8, VK_NUMPAD8);
		map(keyNumpad9, VK_NUMPAD9);
		
		map(keyNumpadDivide, VK_DIVIDE);
		map(keyNumpadMultiply, VK_MULTIPLY);
		map(keyNumpadMinus, VK_SUBTRACT);
		map(keyNumpadPlus, VK_ADD);
		mapKeyToVK.put(keyNumpadEnter, VK_RETURN);
		map(keyNumpadDecimal, VK_DECIMAL);
		
		map(keyF1, VK_F1);
		map(keyF2, VK_F2);
		map(keyF3, VK_F3);
		map(keyF4, VK_F4);
		map(keyF5, VK_F5);
		map(keyF6, VK_F6);
		map(keyF7, VK_F7);
		map(keyF8, VK_F8);
		map(keyF9, VK_F9);
		map(keyF10, VK_F10);
		map(keyF11, VK_F11);
		map(keyF12, VK_F12);
		
		map(keyBackspace, VK_BACK);
		map(keyPageUp, VK_PRIOR);
		map(keyPageDown, VK_NEXT);
		map(keyHome, VK_HOME);
		map(keyEnd, VK_END);
		map(keyLeft, VK_LEFT);
		map(keyUp, VK_UP);
		map(keyRight, VK_RIGHT);
		map(keyDown, VK_DOWN);
		map(keyPrintScreen, VK_SNAPSHOT);
		map(keyInsert, VK_INSERT);
		map(keyDelete, VK_DELETE);
		map(keySleep, VK_SLEEP);
		map(keyPause, VK_PAUSE);
		
		map(keyGoHome, -1);
		map(keyGoMenu, -1);
		map(keyGoBack, -1);
		map(keyCamera, -1);
		map(keyVolumeMute, VK_VOLUME_MUTE);
		map(keyVolumeDown, VK_VOLUME_DOWN);
		map(keyVolumeUp, VK_VOLUME_UP);
		map(keyMediaPrev, VK_MEDIA_PREV_TRACK);
		map(keyMediaNext, VK_MEDIA_NEXT_TRACK);
		map(keyMediaPause, VK_MEDIA_PLAY_PAUSE);
		map(keyMediaStop, VK_MEDIA_STOP);
		map(keyPhoneStar, -1);
		map(keyPhonePound, -1);
		
		map(keyLeftShift, VK_LSHIFT);
		map(keyRightShift, VK_RSHIFT);
		map(keyLeftControl, VK_LCONTROL);
		map(keyRightControl, VK_RCONTROL);
		map(keyLeftAlt, VK_LMENU);
		map(keyRightAlt, VK_RMENU);
		map(keyLeftWin, VK_LWIN);
		map(keyRightWin, VK_RWIN);
		map(keyCapsLock, VK_CAPITAL);
		map(keyScrollLock, VK_SCROLL);
		map(keyNumLock, VK_NUMLOCK);
		map(keyContextMenu, VK_APPS);
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
	
	static _UI_Windows_KeyMapper& instance()
	{
		SLIB_SAFE_STATIC(_UI_Windows_KeyMapper, mapper);
		return mapper;
	}
};

sl_uint32 UIEvent::getSystemKeycode(Keycode key)
{
	return _UI_Windows_KeyMapper::instance().keyToVk(key);
}

Keycode UIEvent::getKeycodeFromSystemKeycode(sl_uint32 vkey)
{
	return _UI_Windows_KeyMapper::instance().vkToKey(vkey);
}

sl_bool UI::checkKeyPressed(Keycode key)
{
	sl_uint32 vk = UIEvent::getSystemKeycode(key);
	if (vk != -1) {
		return (GetAsyncKeyState(vk) & 0x8000) != 0;
	} else {
		return sl_false;
	}
}

sl_bool UI::checkCapsLockOn()
{
	return (GetAsyncKeyState(VK_CAPITAL) & 1) != 0;
}

sl_bool UI::checkNumLockOn()
{
	return (GetAsyncKeyState(VK_NUMLOCK) & 1) != 0;
}

sl_bool UI::checkScrollLockOn()
{
	return (GetAsyncKeyState(VK_SCROLL) & 1) != 0;
}

Point UI::getCursorPos()
{
	POINT pt;
	::GetCursorPos(&pt);
	return Point((sl_real)(pt.x), (sl_real)(pt.y));
}

sl_bool UI::checkLeftButtonPressed()
{
	return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

sl_bool UI::checkRightButtonPressed()
{
	return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
}

sl_bool UI::checkMiddleButtonPressed()
{
	return (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
}

SLIB_UI_NAMESPACE_END

#endif
