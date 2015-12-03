#ifndef CHECKHEADER_SLIB_CAPTURE_VIDEO
#define CHECKHEADER_SLIB_CAPTURE_VIDEO

#include "definition.h"
#include "../core/object.h"
#include "../core/string.h"
#include "../core/pointer.h"

SLIB_CAPTURE_NAMESPACE_BEGIN

class IVideoCaptureListener;
class SLIB_EXPORT VideoCaptureParam
{
public:
	Ptr<IVideoCaptureListener> listener;
	sl_bool flagAutoStart;

public:
	VideoCaptureParam();
};

class VideoCapture;
class VideoCaptureFrame;

class SLIB_EXPORT IVideoCaptureListener
{
public:
	virtual void onCaptureVideoFrame(VideoCapture* capture, VideoCaptureFrame* frame);
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

SLIB_CAPTURE_NAMESPACE_END

#endif
