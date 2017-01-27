#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_FRAME
#define CHECKHEADER_SLIB_MEDIA_VIDEO_FRAME

#include "definition.h"

#include "../graphics/color.h"
#include "../graphics/bitmap.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class SLIB_EXPORT VideoFrame
{
public:
	BitmapData image;
	RotationMode rotation;
	FlipMode flip;
	
public:
	VideoFrame();
	
	~VideoFrame();
	
};

SLIB_MEDIA_NAMESPACE_END

#endif
