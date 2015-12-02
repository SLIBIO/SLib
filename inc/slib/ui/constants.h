#ifndef CHECKHEADER_SLIB_UI_CONSTANTS
#define CHECKHEADER_SLIB_UI_CONSTANTS

#include "definition.h"

#include "../graphics/constants.h"

SLIB_UI_NAMESPACE_BEGIN
enum UIEventAction {
	actionUnknown = 0x0000,
	// keyboard
	actionKeyDown = 0x0101,
	actionKeyUp = 0x0102,
	// mouse
	actionMouseMove = 0x0201,
	actionMouseEnter = 0x0202,
	actionMouseLeave = 0x0203,
	actionLeftButtonDown = 0x0204,
	actionLeftButtonUp = 0x0205,
	actionLeftButtonDrag = 0x0206,
	actionRightButtonDown = 0x0207,
	actionRightButtonUp = 0x0208,
	actionRightButtonDrag = 0x0209,
	actionMiddleButtonDown = 0x020a,
	actionMiddleButtonUp = 0x020b,
	actionMiddleButtonDrag = 0x020c,
	actionLeftButtonDoubleClick = 0x020d,
	actionRightButtonDoubleClick = 0x020e,
	actionMiddleButtonDoubleClick = 0x020f,
	actionMouseWheel = 0x0210,
	// touch
	actionTouchBegin = 0x0301,
	actionTouchMove = 0x0302,
	actionTouchEnd = 0x0303,
	actionTouchCancel = 0x0304,
	// other
	actionSetCursor = 0x0401
};

enum Keycode {
	keyUnknown = 0,
	keyBackspace = 0x08,
	keyTab = 0x09,
	keyEnter = 0x0D,
	keyEscape = 0x1B,
	
	keySpace = 0x20,
	keyGrave = 0x21, // ` ~
	keyEqual = 0x22, // = +
	keySemicolon = 0x23, // ; :
	keyBackslash = 0x24, // \ |
	keyLeftBaracket = 0x25, // [ {
	keyRightBaracket = 0x26, // ] }
	keyQuote = 0x27, // ' "
	keyComma = 0x2C, // , <
	keyMinus = 0x2D, // - _
	keyPeriod = 0x2E, // . >
	keyDivide = 0x2F, // / ?
	
	key0 = 0x30,
	key1 = 0x31,
	key2 = 0x32,
	key3 = 0x33,
	key4 = 0x34,
	key5 = 0x35,
	key6 = 0x36,
	key7 = 0x37,
	key8 = 0x38,
	key9 = 0x39,
	
	keyA = 0x41,
	keyB = 0x42,
	keyC = 0x43,
	keyD = 0x44,
	keyE = 0x45,
	keyF = 0x46,
	keyG = 0x47,
	keyH = 0x48,
	keyI = 0x49,
	keyJ = 0x4A,
	keyK = 0x4B,
	keyL = 0x4C,
	keyM = 0x4D,
	keyN = 0x4E,
	keyO = 0x4F,
	keyP = 0x50,
	keyQ = 0x51,
	keyR = 0x52,
	keyS = 0x53,
	keyT = 0x54,
	keyU = 0x55,
	keyV = 0x56,
	keyW = 0x57,
	keyX = 0x58,
	keyY = 0x59,
	keyZ = 0x5A,
	
	keyNumpad0 = 0x60,
	keyNumpad1 = 0x61,
	keyNumpad2 = 0x62,
	keyNumpad3 = 0x63,
	keyNumpad4 = 0x64,
	keyNumpad5 = 0x65,
	keyNumpad6 = 0x66,
	keyNumpad7 = 0x67,
	keyNumpad8 = 0x68,
	keyNumpad9 = 0x69,
	keyNumpadDivide = 0x6A,
	keyNumpadMultiply = 0x6B,
	keyNumpadMinus = 0x6C,
	keyNumpadPlus = 0x6D,
	keyNumpadEnter = 0x6E,
	keyNumpadDecimal = 0x6F,
	
	keyF1 = 0x71,
	keyF2 = 0x72,
	keyF3 = 0x73,
	keyF4 = 0x74,
	keyF5 = 0x75,
	keyF6 = 0x76,
	keyF7 = 0x77,
	keyF8 = 0x78,
	keyF9 = 0x79,
	keyF10 = 0x80,
	keyF11 = 0x81,
	keyF12 = 0x82,
	
	keyPageUp = 0xA0,
	keyPageDown = 0xA1,
	keyHome = 0xA2,
	keyEnd = 0xA3,
	keyLeft = 0xA4,
	keyUp = 0xA5,
	keyRight = 0xA6,
	keyDown = 0xA7,
	keyPrintScreen = 0xA8,
	keyInsert = 0xA9,
	keyDelete = 0xAA,
	keySleep = 0xAB,
	keyPause = 0xAC,
	
	keyGoHome = 0xC0,
	keyGoMenu = 0xC1,
	keyGoBack = 0xC2,
	keyCamera = 0xC3,
	keyVolumeMute = 0xC8,
	keyVolumeDown = 0xC9,
	keyVolumeUp = 0xCA,
	keyMediaPrev = 0xCB,
	keyMediaNext = 0xCC,
	keyMediaPause = 0xCD,
	keyMediaStop = 0xCE,
	keyPhoneStar = 0xD8,
	keyPhonePound = 0xD9,
	
	keyLeftShift = 0xE0,
	keyRightShift = 0xE1,
	keyLeftControl = 0xE2,
	keyRightControl = 0xE3,
	keyLeftAlt = 0xE4,
	keyLeftOption = 0xE4,
	keyRightAlt = 0xE5,
	keyRightOption = 0xE5,
	keyLeftWin = 0xE6,
	keyLeftCommand = 0xE6,
	keyRightWin = 0xE7,
	keyRightCommand = 0xE7,
	keyCapsLock = 0xF0,
	keyScrollLock = 0xF1,
	keyNumLock = 0xF2,
	keyContextMenu = 0xF3
};

enum BoundShape
{
	boundShapeRectangle = 0,
	boundShapeEllipse = 1,
	boundShapeRoundRect = 2,
	boundShapePath = 10
};

enum ButtonState
{
	buttonStateNormal = 0,
	buttonStateHover = 1,
	buttonStateDown = 2,
	buttonStateDisabled = 3,
	
	buttonStatesCount = 4
};

enum LayoutOrientation
{
	layoutOrientationHorizontal = 0,
	layoutOrientationVertical = 1
};

SLIB_UI_NAMESPACE_END

#endif
