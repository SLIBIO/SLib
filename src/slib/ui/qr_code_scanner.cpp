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

#include "slib/ui/qr_code_scanner.h"

#include "slib/render/canvas.h"
#include "slib/graphics/zxing.h"
#include "slib/graphics/yuv.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(QRCodeScanner, CameraView)
	
	class _priv_QRCodeScanner_ScanBar_Program : public RenderProgram2D_Position
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override
		{
			String source = SLIB_STRINGIFY(
										   uniform mat3 u_Transform;
										   attribute vec2 a_Position;
										   varying vec2 v_Position;
										   void main() {
											   vec3 P = vec3(a_Position.x, a_Position.y, 1.0) * u_Transform;
											   gl_Position = vec4(P.x, P.y, 0.0, 1.0);
											   v_Position = a_Position;
										   }
										   );
			return source;
		}

		String getGLSLFragmentShader(RenderEngine* engine) override
		{
			String source = SLIB_STRINGIFY(
										   uniform vec4 u_Color;
										   varying vec2 v_Position;
										   void main() {
											   float a = 1.0 - (abs(0.5 - v_Position.y) * 2.0);
											   float c = 1.0 - (abs(0.5 - v_Position.x) * 2.0);
											   float b = pow(c, 0.2);
											   gl_FragColor = u_Color*a*b;
										   }
										   );
			return source;
		}
	};
	
	QRCodeScanner::QRCodeScanner()
	{
		m_flagUpdateCameraFrame = sl_false;
		setScaleMode(ScaleMode::Cover);
		m_programScanBar = new _priv_QRCodeScanner_ScanBar_Program;
	}
	
	QRCodeScanner::~QRCodeScanner()
	{
	}

	void QRCodeScanner::start(const CameraParam& param)
	{
		CameraView::start(param);
		m_timerScanner = Timer::start(SLIB_FUNCTION_WEAKREF(QRCodeScanner, onRunScanner, this), 500);
	}
	
	void QRCodeScanner::stop()
	{
		m_timerScanner.setNull();
		CameraView::stop();
	}
	
	SLIB_DEFINE_EVENT_HANDLER(QRCodeScanner, Detect, const String& code)

	void QRCodeScanner::dispatchDetect(const String& code)
	{
		SLIB_INVOKE_EVENT_HANDLER(Detect, code)
	}
	
	void QRCodeScanner::onDraw(Canvas* _canvas)
	{
		CameraView::onDraw(_canvas);
		RenderCanvas* canvas = CastInstance<RenderCanvas>(_canvas);
		if (!canvas) {
			return;
		}
		Ref<RenderEngine> engine = canvas->getEngine();
		if (engine.isNull()) {
			return;
		}
		Rectangle rectBounds = getBounds();
		sl_real width = rectBounds.getWidth();
		if (width < SLIB_EPSILON) {
			return;
		}
		sl_real height = rectBounds.getHeight();
		if (height < SLIB_EPSILON) {
			return;
		}
		sl_real width_half = Math::floor(width / 2);
		sl_real height_half = Math::floor(height / 2);
		sl_real box_size;
		if (width > height) {
			box_size = height;
		} else {
			box_size = width;
		}
		box_size = (sl_real)((int)(box_size * 0.65f) >> 1 << 1);
		sl_real box_half = Math::floor(box_size / 2);
		
		sl_real scanner_height = Math::floor(box_size / 20);
		if (scanner_height < 5) {
			scanner_height = 5;
		}
		Time time = Time::now();
		float ftime = (float)(time.getMillisecondsCount() % 4000) / 2000;
		sl_real scanner_y;
		if (ftime < 1) {
			ftime = (Math::cos((ftime + 1) * SLIB_PI) / 2) + 0.5f;
			scanner_y = (box_size - scanner_height) * ftime;
		} else {
			ftime -= 1;
			ftime = (Math::cos((ftime + 1) * SLIB_PI) / 2) + 0.5f;
			scanner_y = (box_size - scanner_height) * (1 - ftime);
		}
		Matrix3 mat;
		mat.m00 = box_size; mat.m10 = 0; mat.m20 = width_half - box_half;
		mat.m01 = 0; mat.m11 = scanner_height; mat.m21 = height_half - box_half + scanner_y;
		mat.m02 = 0; mat.m12 = 0; mat.m22 = 1;
		mat *= Matrix3(2/width, 0, 0, 0, -2/height, 0, -1, 1, 1);
		engine->drawRectangle2D(m_programScanBar, mat, Color4f(0, 1, 0, 0.5f));
		
		Color colorFillDark(0, 0, 0, 150);
		canvas->fillRectangle(0, 0, width, height_half - box_half, colorFillDark);
		canvas->fillRectangle(0, height_half - box_half, width_half - box_half, box_size, colorFillDark);
		canvas->fillRectangle(width_half + box_half, height_half - box_half, width_half - box_half, box_size, colorFillDark);
		canvas->fillRectangle(0, height_half + box_half, width, height_half - box_half, colorFillDark);
		Color colorFillCorner(0, 255, 0);
		sl_real corner_depth = Math::floor(box_size / 50);
		if (corner_depth < 2) {
			corner_depth = 2;
		}
		sl_real corner_length = Math::floor(box_size / 10);
		if (corner_length < 2) {
			corner_length = 2;
		}
		canvas->fillRectangle(width_half - box_half, height_half - box_half, corner_length, corner_depth, colorFillCorner);
		canvas->fillRectangle(width_half - box_half, height_half - box_half, corner_depth, corner_length, colorFillCorner);
		canvas->fillRectangle(width_half + box_half - corner_length, height_half - box_half, corner_length, corner_depth, colorFillCorner);
		canvas->fillRectangle(width_half + box_half - corner_depth, height_half - box_half, corner_depth, corner_length, colorFillCorner);
		canvas->fillRectangle(width_half - box_half, height_half + box_half - corner_depth, corner_length, corner_depth, colorFillCorner);
		canvas->fillRectangle(width_half - box_half, height_half + box_half - corner_length, corner_depth, corner_length, colorFillCorner);
		canvas->fillRectangle(width_half + box_half - corner_length, height_half + box_half - corner_depth, corner_length, corner_depth, colorFillCorner);
		canvas->fillRectangle(width_half + box_half - corner_depth, height_half + box_half - corner_length, corner_depth, corner_length, colorFillCorner);
	}

	void QRCodeScanner::onCapture(VideoCaptureFrame& frame)
	{
		m_flagUpdateCameraFrame = sl_true;
	}
	
	void QRCodeScanner::onRunScanner(Timer* timer)
	{
		if (!m_flagUpdateCameraFrame) {
			return;
		}
		m_flagUpdateCameraFrame = sl_false;
		Ref<Texture> texture = m_textureFrame;
		if (texture.isNull()) {
			return;
		}
		Ref<Bitmap> bitmap = texture->getSource();
		if (bitmap.isNull()) {
			return;
		}
		if (!(bitmap->isImage())) {
			return;
		}
		Ref<Image> image = Ref<Image>::from(bitmap);
		if (m_flagYUV) {
			image = Image::createCopy(image);
			if (image.isNull()) {
				return;
			}
			sl_uint32 width = image->getWidth();
			sl_uint32 height = image->getHeight();
			for (sl_uint32 y = 0; y < height; y++) {
				Color* colors = image->getColorsAt(0, y);
				for (sl_uint32 x = 0; x < width; x++) {
					sl_uint8 r, g, b;
					YUV::convertYUVToRGB(colors->r, colors->g, colors->b, r, g, b);
					colors->r = r;
					colors->g = g;
					colors->b = b;
					colors++;
				}
			}
		}
		ZXingScanParam param;
		param.format = ZXingFormat::QR_CODE;
		param.image = image;
		param.flagTryHarder = sl_true;
		param.flagTryRotate = sl_true;
		String text = ZXing::scan(param);
		if (text.isNotNull()) {
			dispatchDetect(text);
		}
	}
	
}
