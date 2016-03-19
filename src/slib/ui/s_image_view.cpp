#include "../../../inc/slib/ui/s_image_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(SImageView, SView)

SImageView::SImageView()
{
	m_scaleMode = ScaleMode::Stretch;
	m_sourceAlignment = Alignment::MiddleCenter;
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
