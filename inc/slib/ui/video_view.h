#ifndef CHECKHEADER_SLIB_UI_VIDEO_VIEW
#define CHECKHEADER_SLIB_UI_VIDEO_VIEW

#include "definition.h"

#include "render_view.h"

#include "../media/video_format.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT VideoView : public RenderView
{
	SLIB_DECLARE_OBJECT(VideoView, RenderView)
public:
	VideoView();
	
public:
	
};
SLIB_UI_NAMESPACE_END

#endif
