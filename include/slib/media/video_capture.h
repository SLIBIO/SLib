/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_MEDIA_VIDEO_CAPTURE
#define CHECKHEADER_SLIB_MEDIA_VIDEO_CAPTURE

#include "definition.h"

#include "video_frame.h"

#include "../core/object.h"
#include "../core/string.h"
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
	
	class SLIB_EXPORT VideoCaptureParam
	{
	public:
		sl_bool flagAutoStart;
		
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
		
	protected:
		void _init(const VideoCaptureParam& param);
		
		void _onCaptureVideoFrame(VideoCaptureFrame* frame);
		
	};	
}

#endif
