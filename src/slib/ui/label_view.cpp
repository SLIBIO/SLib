#include "../../../inc/slib/ui/label_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LabelView, View)

LabelView::LabelView()
{
	m_textAlignment = Alignment::Left;
	m_flagBorder = sl_false;
	m_textColor = Color::Black;
	m_backgroundColor = Color::Transparent;
}

void LabelView::dispatchClick()
{
	View::dispatchClick();
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
