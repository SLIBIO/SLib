#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_FRAME
#define CHECKHEADER_SLIB_MEDIA_VIDEO_FRAME

#include "definition.h"

#include "../graphics/color.h"
#include "../graphics/bitmap.h"

namespace slib
{
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
}

#endif
