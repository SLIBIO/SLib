#include "../../../inc/slib/ui/image_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(ImageView, View)

ImageView::ImageView()
{
	m_scaleMode = ScaleMode::Stretch;
	m_gravity = Alignment::MiddleCenter;
}

Ref<Drawable> ImageView::getSource()
{
	return m_source;
}

void ImageView::setSource(const Ref<Drawable>& source, sl_bool flagRedraw)
{
	m_source = source;
	if (flagRedraw) {
		invalidate();
	}
}

ScaleMode ImageView::getScaleMode()
{
	return m_scaleMode;
}

void ImageView::setScaleMode(ScaleMode mode, sl_bool flagRedraw)
{
	m_scaleMode = mode;
	if (flagRedraw) {
		invalidate();
	}
}

Alignment ImageView::getGravity()
{
	return m_gravity;
}

void ImageView::setGravity(Alignment align, sl_bool flagRedraw)
{
	m_gravity = align;
	if (flagRedraw) {
		invalidate();
	}
}

void ImageView::onDraw(Canvas* canvas)
{
	canvas->draw(getBoundsInnerPadding(), m_source, m_scaleMode, m_gravity);
}

SLIB_UI_NAMESPACE_END
