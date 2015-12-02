#include "../../../inc/slib/ui/label_view.h"

SLIB_UI_NAMESPACE_BEGIN
LabelView::LabelView()
{
	m_textAlignment = alignLeft;
	m_flagBorder = sl_false;
	m_textColor = Color::black();
	m_backgroundColor = Color::transparent();
}

void LabelView::onClick()
{
	Ref<Runnable> callback = getOnClick();
	if (callback.isNotNull()) {
		callback->run();
	}
}

Ref<Font> LabelView::getFont()
{
	return m_font;
}
SLIB_UI_NAMESPACE_END
