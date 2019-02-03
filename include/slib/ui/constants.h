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

#ifndef CHECKHEADER_SLIB_UI_CONSTANTS
#define CHECKHEADER_SLIB_UI_CONSTANTS

#include "definition.h"

#include "../graphics/constants.h"

namespace slib
{

	enum class UIAction
	{
		Unknown = 0x0000,
		// keyboard
		KeyDown = 0x0101,
		KeyUp = 0x0102,
		// mouse
		MouseMove = 0x0201,
		MouseEnter = 0x0202,
		MouseLeave = 0x0203,
		LeftButtonDown = 0x0204,
		LeftButtonUp = 0x0205,
		LeftButtonDrag = 0x0206,
		RightButtonDown = 0x0207,
		RightButtonUp = 0x0208,
		RightButtonDrag = 0x0209,
		MiddleButtonDown = 0x020a,
		MiddleButtonUp = 0x020b,
		MiddleButtonDrag = 0x020c,
		LeftButtonDoubleClick = 0x020d,
		RightButtonDoubleClick = 0x020e,
		MiddleButtonDoubleClick = 0x020f,
		// mouse wheel
		MouseWheel = 0x0210,
		// touch
		TouchBegin = 0x0301,
		TouchMove = 0x0302,
		TouchEnd = 0x0303,
		TouchCancel = 0x0304,
		// other
		SetCursor = 0x0401
	};
	
	class UIEventFlags
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(UIEventFlags, value)
		
	public:
		enum {
			PreventDefault = 0x0001,
			StopPropagation = 0x0002,
			PassToNext = 0x0004,
			DispatchToParentInstance = 0x1000,
			FromChildInstance = 0x2000
		};
	};
	
	enum class TouchPhase
	{
		Move = 0,
		Begin = 1,
		End = 2,
		Cancel = 3
	};
	
	enum class Keycode
	{
		Unknown = 0,
		Backspace = 0x08,
		Tab = 0x09,
		Enter = 0x0D,
		Escape = 0x1B,
		
		Space = 0x20,
		Grave = 0x21, // ` ~
		Equal = 0x22, // = +
		Semicolon = 0x23, // ; :
		Backslash = 0x24, // \ |
		LeftBaracket = 0x25, // [ {
		RightBaracket = 0x26, // ] }
		Quote = 0x27, // ' "
		Comma = 0x2C, // , <
		Minus = 0x2D, // - _
		Period = 0x2E, // . >
		Divide = 0x2F, // / ?
		
		Num0 = 0x30,
		Num1 = 0x31,
		Num2 = 0x32,
		Num3 = 0x33,
		Num4 = 0x34,
		Num5 = 0x35,
		Num6 = 0x36,
		Num7 = 0x37,
		Num8 = 0x38,
		Num9 = 0x39,
		
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		
		Numpad0 = 0x60,
		Numpad1 = 0x61,
		Numpad2 = 0x62,
		Numpad3 = 0x63,
		Numpad4 = 0x64,
		Numpad5 = 0x65,
		Numpad6 = 0x66,
		Numpad7 = 0x67,
		Numpad8 = 0x68,
		Numpad9 = 0x69,
		NumpadDivide = 0x6A,
		NumpadMultiply = 0x6B,
		NumpadMinus = 0x6C,
		NumpadPlus = 0x6D,
		NumpadEnter = 0x6E,
		NumpadDecimal = 0x6F,
		
		F1 = 0x71,
		F2 = 0x72,
		F3 = 0x73,
		F4 = 0x74,
		F5 = 0x75,
		F6 = 0x76,
		F7 = 0x77,
		F8 = 0x78,
		F9 = 0x79,
		F10 = 0x80,
		F11 = 0x81,
		F12 = 0x82,
		
		PageUp = 0xA0,
		PageDown = 0xA1,
		Home = 0xA2,
		End = 0xA3,
		Left = 0xA4,
		Up = 0xA5,
		Right = 0xA6,
		Down = 0xA7,
		PrintScreen = 0xA8,
		Insert = 0xA9,
		Delete = 0xAA,
		Sleep = 0xAB,
		Pause = 0xAC,
		
		GoHome = 0xC0,
		GoMenu = 0xC1,
		GoBack = 0xC2,
		Camera = 0xC3,
		VolumeMute = 0xC8,
		VolumeDown = 0xC9,
		VolumeUp = 0xCA,
		MediaPrev = 0xCB,
		MediaNext = 0xCC,
		MediaPause = 0xCD,
		MediaStop = 0xCE,
		PhoneStar = 0xD8,
		PhonePound = 0xD9,
		
		LeftShift = 0xE0,
		RightShift = 0xE1,
		LeftControl = 0xE2,
		RightControl = 0xE3,
		LeftAlt = 0xE4,
		LeftOption = 0xE4,
		RightAlt = 0xE5,
		RightOption = 0xE5,
		LeftWin = 0xE6,
		LeftCommand = 0xE6,
		RightWin = 0xE7,
		RightCommand = 0xE7,
		CapsLock = 0xF0,
		ScrollLock = 0xF1,
		NumLock = 0xF2,
		ContextMenu = 0xF3
	};
	
	class Modifiers
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(Modifiers, value)
		
	public:
		enum {
			Mask = 0xFFFF0000,
			
			Control = 0x00010000,
			Alt = 0x00020000,
			Option = Alt,
			Shift = 0x00040000,
			Windows = 0x00080000,
			Command = Windows
		};
	};
	
	enum class BoundShape
	{
		Rectangle = 0,
		Ellipse = 1,
		RoundRect = 2,
		Path = 10
	};
	
	enum class ButtonState
	{
		Normal = 0,
		Hover = 1,
		Pressed = 2,
		Disabled = 3,
		
		Count = 4,
		All = 4
	};
	
	enum class UIUpdateMode
	{
		None = 0,
		Redraw = 1,
		UpdateLayout = 3,
		Animate = 5,
		Init = 0x8000
	};
	
#define SLIB_UI_UPDATE_MODE_IS_REDRAW(mode) (((int)(mode)) & ((int)(UIUpdateMode::Redraw)))
#define SLIB_UI_UPDATE_MODE_IS_UPDATE_LAYOUT(mode) ((((int)(mode)) & ((int)(UIUpdateMode::UpdateLayout))) == ((int)(UIUpdateMode::UpdateLayout)))
#define SLIB_UI_UPDATE_MODE_IS_ANIMATE(mode) ((((int)(mode)) & ((int)(UIUpdateMode::Animate))) == ((int)(UIUpdateMode::Animate)))
#define SLIB_UI_UPDATE_MODE_IS_INIT(mode) ((mode) == UIUpdateMode::Init)

	enum class RedrawMode
	{
		Continuously = 0,
		WhenDirty = 1
	};
	
	enum class Visibility
	{
		Visible = 0,
		Hidden = 1,
		Gone = 2,
	};
	
	enum class LayoutOrientation
	{
		Horizontal = 0,
		Vertical = 1
	};
	
	enum class SizeMode
	{
		Fixed = 0,
		Filling = 1,
		Wrapping = 2,
		Weight = 3
	};
	
	enum class PositionMode
	{
		Free = 0,
		ParentEdge = 1,
		OtherStart = 2,
		OtherEnd = 3,
		CenterInParent = 4,
		CenterInOther = 5
	};
	
	enum class AspectRatioMode
	{
		None = 0,
		AdjustWidth = 1,
		AdjustHeight = 2
	};
	
	class DialogResult
	{
	public:
		int value;
		SLIB_MEMBERS_OF_PRIMITIVE_WRAPPER(DialogResult, int, value)

	public:
		enum {
			Ok = 0,
			Yes = 1,
			No = 2,
			Cancel = 3
		};

	public:
		constexpr DialogResult(): value(Ok) {}

	};
	
	enum class UIAttachMode
	{
		NotAttach = 0,
		AttachAlways = 1,
		NotAttachInNativeWidget = 2,
		AttachInNativeWidget = 3
	};
	
	enum class GestureType
	{
		SwipeLeft = 0,
		SwipeRight = 1,
		SwipeUp = 2,
		SwipeDown = 3,
		
		Count = 4
	};
	
	enum class UIPageAction
	{
		Push = 0,
		Pop = 1,
		Resume = 2,
		Pause = 3
	};
	
	enum class UIReturnKeyType
	{
		Default = 0,
		Return = 1,
		Done = 2,
		Search = 3,
		Next = 4,
		Continue = 5,
		Go = 6,
		Send = 7,
		Join = 8,
		Route = 9,
		EmergencyCall = 10,
		Google = 11,
		Yahoo = 12
	};
	
	enum class UIKeyboardType
	{
		Default = 0,
		Numpad = 1,
		Phone = 2,
		Email = 3,
		Decimal = 4,
		Alphabet = 5,
		Url = 6,
		WebSearch = 7,
		Twitter = 8,
		NumbersAndPunctuation = 9,
		NamePhone = 10,
		Ascii = 11,
		AsciiNumpad = 12
	};
	
	enum class UIAutoCapitalizationType
	{
		None = 0,
		Words = 1,
		Sentences = 2,
		AllCharacters = 3
	};

	enum class ScreenOrientation
	{
		Portrait = 0,
		LandscapeRight = 90,
		PortraitUpsideDown = 180,
		LandscapeLeft = 270
	};
	
	enum class StatusBarStyle
	{
		Hidden = 0,
		Dark = 1,
		Light = 2
	};
	
	struct UIEdgeInsets
	{
		sl_ui_len left;
		sl_ui_len top;
		sl_ui_len right;
		sl_ui_len bottom;
	};
	
}

#endif
