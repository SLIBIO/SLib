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

#include "slib/ui/image_view.h"

#include "slib/core/timer.h"
#include "slib/graphics/image.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(ImageView, View)
	
	ImageView::ImageView()
	{
		SLIB_REFERABLE_CONSTRUCTOR
		
		setSavingCanvasState(sl_false);
		
		m_scaleMode = ScaleMode::Stretch;
		m_gravity = Alignment::MiddleCenter;
	}
	
	ImageView::~ImageView()
	{
	}

	Ref<ImageView> ImageView::create(sl_ui_len width, sl_ui_len height, const Ref<slib::Drawable> &drawable)
	{
		Ref<ImageView> ret = new ImageView;
		if (ret.isNotNull()) {
			ret->setSource(drawable, UIUpdateMode::Init);
			ret->setWidth(width, UIUpdateMode::Init);
			ret->setHeight(height, UIUpdateMode::Init);
			return ret;
		}
		return sl_null;
	}
	
	Ref<Drawable> ImageView::getSource()
	{
		return m_source;
	}
	
	void ImageView::setSource(const Ref<Drawable>& source, UIUpdateMode mode)
	{
		if (m_source == source) {
			return;
		}
		m_source = source;
		m_timerAnimation.setNull();
		if (source.isNotNull()) {
			sl_real h = source->getDrawableHeight();
			if (h > 0.0000001) {
				setAspectRatio(source->getDrawableWidth() / h, mode);
			}
			DrawableAnimationInfo animation;
			if (source->getAnimationInfo(&animation)) {
				float fps = animation.framesPerSecond;
				if (fps < 1) {
					fps = 1;
				}
				sl_uint32 interval = (sl_int32)(1000 / fps);
				if (interval > 60) {
					interval = 60;
				}
				m_timeStartAnimation = Time::now();
				m_timerAnimation = startTimer(SLIB_FUNCTION_WEAKREF(ImageView, onAnimationFrame, this), interval);
			}
		}
		invalidateLayoutOfWrappingControl(mode);
	}
	
	ScaleMode ImageView::getScaleMode()
	{
		return m_scaleMode;
	}
	
	void ImageView::setScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode)
	{
		m_scaleMode = scaleMode;
		invalidate(updateMode);
	}
	
	Alignment ImageView::getGravity()
	{
		return m_gravity;
	}
	
	void ImageView::setGravity(Alignment align, UIUpdateMode mode)
	{
		m_gravity = align;
		invalidate(mode);
	}
	
	void ImageView::loadUrl(const String& url)
	{
		UrlRequestParam param;
		param.url = url;
		loadUrl(param);
	}
	
	void ImageView::loadUrl(const UrlRequestParam& _param)
	{
		if (_param.url.isEmpty()) {
			setSource(sl_null);
			return;
		}
		UrlRequestParam param = _param;
		param.flagSelfAlive = sl_false;
		param.onComplete += SLIB_FUNCTION_WEAKREF(ImageView, onCompleteLoadingUrl, this);
		m_request = UrlRequest::send(param);
	}
	
	void ImageView::onDraw(Canvas* canvas)
	{
		if (m_timerAnimation.isNotNull()) {
			double time = (Time::now() - m_timeStartAnimation).getSecondsCountf();
			DrawParam param;
			param.time = (float)time;
			canvas->draw(getBoundsInnerPadding(), m_source, m_scaleMode, m_gravity, param);
		} else {
			canvas->draw(getBoundsInnerPadding(), m_source, m_scaleMode, m_gravity);
		}
	}
	
	void ImageView::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		Ref<Drawable> source = m_source;
		if (source.isNotNull()) {
			if (flagHorizontal) {
				setLayoutWidth((sl_ui_len)(source->getDrawableWidth()));
			}
			if (flagVertical) {
				setLayoutHeight((sl_ui_len)(source->getDrawableHeight()));
			}
		}
	}
	
	void ImageView::onAnimationFrame(Timer* timer)
	{
		invalidate();
	}
	
	void ImageView::onCompleteLoadingUrl(UrlRequest* request)
	{
		m_request.setNull();
		if (request->isError()) {
			return;
		}
		Memory mem = request->getResponseContent();
		setSource(Image::loadFromMemory(mem));
	}

}
