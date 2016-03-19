#include "../../../inc/slib/media/video_frame.h"

SLIB_MEDIA_NAMESPACE_BEGIN

VideoFrame::VideoFrame()
{
	rotation = RotationMode::Rotate0;
	flip = FlipMode::None;
}

SLIB_MEDIA_NAMESPACE_END

