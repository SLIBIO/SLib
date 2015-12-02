#ifndef CHECKHEADER_SLIB_GRAPHICS_CONSTANTS
#define CHECKHEADER_SLIB_GRAPHICS_CONSTANTS

#include "definition.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

enum PenStyle {
	penStyleSolid = 0,
	penStyleDot = 1,
	penStyleDash = 2,
	penStyleDashDot = 3,
	penStyleDashDotDot = 4
};

enum LineCap {
	lineCapFlat = 0,
	lineCapRound = 1,
	lineCapSquare = 2
};

enum LineJoin {
	lineJoinMiter = 0,
	lineJoinRound = 1,
	lineJoinBevel = 2
};

enum BrushStyle {
	brushStyleSolid = 0
};

enum FillMode {
	fillModeWinding = 0,
	fillModeAlternate = 1
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
	imageFileTypeUnknown = 0,
	imageFileTypeJPEG = 10,
	imageFileTypePNG = 11,
	imageFileTypeBMP = 12,
	imageFileTypeDDS = 13
};

enum StretchMode
{
	stretchFast = 0,
	stretchHalftone = 1
};

enum BlendMode
{
	blendCopy = 0,
	blendSrcAlpha = 1
};

enum ScaleType
{
	scaleTypeNone = 0,
	scaleTypeStretch = 1,
	scaleTypeContain = 2,
	scaleTypeCover = 3
};

enum GraphicsPathPointType
{
	graphicsPathPointTypeBegin = 0,
	graphicsPathPointTypeLine = 1,
	graphicsPathPointTypeBezierCubic = 3,
	graphicsPathPointFlagClose = 0x80
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
