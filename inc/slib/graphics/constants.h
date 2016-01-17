#ifndef CHECKHEADER_SLIB_GRAPHICS_CONSTANTS
#define CHECKHEADER_SLIB_GRAPHICS_CONSTANTS

#include "definition.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

enum PenStyle {
	penStyle_Solid = 0,
	penStyle_Dot = 1,
	penStyle_Dash = 2,
	penStyle_DashDot = 3,
	penStyle_DashDotDot = 4
};

enum LineCap {
	lineCap_Flat = 0,
	lineCap_Round = 1,
	lineCap_Square = 2
};

enum LineJoin {
	lineJoin_Miter = 0,
	lineJoin_Round = 1,
	lineJoin_Bevel = 2
};

enum BrushStyle {
	brushStyle_Solid = 0
};

enum FillMode {
	fillMode_Winding = 0,
	fillMode_Alternate = 1
};

enum Alignment
{
	alignHorizontalMask = 3,
	alignVerticalMask = 12,
	
	alignCenter = 0,
	alignLeft = 1,
	alignRight = 2,
	
	alignMiddle = 0,
	alignTop = 4,
	alignBottom = 8,
	
	alignMiddleCenter = 0,
	alignMiddleLeft = 1,
	alignMiddleRight = 2,
	alignTopCenter = 4,
	alignTopLeft = 5,
	alignTopRight = 6,
	alignBottomCenter = 8,
	alignBottomLeft = 9,
	alignBottomRight = 10
};

enum ImageFileType
{
	imageFileType_Unknown = 0,
	imageFileType_JPEG = 10,
	imageFileType_PNG = 11,
	imageFileType_BMP = 12,
	imageFileType_DDS = 13
};

enum StretchMode
{
	stretchMode_Fast = 0,
	stretchMode_Halftone = 1
};

enum BlendMode
{
	blendMode_Copy = 0,
	blendMode_SrcAlpha = 1
};

enum RotationMode
{
	rotationMode_0 = 0,
	rotationMode_90 = 90,
	rotationMode_180 = 180,
	rotationMode_270 = 270
};

enum FlipMode
{
	flipMode_None = 0,
	flipMode_Horizontal = 1,
	flipMode_Vertical = 2,
	flipMode_HorizontalAndVertical = 3
};

enum ScaleMode
{
	scaleMode_None = 0,
	scaleMode_Stretch = 1,
	scaleMode_Contain = 2,
	scaleMode_Cover = 3
};

enum GraphicsPathPointType
{
	graphicsPathPointType_Begin = 0,
	graphicsPathPointType_Line = 1,
	graphicsPathPointType_BezierCubic = 3
    
};

enum GraphicsPathPointFlag
{
	graphicsPathPointFlag_Close = 0x80
};

enum ColorSpace
{
	colorSpace_None = 0,
	colorSpace_RGB = 1,
	colorSpace_YUV = 2,
	colorSpace_CMYK = 3,
	colorSpace_HLS = 4,
	colorSpace_HSV = 5
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
