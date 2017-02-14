#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_CAPTURE
#define CHECKHEADER_SLIB_MEDIA_VIDEO_CAPTURE

#include "definition.h"

#include "video_frame.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/ptr.h"
#include "../core/function.h"

namespace slib
{
	class VideoCapture;
	
	class VideoCaptureFrame : public VideoFrame
	{
	public:
		VideoCaptureFrame();
		
		~VideoCaptureFrame();
		
	};
	
	class SLIB_EXPORT IVideoCaptureListener
	{
	public:
		IVideoCaptureListener();

		virtual ~IVideoCaptureListener();

	public:
		virtual void onCaptureVideoFrame(VideoCapture* capture, VideoCaptureFrame* frame) = 0;
		
	};
	
	class SLIB_EXPORT VideoCaptureParam
	{
	public:
		sl_bool flagAutoStart;
		
		Ptr<IVideoCaptureListener> listener;
		Function<void(VideoCapture*, VideoCaptureFrame*)> onCaptureVideoFrame;
		
	public:
		VideoCaptureParam();
		
		~VideoCaptureParam();
		
	};
	
	class SLIB_EXPORT VideoCapture : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		VideoCapture();
		
		~VideoCapture();
		
	public:
		virtual void release() = 0;
		
		virtual sl_bool isOpened() = 0;
		
		virtual void start() = 0;
		
		virtual void stop() = 0;
		
		virtual sl_bool isRunning() = 0;
		
	public:
		SLIB_PROPERTY(AtomicFunction<void(VideoCapture*, VideoCaptureFrame*)>, OnCaptureVideoFrame)
		
		SLIB_PROPERTY(AtomicPtr<IVideoCaptureListener>, Listener)
		
	protected:
		void _init(const VideoCaptureParam& param);
		
		void _onCaptureVideoFrame(VideoCaptureFrame* frame);
		
	};	
}

#endif
