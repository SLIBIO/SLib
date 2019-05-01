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

#include "slib/ui/camera_view.h"

#include "slib/ui/resource.h"

#include "../resources.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(CameraView, VideoView)
	
	CameraView::CameraView()
	{
		m_flagAutoStart = sl_false;
		m_flashMode = CameraFlashMode::Auto;
		
		m_flagTouchFocus = sl_false;
		m_flagDuringTouchFocusEffect = sl_false;
	}
	
	CameraView::~CameraView()
	{
	}

	void CameraView::start()
	{
		ObjectLocker lock(this);
		Ref<Camera> camera = m_camera;
		if (camera.isNotNull()) {
			return;
		}
		CameraParam param;
		String deviceId = m_deviceId;
		if (deviceId.isNotEmpty()) {
			if (deviceId.equalsIgnoreCase("FRONT")) {
				param.setFrontCamera();
			} else if (deviceId.equalsIgnoreCase("BACK")) {
				param.setBackCamera();
			} else {
				param.deviceId = deviceId;
			}
		} else {
			param.setBackCamera();
		}
		start(param);
	}
	
	void CameraView::start(const CameraParam& _param)
	{
		ObjectLocker lock(this);
		stop();
		CameraParam param(_param);
		param.onCaptureVideoFrame = SLIB_FUNCTION_WEAKREF(CameraView, _onCaptureCameraFrame, this);
		m_camera = Camera::create(param);
	}
	
	void CameraView::stop()
	{
		ObjectLocker lock(this);
		Ref<Camera> camera = m_camera;
		if (camera.isNotNull()) {
			camera->release();
		}
		m_camera.setNull();
	}
	
	sl_bool CameraView::isAutoStart()
	{
		return m_flagAutoStart;
	}
	
	void CameraView::setAutoStart(sl_bool flagAutoStart)
	{
		m_flagAutoStart = flagAutoStart;
	}
	
	String CameraView::getDeviceId()
	{
		ObjectLocker lock(this);
		return m_deviceId;
	}
	
	void CameraView::setDeviceId(const String& deviceId)
	{
		ObjectLocker lock(this);
		if (m_deviceId == deviceId) {
			return;
		}
		m_deviceId = deviceId;
		if (m_camera.isNotNull()) {
			stop();
			start();
		}
	}
	
	Ref<Camera> CameraView::getCamera()
	{
		ObjectLocker lock(this);
		return m_camera;
	}
	
	class _priv_CameraView : public CameraView
	{
		friend class _priv_CameraView_Controls;
	};
	
	class _priv_CameraView_ShutterIcon : public Drawable
	{
	public:
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override
		{
			if (param.useColorMatrix) {
				canvas->fillEllipse(rectDst, param.colorMatrix.transformColor(Color::White));
			} else {
				canvas->fillEllipse(rectDst, Color::White);
			}
		}
		
	};
	
	class _priv_CameraView_Controls : public ui::CameraControlView
	{
	public:
		_priv_CameraView* cameraView;
		
	public:
		_priv_CameraView_Controls(CameraView* _cameraView)
		{
			cameraView = (_priv_CameraView*)_cameraView;
		}
		
	public:
		void init() override
		{
			ui::CameraControlView::init();
			btnShutter->setIcon(new _priv_CameraView_ShutterIcon, UIUpdateMode::Init);
			btnShutter->setPadding((sl_ui_len)(UIResource::dpToPixel(10)), UIUpdateMode::Init);
			btnShutter->setOnClick([this](View* view) {
				cameraView->onClickShutter();
			});
#if defined(SLIB_PLATFORM_IS_MOBILE)
			btnSwitch->setOnClick([this](View* view) {
				if (cameraView->getDeviceId().equalsIgnoreCase("FRONT")) {
					cameraView->setDeviceId("BACK");
				} else {
					cameraView->setDeviceId("FRONT");
				}
				cameraView->start();
			});
			btnFlash->setIcon(getFlashIcon(cameraView->getFlashMode()), UIUpdateMode::Init);
			btnFlash->setOnClick([this](View* view) {
				switch (cameraView->getFlashMode()) {
					case CameraFlashMode::On:
						cameraView->setFlashMode(CameraFlashMode::Off);
						break;
					case CameraFlashMode::Off:
						cameraView->setFlashMode(CameraFlashMode::Auto);
						break;
					default:
						cameraView->setFlashMode(CameraFlashMode::On);
						break;
				}
			});
#else
			btnSwitch->setVisible(sl_false, UIUpdateMode::Init);
			btnFlash->setVisible(sl_false, UIUpdateMode::Init);
#endif
		}
		
		static Ref<Drawable> getFlashIcon(CameraFlashMode flash)
		{
			switch (flash) {
				case CameraFlashMode::On:
					return drawable::camera_view_control_flash_on::get();
				case CameraFlashMode::Off:
					return drawable::camera_view_control_flash_off::get();
				default:
					break;
			}
			return drawable::camera_view_control_flash_auto::get();
		}
		
	};
	
	void CameraView::setControlsVisible(sl_bool flagVisible, UIUpdateMode mode)
	{
		ObjectLocker lock(this);
		m_flagControlsVisible = flagVisible;
		if (flagVisible) {
			if (m_controls.isNull()) {
				m_controls = new _priv_CameraView_Controls(this);
				addChild(m_controls, mode);
			}
		}
		if (m_controls.isNotNull()) {
			m_controls->setVisible(flagVisible, mode);
		}
	}
	
	Ref<Button> CameraView::getShutterButton()
	{
		ObjectLocker lock(this);
		_priv_CameraView_Controls* controls = (_priv_CameraView_Controls*)(m_controls.get());
		if (controls) {
			return controls->btnShutter;
		}
		return sl_null;
	}
	
	Ref<Button> CameraView::getSwitchCameraButton()
	{
		ObjectLocker lock(this);
		_priv_CameraView_Controls* controls = (_priv_CameraView_Controls*)(m_controls.get());
		if (controls) {
			return controls->btnSwitch;
		}
		return sl_null;
	}
	
	Ref<Button> CameraView::getChangeFlashModeButton()
	{
		ObjectLocker lock(this);
		_priv_CameraView_Controls* controls = (_priv_CameraView_Controls*)(m_controls.get());
		if (controls) {
			return controls->btnFlash;
		}
		return sl_null;
	}
	
	CameraFlashMode CameraView::getFlashMode()
	{
		return m_flashMode;
	}
	
	void CameraView::setFlashMode(CameraFlashMode flashMode, UIUpdateMode updateMode)
	{
		m_flashMode = flashMode;
		if (SLIB_UI_UPDATE_MODE_IS_REDRAW(updateMode)) {
			Ref<Button> button = getChangeFlashModeButton();
			if (button.isNotNull()) {
				button->setIcon(_priv_CameraView_Controls::getFlashIcon(flashMode));
			}
		}
	}
	
	sl_bool CameraView::isTouchFocusEnabled()
	{
		return m_flagTouchFocus;
	}
	
	void CameraView::setTouchFocusEnabled(sl_bool flag)
	{
		m_flagTouchFocus = flag;
	}
	
	SLIB_DEFINE_EVENT_HANDLER(CameraView, Capture, VideoCaptureFrame& frame)

	void CameraView::dispatchCapture(VideoCaptureFrame& frame)
	{
		SLIB_INVOKE_EVENT_HANDLER(Capture, frame)
		
		updateCurrentFrame(frame);
	}
	
	SLIB_DEFINE_EVENT_HANDLER(CameraView, TakePicture, CameraTakePictureResult& result)
	
	void CameraView::dispatchTakePicture(CameraTakePictureResult& result)
	{
		SLIB_INVOKE_EVENT_HANDLER(TakePicture, result)
	}
	
	void CameraView::onAttach()
	{
		if (m_flagAutoStart) {
			start();
		}
	}
	
	void CameraView::onDraw(Canvas* canvas)
	{
		VideoView::onDraw(canvas);
		if (m_flagDuringTouchFocusEffect) {
			do {
				sl_real f = (sl_real)((Time::now() - m_timeTouchFocusBegan).getSecondsCountf());
				if (f > 1.2f) {
					m_flagDuringTouchFocusEffect = sl_false;
					break;
				}
				sl_real sizeFocus = UIResource::dpToPixel(50);
				Color colorFocus = Color::Lime;
				if (f < 0.2f) {
					sizeFocus *= (1 + (0.2f - f) * 5);
				} else {
					int t = ((int)((f - 0.2f) / 0.1f)) % 2;
					if (t) {
						colorFocus = Color::Green;
					}
				}
				Ref<Pen> pen = Pen::createSolidPen(UIResource::dpToPixel(1), colorFocus);
				sl_real x = (sl_real)(m_pointTouchFocus.x);
				sl_real y = (sl_real)(m_pointTouchFocus.y);
				sl_real x1 = x - sizeFocus / 2;
				sl_real x2 = x + sizeFocus / 2;
				sl_real y1 = y - sizeFocus / 2;
				sl_real y2 = y + sizeFocus / 2;
				canvas->drawLine(x1, y1, x2, y1, pen);
				canvas->drawLine(x2, y1, x2, y2, pen);
				canvas->drawLine(x2, y2, x1, y2, pen);
				canvas->drawLine(x1, y2, x1, y1, pen);
				sl_real t = sizeFocus / 8;
				canvas->drawLine(x, y1, x, y1 + t, pen);
				canvas->drawLine(x2 - t, y, x2, y, pen);
				canvas->drawLine(x, y2 - t, x, y2, pen);
				canvas->drawLine(x1, y, x1 + t, y, pen);
			} while (0);
		}
	}
	
	void CameraView::onClickEvent(UIEvent* ev)
	{
		if (m_flagTouchFocus) {
			Point pt = ev->getPoint();
			Ref<View> child = getTopmostViewAt(pt);			
			if (child.isNotNull() && child != m_controls) {
				return;
			}
			ObjectLocker lock(this);
			if (m_camera.isNull()) {
				return;
			}
			if (!(m_camera->isRunning())) {
				return;
			}
			if (m_flagDuringTouchFocusEffect) {
				return;
			}
			m_flagDuringTouchFocusEffect = sl_true;
			m_timeTouchFocusBegan = Time::now();
			m_pointTouchFocus = pt;
			if (convertCoordinateToTexture(pt)) {
				m_camera->autoFocusOnPoint(pt.x, pt.y);
			}
		}
	}
	
	void CameraView::onClickShutter()
	{
		ObjectLocker lock(this);
		if (m_camera.isNull()) {
			return;
		}
		Ref<Button> button = getShutterButton();
		if (button.isNotNull()) {
			button->setEnabled(sl_false);
		}
		CameraTakePictureParam param;
		param.flashMode = m_flashMode;
		param.onComplete = SLIB_FUNCTION_WEAKREF(CameraView, _onTakePicture, this);
		m_camera->takePicture(param);
	}
	
	void CameraView::_onCaptureCameraFrame(VideoCapture* capture, VideoCaptureFrame& frame)
	{
		dispatchCapture(frame);
	}
	
	void CameraView::_onTakePicture(CameraTakePictureResult& result)
	{
		dispatchTakePicture(result);
		Ref<Button> button = getShutterButton();
		if (button.isNotNull()) {
			button->setEnabled(sl_true);
		}
	}
	
}
