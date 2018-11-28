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

#ifndef CHECKHEADER_SLIB_GRAPHICS_CONSTANTS
#define CHECKHEADER_SLIB_GRAPHICS_CONSTANTS

#include "definition.h"

#include "../core/macro.h"

namespace slib
{
	
	enum class PenStyle
	{
		Solid = 0,
		Dot = 1,
		Dash = 2,
		DashDot = 3,
		DashDotDot = 4
	};

	enum class LineCap
	{
		Flat = 0,
		Round = 1,
		Square = 2
	};

	enum class LineJoin
	{
		Miter = 0,
		Round = 1,
		Bevel = 2
	};

	enum class BrushStyle
	{
		Solid = 0
	};

	enum class FillMode
	{
		Winding = 0,
		Alternate = 1
	};

	class Alignment
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(Alignment, value)
		
		enum
		{
			HorizontalMask = 3,
			VerticalMask = 12,

			Center = 0,
			Left = 1,
			Right = 2,

			Middle = 0,
			Top = 4,
			Bottom = 8,

			MiddleCenter = 0,
			MiddleLeft = 1,
			MiddleRight = 2,
			TopCenter = 4,
			TopLeft = 5,
			TopRight = 6,
			BottomCenter = 8,
			BottomLeft = 9,
			BottomRight = 10
		};
	};

	enum class ImageFileType
	{
		Unknown = 0,
		JPEG = 10,
		PNG = 11,
		BMP = 12,
		DDS = 13
	};

	enum class StretchMode
	{
		Nearest = 0,
		Linear = 1,
		Box = 2,
		
		Default = Box
	};

	enum class BlendMode
	{
		Copy = 0,
		SrcAlpha = 1
	};

	enum class RotationMode
	{
		Rotate0 = 0,
		Rotate90 = 90,
		Rotate180 = 180,
		Rotate270 = 270
	};

	enum class FlipMode
	{
		None = 0,
		Horizontal = 1,
		Vertical = 2,
		Both = 3 // Same effect with `RotationMode::Rotate180`
	};

	enum class ScaleMode
	{
		None = 0,
		Stretch = 1,
		Contain = 2,
		Cover = 3
	};

	enum class ColorSpace
	{
		None = 0,
		RGB = 1,
		YUV = 2,
		CMYK = 3,
		HLS = 4,
		HSV = 5
	};
	
	enum class MultiLineMode
	{
		Single, // Single line
		Multiple, // Break only at CR/LF
		WordWrap, // Break at TAB, Spaces, CR/LF
		BreakWord // Break at any position
	};

}

#endif
