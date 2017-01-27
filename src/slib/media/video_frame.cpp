#include "../../../inc/slib/media/video_frame.h"

SLIB_MEDIA_NAMESPACE_BEGIN

VideoFrame::VideoFrame()
{
	rotation = RotationMode::Rotate0;
	flip = FlipMode::None;
}

VideoFrame::~VideoFrame()
{
}

SLIB_MEDIA_NAMESPACE_END

