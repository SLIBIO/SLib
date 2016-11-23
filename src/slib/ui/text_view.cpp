#include "../../../inc/slib/ui/text_view.h"

SLIB_UI_NAMESPACE_BEGIN

SDM_Item::SDM_Item(SDM_ItemType type)
: m_type(type)
{
}

SDM_ItemType SDM_Item::getType()
{
	return m_type;
}

Ref<SDM_Frame> SDM_Item::getFrame()
{
	return m_frame;
}

sl_bool SDM_Item::isNewLine()
{
	return m_flagNewLine;
}

void SDM_Item::setNewLine(sl_bool flag, UIUpdateMode mode)
{
	m_flagNewLine = flag;
	_relayout(mode);
}

void SDM_Item::_relayout(UIUpdateMode mode)
{
	if (mode != UIUpdateMode::Init) {
		Ref<SDM_Frame> frame = m_frame;
		if (frame.isNotNull()) {
			frame->_relayout(this, mode);
		}
	}
}

void SDM_Item::_redraw(UIUpdateMode mode)
{
	if (mode == UIUpdateMode::Redraw) {
		Ref<SDM_Frame> frame = m_frame;
		if (frame.isNotNull()) {
			frame->_redraw(this);
		}
	}
}


SDM_Text::SDM_Text() : SDM_Item(SDM_ItemType::Text), m_textColor(Color::Black), m_backgroundColor(Color::Zero)
{
}

Ref<SDM_Text> SDM_Text::create(const Ref<Font>& font)
{
	if (font.isNotNull()) {
		Ref<SDM_Text> ret = new SDM_Text;
		if (ret.isNotNull()) {
			ret->m_font = font;
			return ret;
		}
	}
	return Ref<SDM_Text>::null();
}

String SDM_Text::getText()
{
	return m_text;
}

void SDM_Text::setText(const String& text, UIUpdateMode mode)
{
	m_text = text;
	_relayout(mode);
}

Ref<Font> SDM_Text::getFont()
{
	return m_font;
}

void SDM_Text::setFont(const Ref<Font>& font, UIUpdateMode mode)
{
	if (font.isNotNull()) {
		m_font = font;
		_relayout(mode);
	}
}

Color SDM_Text::getTextColor()
{
	return m_textColor;
}

void SDM_Text::setTextColor(const Color& color, UIUpdateMode mode)
{
	m_textColor = color;
	_redraw(mode);
}

Color SDM_Text::getBackgroundColor()
{
	return m_backgroundColor;
}

void SDM_Text::setBackgroundColor(const Color& color, UIUpdateMode mode)
{
	m_backgroundColor = color;
	_redraw(mode);
}


SDM_Image::SDM_Image() : SDM_Item(SDM_ItemType::Image), m_scaleMode(ScaleMode::Stretch), m_gravity(Alignment::MiddleCenter), m_width(0), m_height(0)
{
}

Ref<SDM_Image> SDM_Image::create()
{
	return new SDM_Image;
}

Ref<Drawable> SDM_Image::getSource()
{
	return m_source;
}

void SDM_Image::setSource(const Ref<Drawable>& source, UIUpdateMode mode)
{
	m_source = source;
	if (m_width == 0 || m_height == 0) {
		_relayout(mode);
	} else {
		_redraw(mode);
	}
}

ScaleMode SDM_Image::getScaleMode()
{
	return m_scaleMode;
}

void SDM_Image::setScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode)
{
	m_scaleMode = scaleMode;
	_redraw(updateMode);
}

Alignment SDM_Image::getGravity()
{
	return m_gravity;
}

void SDM_Image::setGravity(Alignment gravity, UIUpdateMode mode)
{
	m_gravity = gravity;
	_redraw(mode);
}

sl_ui_len SDM_Image::getWidth()
{
	return m_width;
}

void SDM_Image::setWidth(sl_ui_len width, UIUpdateMode mode)
{
	m_width = width;
	_relayout(mode);
}

sl_ui_len SDM_Image::getHeight()
{
	return m_height;
}

void SDM_Image::setHeight(sl_ui_len height, UIUpdateMode mode)
{
	m_height = height;
	_relayout(mode);
}


SDM_View::SDM_View() : SDM_Item(SDM_ItemType::View)
{
}


SDM_Frame::SDM_Frame() : SDM_Item(SDM_ItemType::Frame)
{
}

void SDM_Frame::_relayout(SDM_Item* item, UIUpdateMode mode)
{
	
}

void SDM_Frame::_redraw(SDM_Item* item)
{
	
}


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
