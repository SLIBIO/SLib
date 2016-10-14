#ifndef CHECKHEADER_SLIB_GRAPHICS_CONSTANTS
#define CHECKHEADER_SLIB_GRAPHICS_CONSTANTS

#include "definition.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

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
	SLIB_DECLARE_FLAGS(Alignment)
public:
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
	Fast = 0,
	Halftone = 1
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
	Both = 3
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

SLIB_GRAPHICS_NAMESPACE_END

#endif
