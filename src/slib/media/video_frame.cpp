#include "../../../inc/slib/media/video_frame.h"

namespace slib
{

	VideoFrame::VideoFrame()
	{
		rotation = RotationMode::Rotate0;
		flip = FlipMode::None;
	}

	VideoFrame::~VideoFrame()
	{
	}

}
