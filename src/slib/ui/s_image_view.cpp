#include "../../../inc/slib/ui/s_image_view.h"

SLIB_UI_NAMESPACE_BEGIN

SImageView::SImageView()
{
	m_scaleMode = scaleMode_Stretch;
	m_sourceAlignment = alignMiddleCenter;
}

Ref<Drawable> SImageView::getSource()
{
	return m_source;
}

void SImageView::setSource(const Ref<Drawable>& source)
{
	m_source = source;
	invalidate();
}

ScaleMode SImageView::getScaleMode()
{
	return m_scaleMode;
}

void SImageView::setScaleMode(ScaleMode mode)
{
	m_scaleMode = mode;
	invalidate();
}

Alignment SImageView::getSourceAlignment()
{
	return m_sourceAlignment;
}

void SImageView::setSourceAlignment(Alignment align)
{
	m_sourceAlignment = align;
	invalidate();
}

void SImageView::onDraw(Canvas* canvas)
{
	canvas->draw(getContentBounds(), m_source, m_scaleMode, m_sourceAlignment);
}

SLIB_UI_NAMESPACE_END
