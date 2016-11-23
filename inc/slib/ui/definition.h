#ifndef CHECKHEADER_SLIB_UI_DEFINITION
#define CHECKHEADER_SLIB_UI_DEFINITION

#include "../core/definition.h"

#define SLIB_UI_NAMESPACE_BEGIN		SLIB_NAMESPACE_BEGIN
#define SLIB_UI_NAMESPACE_END		SLIB_NAMESPACE_END

#include "../math/rectangle.h"
#include "../math/point.h"

typedef sl_int32 sl_ui_pos;
typedef sl_int32 sl_ui_len;
typedef float sl_ui_posf;
typedef double sl_scroll_pos;

SLIB_UI_NAMESPACE_BEGIN

typedef Rectanglei UIRect;
typedef Pointi UIPoint;
typedef Sizei UISize;
typedef Pointf UIPointf;
typedef Sizef UISizef;
typedef Rectanglef UIRectf;
typedef Pointlf ScrollPoint;

SLIB_UI_NAMESPACE_END

#endif
