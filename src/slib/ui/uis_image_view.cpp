#include "../../../inc/slib/ui/s_image_view.h"

SLIB_UI_NAMESPACE_BEGIN
SImageView::SImageView()
{
	m_scaleType = scaleTypeStretch;
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

ScaleType SImageView::getScaleType()
{
	return m_scaleType;
}

void SImageView::setScaleType(ScaleType type)
{
	m_scaleType = type;
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
	canvas->draw(getContentBounds(), m_source, m_scaleType, m_sourceAlignment);
}
SLIB_UI_NAMESPACE_END
