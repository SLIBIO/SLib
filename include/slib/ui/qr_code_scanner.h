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

#ifndef CHECKHEADER_SLIB_UI_QR_CODE_SCANNER
#define CHECKHEADER_SLIB_UI_QR_CODE_SCANNER

#include "definition.h"

#include "camera_view.h"

#include "../core/timer.h"

namespace slib
{

	class SLIB_EXPORT QRCodeScanner : public CameraView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		QRCodeScanner();
		
		~QRCodeScanner();

	public:
		using CameraView::start;
		
		void start(const CameraParam& param) override;
		
		void stop() override;
		
	public:
		SLIB_DECLARE_EVENT_HANDLER(QRCodeScanner, Detect, const String& code)
		
	protected:
		void onDraw(Canvas* canvas) override;
		
		void onCapture(VideoCaptureFrame* frame) override;
		
		void onRunScanner(Timer* timer);

	protected:
		Ref<RenderProgram2D_Position> m_programScanBar;
		Ref<Timer> m_timerScanner;
		sl_bool m_flagUpdateCameraFrame;
		
	};

}

#endif
