#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_CAPTURE
#define CHECKHEADER_SLIB_MEDIA_VIDEO_CAPTURE

#include "definition.h"

#include "video_frame.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/ptr.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class IVideoCaptureListener;

class SLIB_EXPORT VideoCaptureParam
{
public:
	sl_bool flagAutoStart;

	Ptr<IVideoCaptureListener> listener;
	
public:
	VideoCaptureParam();
	
};

class VideoCaptureFrame : public VideoFrame
{
};

class VideoCapture;

class SLIB_EXPORT IVideoCaptureListener
{
public:
	virtual void onCaptureVideoFrame(VideoCapture* capture, VideoCaptureFrame* frame) = 0;
	
};

class SLIB_EXPORT VideoCapture : public Object
{
	SLIB_DECLARE_OBJECT

public:
	virtual void release() = 0;
	
	virtual sl_bool isOpened() = 0;
	
	virtual void start() = 0;
	
	virtual void stop() = 0;
	
	virtual sl_bool isRunning() = 0;
	
protected:
	void onCaptureVideoFrame(VideoCaptureFrame* frame);

protected:
	Ptr<IVideoCaptureListener> m_listener;
	
};

SLIB_MEDIA_NAMESPACE_END

#endif
