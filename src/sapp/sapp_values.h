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

#ifndef CHECKHEADER_SLIB_SDEV_SAPP_VALUES
#define CHECKHEADER_SLIB_SDEV_SAPP_VALUES

#include "slib/core/string.h"
#include "slib/math/vector2.h"
#include "slib/math/vector3.h"
#include "slib/math/vector4.h"
#include "slib/graphics/constants.h"
#include "slib/graphics/color.h"
#include "slib/graphics/font.h"
#include "slib/ui/constants.h"

namespace slib
{
	
	class SAppDocument;
	
	class SAppStringValue
	{
	public:
		sl_bool flagDefined;
		sl_bool flagReferResource;
		String valueOrName;
		
	public:
		SAppStringValue();
		
	public:
		sl_bool parse(const String& str);
				
	};

	class SAppDimensionValue
	{
	public:
		sl_bool flagDefined;
		enum {
			FILL, WRAP, WEIGHT,
			PX,
			SW, SH, SMIN, SMAX,
			VW, VH, VMIN, VMAX,
			SP,
			DP, PT, MM, CM, M, INCH,
			SBAR,
			SAFE_L, SAFE_T, SAFE_R, SAFE_B, SAFE_W, SAFE_H
		};
		int unit;
		sl_real amount;
		
	public:
		SAppDimensionValue();
		
	public:
		String getAccessString() const;
		
		sl_bool parse(const String& str, SAppDocument* doc);
		
		sl_bool checkGlobal();
		
		sl_bool checkSP();
		
		sl_bool checkPosition();
		
		sl_bool checkSize();
		
		sl_bool checkScalarSize();
		
		sl_bool checkScalarSizeOrWeight();
		
		sl_bool checkMargin();
		
		sl_bool checkForWindow();
		
		sl_bool checkForWindowSize();
		
		sl_bool checkForRootViewPosition();
		
		sl_bool checkForRootViewSize();
		
		sl_bool checkForRootViewScalarSize();
		
		sl_bool checkForRootViewMargin();

		sl_bool isNeededOnLayoutFunction();
		
		static sl_bool isRelativeUnit(int unit);
		
		static sl_bool isGlobalUnit(int unit);
		
		static sl_bool isViewportUnit(int unit);
		
	};

	class SAppDimensionFloatValue : public SAppDimensionValue
	{
	public:
		String getAccessString() const;
		
	};

	class SAppBooleanValue
	{
	public:
		sl_bool flagDefined;
		sl_bool value;
		
	public:
		SAppBooleanValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);

	};

	class SAppFloatValue
	{
	public:
		sl_bool flagDefined;
		float value;
		
	public:
		SAppFloatValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppInt32Value
	{
	public:
		sl_bool flagDefined;
		sl_int32 value;
		
	public:
		SAppInt32Value();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppUint32Value
	{
	public:
		sl_bool flagDefined;
		sl_uint32 value;
		
	public:
		SAppUint32Value();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppInt64Value
	{
	public:
		sl_bool flagDefined;
		sl_int64 value;
		
	public:
		SAppInt64Value();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppUint64Value
	{
	public:
		sl_bool flagDefined;
		sl_uint64 value;
		
	public:
		SAppUint64Value();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppVector2Value
	{
	public:
		sl_bool flagDefined;
		Vector2 value;
		
	public:
		SAppVector2Value();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppVector3Value
	{
	public:
		sl_bool flagDefined;
		Vector3 value;
		
	public:
		SAppVector3Value();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppVector4Value
	{
	public:
		sl_bool flagDefined;
		Vector4 value;
		
	public:
		SAppVector4Value();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppVisibilityValue
	{
	public:
		sl_bool flagDefined;
		Visibility value;
		
	public:
		SAppVisibilityValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppPenStyleValue
	{
	public:
		sl_bool flagDefined;
		PenStyle value;
		
	public:
		SAppPenStyleValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	// scrollBars (none, horizontal, vertical, both)
	class SAppScrollBarsValue
	{
	public:
		sl_bool flagDefined;
		sl_bool horizontalScrollBar;
		sl_bool verticalScrollBar;
		
	public:
		SAppScrollBarsValue();
		
	public:
		sl_bool parse(const String& str);
		
	};

	class SAppNameValue
	{
	public:
		sl_bool flagDefined;
		String value;
		
	public:
		SAppNameValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};
	
	class SAppColorValue
	{
	public:
		sl_bool flagDefined;
		
		String resourceName;
		Color color;
		
	public:
		SAppColorValue();
		
	public:
		sl_bool parse(const String& str);
		
	};

	class SAppDrawableValue : public SAppColorValue
	{
	public:
		sl_bool flagNull;
		sl_bool flagColor;
		
		sl_bool flagWhole;
		sl_real x;
		sl_real y;
		sl_real width;
		sl_real height;
		
		enum {
			FUNC_NONE, FUNC_NINEPATCH, FUNC_THREEPATCH_HORIZONTAL, FUNC_THREEPATCH_VERTICAL
		};
		int func;
		SAppDimensionValue patchLeftWidthDst;
		SAppDimensionValue patchRightWidthDst;
		SAppDimensionValue patchTopHeightDst;
		SAppDimensionValue patchBottomHeightDst;
		sl_real patchLeftWidth;
		sl_real patchRightWidth;
		sl_real patchTopHeight;
		sl_real patchBottomHeight;
		
	public:
		SAppDrawableValue();
		
	public:
		sl_bool parse(const String& str, SAppDocument* doc);

		sl_bool parseWhole(const String& str);
		
	};

	class SAppMenuValue
	{
	public:
		sl_bool flagDefined;
		sl_bool flagNull;
		String resourceName;
		
	public:
		SAppMenuValue();
		
	public:
		sl_bool parse(const String& str);

	};

	class SAppAlignLayoutValue
	{
	public:
		sl_bool flagDefined;
		sl_bool flagAlignParent;
		String referingView;
		
	public:
		SAppAlignLayoutValue();
		
	public:
		sl_bool parse(const String& str);
		
	};

	// Scrolling (horizontal, vertical, both)
	class SAppScrollingValue
	{
	public:
		sl_bool flagDefined;
		sl_bool horizontal;
		sl_bool vertical;
		
	public:
		SAppScrollingValue();
		
	public:
		sl_bool parse(const String& str);
		
	};

	class SAppLayoutOrientationValue
	{
	public:
		sl_bool flagDefined;
		LayoutOrientation value;
		
	public:
		SAppLayoutOrientationValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppAlignmentValue
	{
	public:
		sl_bool flagDefined;
		Alignment value;
		
	public:
		SAppAlignmentValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppScaleModeValue
	{
	public:
		sl_bool flagDefined;
		ScaleMode value;
		
	public:
		SAppScaleModeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppBoundShapeValue
	{
	public:
		sl_bool flagDefined;
		BoundShape value;
		
	public:
		SAppBoundShapeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppRedrawModeValue
	{
	public:
		sl_bool flagDefined;
		RedrawMode value;
		
	public:
		SAppRedrawModeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppMultiLineModeValue
	{
	public:
		sl_bool flagDefined;
		MultiLineMode value;
		
	public:
		SAppMultiLineModeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppUIReturnKeyTypeValue
	{
	public:
		sl_bool flagDefined;
		UIReturnKeyType value;
		
	public:
		SAppUIReturnKeyTypeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppUIKeyboardTypeValue
	{
	public:
		sl_bool flagDefined;
		UIKeyboardType value;
		
	public:
		SAppUIKeyboardTypeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppUIAutoCapitalizationTypeValue
	{
	public:
		sl_bool flagDefined;
		UIAutoCapitalizationType value;
		
	public:
		SAppUIAutoCapitalizationTypeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};

	class SAppRotationModeValue
	{
	public:
		sl_bool flagDefined;
		RotationMode value;
		
	public:
		SAppRotationModeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};
	
	class SAppFlipModeValue
	{
	public:
		sl_bool flagDefined;
		FlipMode value;
		
	public:
		SAppFlipModeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};
	
	class SAppEllipsizeModeValue
	{
	public:
		sl_bool flagDefined;
		EllipsizeMode value;
		
	public:
		SAppEllipsizeModeValue();
		
	public:
		String getAccessString();
		
		sl_bool parse(const String& str);
		
	};
	
}

#endif
