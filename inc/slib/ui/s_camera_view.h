#ifndef CHECKHEADER_SLIB_UI_S_CAMERA_VIEW
#define CHECKHEADER_SLIB_UI_S_CAMERA_VIEW

#include "definition.h"

#include "s_view.h"

#include "../capture/video.h"

SLIB_UI_NAMESPACE_BEGIN

class SCameraView;

class SLIB_EXPORT SCameraViewListener
{
public:
	virtual void onCaptureVideoFrame(SCameraView* view, VideoCaptureFrame* frame);
};

class SLIB_EXPORT SCameraView : public SView
{
	SLIB_DECLARE_OBJECT(SCameraView, SView)
public:
	SCameraView();
	
public:
	virtual void start(sl_uint32 cameraIndex);
	virtual void stop();

public:
	SLIB_PROPERTY_INLINE(Ptr<SCameraViewListener>, Listener)
	
public:
	virtual void onCaptureVideoFrame(VideoCaptureFrame* frame);
	
};
SLIB_UI_NAMESPACE_END

#endif
