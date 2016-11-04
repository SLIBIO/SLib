#include "../../../inc/slib/ui/text_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(TextView, View)

TextView::TextView()
{
	m_flagEditable = sl_false;
	setFocusable(sl_true);
}

sl_bool TextView::isEditable()
{
	return m_flagEditable;
}

void TextView::setEditable(sl_bool flagEditable)
{
	m_flagEditable = flagEditable;
	setTextInput(flagEditable);
}

void TextView::onDraw(Canvas* canvas)
{
	canvas->fillRectangle(getBounds(), Color::White);
}

void TextView::onMouseEvent(UIEvent* ev)
{
	
	ev->stopPropagation();
	
}

void TextView::onKeyEvent(UIEvent* ev)
{
	if (m_flagEditable) {
		ev->stopPropagation();
	}
}

SLIB_UI_NAMESPACE_END
