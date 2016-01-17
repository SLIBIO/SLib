#ifndef CHECKHEADER_SLIB_UI_VIDEO_VIEW
#define CHECKHEADER_SLIB_UI_VIDEO_VIEW

#include "definition.h"

#include "render_view.h"

#include "../media/video_frame.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT VideoView : public RenderView
{
	SLIB_DECLARE_OBJECT(VideoView, RenderView)
public:
	VideoView();
	
public:
	void updateCurrentFrame(const VideoFrame* frame);
	
protected:
	// override
	void onFrame(RenderEngine* engine);
	
protected:
	Ref<Texture> m_textureFrame;
	sl_bool m_flagYUV;
	Ref<RenderProgram2D> m_programRGB;
	Ref<RenderProgram2D> m_programYUV;
};
SLIB_UI_NAMESPACE_END

#endif
