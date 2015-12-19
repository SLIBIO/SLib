#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_CAPTURE
#define CHECKHEADER_SLIB_MEDIA_VIDEO_CAPTURE

#include "definition.h"

#include "video_format.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/pointer.h"

SLIB_MEDIA_NAMESPACE_BEGIN

class IVideoCaptureListener;
class SLIB_EXPORT VideoCaptureParam
{
public:
	Ptr<IVideoCaptureListener> listener;
	sl_bool flagAutoStart;

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
	SLIB_DECLARE_OBJECT(VideoCapture, Object)
protected:
	VideoCapture();
public:
	~VideoCapture();

public:
	virtual void release() = 0;
	virtual sl_bool isOpened() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual sl_bool isRunning() = 0;

public:
	SLIB_PROPERTY_INLINE(Ptr<IVideoCaptureListener>, Listener)
	
protected:
	void onCaptureVideoFrame(VideoCaptureFrame* frame);

};

SLIB_MEDIA_NAMESPACE_END

#endif
