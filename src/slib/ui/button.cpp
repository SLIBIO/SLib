#include "../../../inc/slib/ui/button.h"

#include "../../../inc/slib/graphics/context.h"
#include "../../../inc/slib/graphics/util.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Button, View)

ButtonCategoryProperties::ButtonCategoryProperties()
{
	textColor = Color::zero();
	backgroundColor = Color::zero();
}

class _Button_Categories
{
public:
	ButtonCategory categories[2];
	
public:
	_Button_Categories()
	{
		categories[0].properties[(int)(ButtonState::Normal)].border = Pen::create(PenStyle::Solid, 1, Color(100, 100, 100));
		categories[0].properties[(int)(ButtonState::Normal)].backgroundColor = Color(240, 240, 240);
		categories[0].properties[(int)(ButtonState::Normal)].textColor = Color::Black;
		categories[0].properties[(int)(ButtonState::Disabled)].backgroundColor = Color(220, 220, 220);
		categories[0].properties[(int)(ButtonState::Disabled)].textColor = Color(110, 110, 110);
		categories[0].properties[(int)(ButtonState::Hover)].border = Pen::create(PenStyle::Solid, 1, Color(0, 100, 250));
		categories[0].properties[(int)(ButtonState::Hover)].backgroundColor = Color(220, 230, 255);
		categories[0].properties[(int)(ButtonState::Down)].border = categories[0].properties[(int)(ButtonState::Hover)].border;
		categories[0].properties[(int)(ButtonState::Down)].backgroundColor = Color(180, 210, 255);
		
		categories[1] = categories[0];
		categories[1].properties[(int)(ButtonState::Normal)].border = Pen::create(PenStyle::Solid, 3, Color(0, 100, 250));
	}
	
public:
	static _Button_Categories* get()
	{
		SLIB_SAFE_STATIC(_Button_Categories, ret);
		return &ret;
	}
};

Button::Button() : Button(2)
{
	setCreatingNativeWidget(sl_true);
}

Button::Button(sl_uint32 nCategories, ButtonCategory* categories)
{
	m_flagDefaultButton = sl_false;
	
	View::setBackgroundColor(Color::Transparent, sl_false);
	View::setBorderPen(Pen::getDefault(), sl_false);
	
	m_state = ButtonState::Normal;
	m_category = 0;
	
	m_iconSize.x = 0;
	m_iconSize.y = 0;
	m_contentAlignment = Alignment::MiddleCenter;
	m_iconAlignment = Alignment::MiddleCenter;
	m_textAlignment = Alignment::MiddleCenter;
	m_flagTextBeforeIcon = sl_false;
	m_layoutOrientation = LayoutOrientation::Horizontal;
	
	m_iconMarginLeft = 0;
	m_iconMarginTop = 0;
	m_iconMarginRight = 0;
	m_iconMarginBottom = 0;
	
	m_textMarginLeft = 2;
	m_textMarginTop = 2;
	m_textMarginRight = 2;
	m_textMarginBottom = 2;
	
	if (nCategories == 0) {
		nCategories = 1;
	}
	m_nCategories = nCategories;
	m_categories = New<ButtonCategory>::create(nCategories);
	if (!categories) {
		categories = _Button_Categories::get()->categories;
		if (nCategories > 2) {
			nCategories = 2;
		}
	}
	for (sl_uint32 i = 0; i < nCategories; i++) {
		m_categories[i] = categories[i];
	}
	setOccurringClick(sl_true);
}

Button::~Button()
{
	New<ButtonCategory>::free(m_categories, m_nCategories);
}

String Button::getText()
{
	return m_text;
}

void Button::setText(const String& text, sl_bool flagRedraw)
{
	m_text = text;
	if (isNativeWidget()) {
		_setText_NW(text);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool Button::isDefaultButton()
{
	return m_flagDefaultButton;
}

void Button::setDefaultButton(sl_bool flag, sl_bool flagRedraw)
{
	m_flagDefaultButton = flag;
	if (flag) {
		setCurrentCategory(1);
	} else {
		setCurrentCategory(0);
	}
	if (isNativeWidget()) {
		_setDefaultButton_NW(flag);
	} else {
		if (flagRedraw) {
			invalidate();
		}		
	}
}

sl_uint32 Button::getCategoriesCount()
{
	return m_nCategories;
}

ButtonState Button::getButtonState()
{
	return m_state;
}

sl_uint32 Button::getCurrentCategory()
{
	return m_category;
}

void Button::setCurrentCategory(sl_uint32 n, sl_bool flagRedraw)
{
	if (n < m_nCategories) {
		m_category = n;
		if (flagRedraw) {
			invalidate();
		}
	}
}

const Size& Button::getIconSize()
{
	return m_iconSize;
}

void Button::setIconSize(const Size& size, sl_bool flagRedraw)
{
	m_iconSize = size;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::setIconSize(sl_real width, sl_real height, sl_bool flagRedraw)
{
	setIconSize(Size(width, height), flagRedraw);
}

void Button::setIconSize(sl_real size, sl_bool flagRedraw)
{
	setIconSize(Size(size, size),flagRedraw);
}

Alignment Button::getContentAlignment()
{
	return m_contentAlignment;
}

void Button::setContentAlignment(Alignment align, sl_bool flagRedraw)
{
	m_contentAlignment = align;
	if (flagRedraw) {
		invalidate();
	}
}

Alignment Button::getIconAlignment()
{
	return m_iconAlignment;
}

void Button::setIconAlignment(Alignment align, sl_bool flagRedraw)
{
	m_iconAlignment = align;
	if (flagRedraw) {
		invalidate();
	}
}

Alignment Button::getTextAlignment()
{
	return m_textAlignment;
}

void Button::setTextAlignment(Alignment align, sl_bool flagRedraw)
{
	m_textAlignment = align;
	if (flagRedraw) {
		invalidate();
	}
}

sl_bool Button::isTextBeforeIcon()
{
	return m_flagTextBeforeIcon;
}

void Button::setTextBeforeIcon(sl_bool flag, sl_bool flagRedraw)
{
	m_flagTextBeforeIcon = flag;
	if (flagRedraw) {
		invalidate();
	}
}

LayoutOrientation Button::getLayoutOrientation()
{
	return m_layoutOrientation;
}

void Button::setLayoutOrientation(LayoutOrientation orientation, sl_bool flagRedraw)
{
	m_layoutOrientation = orientation;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::setIconMargin(sl_real left, sl_real top, sl_real right, sl_real bottom, sl_bool flagRedraw)
{
	m_iconMarginLeft = left;
	m_iconMarginTop = top;
	m_iconMarginRight = right;
	m_iconMarginBottom = bottom;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::setIconMargin(sl_real margin, sl_bool flagRedraw)
{
	setIconMargin(margin, margin, margin, margin, flagRedraw);
}

sl_real Button::getIconMarginLeft()
{
	return m_iconMarginLeft;
}

void Button::setIconMarginLeft(sl_real margin, sl_bool flagRedraw)
{
	setIconMargin(margin, m_iconMarginTop, m_iconMarginRight, m_iconMarginBottom, flagRedraw);
}

sl_real Button::getIconMarginTop()
{
	return m_iconMarginTop;
}

void Button::setIconMarginTop(sl_real margin, sl_bool flagRedraw)
{
	setIconMargin(m_iconMarginLeft, margin, m_iconMarginRight, m_iconMarginBottom, flagRedraw);
}

sl_real Button::getIconMarginRight()
{
	return m_iconMarginRight;
}

void Button::setIconMarginRight(sl_real margin, sl_bool flagRedraw)
{
	setIconMargin(m_iconMarginLeft, m_iconMarginTop, margin, m_iconMarginBottom, flagRedraw);
}

sl_real Button::getIconMarginBottom()
{
	return m_iconMarginBottom;
}

void Button::setIconMarginBottom(sl_real margin, sl_bool flagRedraw)
{
	setIconMargin(m_iconMarginLeft, m_iconMarginTop, m_iconMarginRight, margin, flagRedraw);
}

void Button::setTextMargin(sl_real left, sl_real top, sl_real right, sl_real bottom, sl_bool flagRedraw)
{
	m_textMarginLeft = left;
	m_textMarginTop = top;
	m_textMarginRight = right;
	m_textMarginBottom = bottom;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::setTextMargin(sl_real margin, sl_bool flagRedraw)
{
	setTextMargin(margin, margin, margin, margin, flagRedraw);
}

sl_real Button::getTextMarginLeft()
{
	return m_textMarginLeft;
}

void Button::setTextMarginLeft(sl_real margin, sl_bool flagRedraw)
{
	setTextMargin(margin, m_textMarginTop, m_textMarginRight, m_textMarginBottom, flagRedraw);
}

sl_real Button::getTextMarginTop()
{
	return m_textMarginTop;
}

void Button::setTextMarginTop(sl_real margin, sl_bool flagRedraw)
{
	setTextMargin(m_textMarginLeft, margin, m_textMarginRight, m_textMarginBottom, flagRedraw);
}

sl_real Button::getTextMarginRight()
{
	return m_textMarginRight;
}

void Button::setTextMarginRight(sl_real margin, sl_bool flagRedraw)
{
	setTextMargin(m_textMarginLeft, m_textMarginTop, margin, m_textMarginBottom, flagRedraw);
}

sl_real Button::getTextMarginBottom()
{
	return m_textMarginBottom;
}

void Button::setTextMarginBottom(sl_real margin, sl_bool flagRedraw)
{
	setTextMargin(m_textMarginLeft, m_textMarginTop, m_textMarginRight, margin, flagRedraw);
}

Color Button::getTextColor(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[(int)state].textColor;
	} else {
		return Color::zero();
	}
}

void Button::setTextColor(const Color& color, ButtonState state, sl_uint32 category, sl_bool flagRedraw)
{
	if (category < m_nCategories) {
		m_categories[category].properties[(int)state].textColor = color;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Color Button::getBackgroundColor(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[(int)state].backgroundColor;
	} else {
		return Color::zero();
	}
}

void Button::setBackgroundColor(const Color& color, ButtonState state, sl_uint32 category, sl_bool flagRedraw)
{
	if (category < m_nCategories) {
		m_categories[category].properties[(int)state].backgroundColor = color;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Ref<Drawable> Button::getBackground(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[(int)state].background;
	} else {
		return Ref<Drawable>::null();
	}
}

void Button::setBackground(const Ref<Drawable>& background, ButtonState state, sl_uint32 category, sl_bool flagRedraw)
{
	if (category < m_nCategories) {
		m_categories[category].properties[(int)state].background = background;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Ref<Drawable> Button::getIcon(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[(int)state].icon;
	} else {
		return Ref<Drawable>::null();
	}
}

void Button::setIcon(const Ref<Drawable>& icon, ButtonState state, sl_uint32 category, sl_bool flagRedraw)
{
	if (category < m_nCategories) {
		m_categories[category].properties[(int)state].icon = icon;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Ref<Pen> Button::getBorder(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[(int)state].border;
	} else {
		return Ref<Pen>::null();
	}
}

void Button::setBorder(const Ref<Pen>& pen, ButtonState state, sl_uint32 category, sl_bool flagRedraw)
{
	if (category < m_nCategories) {
		m_categories[category].properties[(int)state].border = pen;
		if (flagRedraw) {
			invalidate();
		}
	}
}

Color Button::getBackgroundColor()
{
	return View::getBackgroundColor();
}

void Button::setBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	View::setBackgroundColor(color, flagRedraw);
}

Ref<Drawable> Button::getBackground()
{
	return View::getBackground();
}

void Button::setBackground(const Ref<Drawable>& background, sl_bool flagRedraw)
{
	View::setBackground(background, flagRedraw);
}

void Button::setEnabled(sl_bool flagEnabled, sl_bool flagRedraw)
{
	View::setEnabled(flagEnabled, flagRedraw);
	_invalidateButtonState();
	invalidate();
}

void Button::setDownState(sl_bool flagState, sl_bool flagRedraw)
{
	View::setDownState(flagState, flagRedraw);
	_invalidateButtonState();
	invalidate();
}

void Button::setHoverState(sl_bool flagState, sl_bool flagRedraw)
{
	View::setHoverState(flagState, flagRedraw);
	_invalidateButtonState();
	invalidate();
}

void Button::onDraw(Canvas* canvas)
{
	ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
	Color textColor = params.textColor;
	Ref<Drawable> icon = params.icon;
	if (textColor.isZero()) {
		textColor = m_categories[m_category].properties[(int)ButtonState::Normal].textColor;
	}
	if (icon.isNull()) {
		icon = m_categories[m_category].properties[(int)ButtonState::Normal].icon;
	}
	drawContent(canvas, icon, m_text, textColor);
}

void Button::onDrawBackground(Canvas* canvas)
{
	ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
	Color color = params.backgroundColor;
	Ref<Drawable> background = params.background;
	if (color.isZero()) {
		color = m_categories[m_category].properties[(int)ButtonState::Normal].backgroundColor;
		if (color.isZero()) {
			color = View::getBackgroundColor();
		}
	}
	if (background.isNull()) {
		background = m_categories[m_category].properties[(int)ButtonState::Normal].background;
		if (background.isNull()) {
			background = View::getBackground();
		}
	}
	drawBackground(canvas, color, background);
}

void Button::onDrawBorder(Canvas* canvas)
{
	ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
	Ref<Pen> pen = params.border;
	if (pen.isNull()) {
		pen = m_categories[m_category].properties[(int)ButtonState::Normal].border;
		if (pen.isNull()) {
			pen = getBorderPen();
		}
	}
	drawBorder(canvas, pen);
}

void Button::_invalidateButtonState()
{
	if (isEnabled()) {
		if (isDownState()) {
			m_state = ButtonState::Down;
		} else if (isHoverState()) {
			m_state = ButtonState::Hover;
		} else {
			m_state = ButtonState::Normal;
		}
	} else {
		m_state = ButtonState::Disabled;
	}
}

void Button::layoutIconAndText(GraphicsContext* gc, sl_real widthFrame, sl_real heightFrame, Size& sizeContent, Rectangle& frameIcon, Rectangle& frameText)
{
	sl_real widthIcon = m_iconSize.x + m_iconMarginLeft + m_iconMarginRight;
	sl_real heightIcon = m_iconSize.y + m_iconMarginTop + m_iconMarginBottom;
	
	Ref<Font> font = getFont();
	Size sizeText = gc->getFontTextSize(font, m_text);
	sl_real widthText = sizeText.x + m_textMarginLeft + m_textMarginRight;
	sl_real heightText = sizeText.y + m_textMarginTop + m_textMarginBottom;
	
	Alignment alignIcon = m_iconAlignment;
	Alignment horzIcon = alignIcon & Alignment::HorizontalMask;
	Alignment vertIcon = alignIcon & Alignment::VerticalMask;
	
	Alignment alignText = m_textAlignment;
	Alignment horzText = alignText & Alignment::HorizontalMask;
	Alignment vertText = alignText & Alignment::VerticalMask;
	
	sl_real xIcon = 0;
	sl_real yIcon = 0;
	
	sl_real xText = 0;
	sl_real yText = 0;
	
	sl_real widthContent = 0;
	sl_real heightContent = 0;
	
	if (m_layoutOrientation == LayoutOrientation::Horizontal) {
		
		if (horzIcon != horzText) {
			widthContent = widthFrame;
			if (widthContent >= 0) {
				if (m_flagTextBeforeIcon) {
					xText = GraphicsUtil::calculateAlignX(0, widthContent, widthText, alignText);
					if (horzText == Alignment::Right) {
						xIcon = GraphicsUtil::calculateAlignX(0, xText, widthIcon, alignIcon);
					} else {
						xIcon = GraphicsUtil::calculateAlignX(xText + widthText, widthContent, widthIcon, alignIcon);
					}
				} else {
					xIcon = GraphicsUtil::calculateAlignX(0, widthContent, widthIcon, alignIcon);
					if (horzIcon == Alignment::Right) {
						xText = GraphicsUtil::calculateAlignX(0, xIcon, widthText, alignText);
					} else {
						xText = GraphicsUtil::calculateAlignX(xIcon + widthIcon, widthContent, widthText, alignText);
					}
				}
			}
		} else {
			widthContent = widthIcon + widthText;
			if (m_flagTextBeforeIcon) {
				xText = 0;
				xIcon = widthText;
			} else {
				xIcon = 0;
				xText = widthIcon;
			}
		}
		
		if (vertIcon != vertText) {
			heightContent = heightFrame;
		} else {
			heightContent = SLIB_MAX(heightIcon, heightText);
		}
		if (heightContent >= 0) {
			yIcon = GraphicsUtil::calculateAlignY(0, heightContent, heightIcon, alignIcon);
			yText = GraphicsUtil::calculateAlignY(0, heightContent, heightText, alignText);
		}
		
	} else {
		if (horzIcon != horzText) {
			widthContent = widthFrame;
		} else {
			widthContent = SLIB_MAX(widthIcon, widthText);
		}
		if (widthContent >= 0) {
			xIcon = GraphicsUtil::calculateAlignX(0, widthContent, widthIcon, alignIcon);
			xText = GraphicsUtil::calculateAlignX(0, widthContent, widthText, alignText);
		}
		
		if (vertIcon != vertText) {
			heightContent = heightFrame;
			if (heightContent >= 0) {
				if (m_flagTextBeforeIcon) {
					yText = GraphicsUtil::calculateAlignY(0, heightContent, heightText, alignText);
					if (vertText == Alignment::Bottom) {
						yIcon = GraphicsUtil::calculateAlignY(0, yText, heightIcon, alignIcon);
					} else {
						yIcon = GraphicsUtil::calculateAlignY(yText + heightText, heightContent, heightIcon, alignIcon);
					}
				} else {
					yIcon = GraphicsUtil::calculateAlignY(0, heightContent, heightIcon, alignIcon);
					if (vertIcon == Alignment::Bottom) {
						yText = GraphicsUtil::calculateAlignY(0, yIcon, heightText, alignText);
					} else {
						yText = GraphicsUtil::calculateAlignY(yIcon + heightIcon, heightContent, heightText, alignText);
					}
				}
			}
		} else {
			heightContent = heightIcon + heightText;
			if (m_flagTextBeforeIcon) {
				yText = 0;
				yIcon = heightText;
			} else {
				yIcon = 0;
				yText = heightIcon;
			}
		}
		
	}
	
	sizeContent.x = widthContent;
	sizeContent.y = heightContent;
	
	if (widthContent >= 0 && heightContent >= 0) {
		frameIcon.left = xIcon + m_iconMarginLeft;
		frameIcon.top = yIcon + m_iconMarginTop;
		frameIcon.right = xIcon + widthIcon - m_iconMarginRight;
		frameIcon.bottom = yIcon + heightIcon - m_iconMarginBottom;
		
		frameText.left = xText + m_textMarginLeft;
		frameText.top = yText + m_textMarginTop;
		frameText.right = xText + widthText - m_textMarginRight;
		frameText.bottom = yText + heightText - m_textMarginBottom;
	}
}

Size Button::measureContentSize(GraphicsContext* gc)
{
	Size size;
	Rectangle rcIcon, rcText;
	layoutIconAndText(gc, -1, -1, size, rcIcon, rcText);
	return size;
}

void Button::drawContent(Canvas* canvas, const Ref<Drawable>& icon, const String& text, const Color& textColor)
{
	Ref<GraphicsContext> gc = canvas->getGraphicsContext();
	if (gc.isNull()) {
		return;
	}
	Rectangle bound = getBoundsInnerPadding();
	if (bound.getWidth() <= 0 || bound.getHeight() <= 0) {
		return;
	}
	Size sizeContent;
	Rectangle rcIcon, rcText;
	layoutIconAndText(gc.ptr, bound.getWidth(), bound.getHeight(), sizeContent, rcIcon, rcText);
	Point pt = GraphicsUtil::calculateAlignPosition(bound, sizeContent.x, sizeContent.y, m_contentAlignment);
	rcIcon.left += pt.x;
	rcIcon.top += pt.y;
	rcIcon.right += pt.x;
	rcIcon.bottom += pt.y;
	rcText.left += pt.x;
	rcText.top += pt.y;
	rcText.right += pt.x;
	rcText.bottom += pt.y;
	if (icon.isNotNull()) {
		canvas->draw(rcIcon, icon);
	}
	if (text.isNotEmpty()) {
		canvas->drawText(text, rcText.left, rcText.top, getFont(), textColor);
	}
}

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> Button::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void Button::_setText_NW(const String& text)
{
}

void Button::_setFont_NW(const Ref<Font>& font)
{
}

void Button::_setDefaultButton_NW(sl_bool flag)
{
}

#endif

SLIB_UI_NAMESPACE_END
