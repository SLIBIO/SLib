#include "../../../inc/slib/ui/s_button.h"
#include "../../../inc/slib/graphics/context.h"
#include "../../../inc/slib/graphics/util.h"

SLIB_UI_NAMESPACE_BEGIN
SButtonCategoryProperties::SButtonCategoryProperties()
{
	textColor = Color::zero();
	backgroundColor = Color::zero();
}

class _SButton_Categories
{
public:
	SButtonCategory categories[2];
	
	_SButton_Categories()
	{
		categories[0].properties[buttonState_Normal].border = Pen::create(penStyle_Solid, 1, Color::gray(100));
		categories[0].properties[buttonState_Normal].backgroundColor = Color::gray(240);
		categories[0].properties[buttonState_Normal].textColor = Color::black();
		categories[0].properties[buttonState_Disabled].backgroundColor = Color::gray(220);
		categories[0].properties[buttonState_Disabled].textColor = Color::gray(110);
		categories[0].properties[buttonState_Hover].border = Pen::create(penStyle_Solid, 1, Color(0, 100, 250));
		categories[0].properties[buttonState_Hover].backgroundColor = Color(220, 230, 255);
		categories[0].properties[buttonState_Down].border = categories[0].properties[buttonState_Hover].border;
		categories[0].properties[buttonState_Down].backgroundColor = Color(180, 210, 255);
		
		categories[1] = categories[0];
		categories[1].properties[buttonState_Normal].border = Pen::create(penStyle_Solid, 3, Color(0, 100, 250));
	}
	
	static _SButton_Categories* get()
	{
		SLIB_SAFE_STATIC(_SButton_Categories, ret);
		return &ret;
	}
};

SButton::SButton(sl_uint32 nCategories, SButtonCategory* categories)
{
	m_flagDefaultButton = sl_false;
	
	m_state = buttonState_Normal;
	m_category = 0;
	
	m_iconSize.x = 0;
	m_iconSize.y = 0;
	m_contentAlignment = alignMiddleCenter;
	m_iconAlignment = alignMiddleCenter;
	m_textAlignment = alignMiddleCenter;
	m_flagTextBeforeIcon = sl_false;
	m_layoutOrientation = layoutOrientation_Horizontal;
	
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
	m_categories = new SButtonCategory[nCategories];
	if (!categories) {
		categories = _SButton_Categories::get()->categories;
		if (nCategories > 2) {
			nCategories = 2;
		}
	}
	for (sl_uint32 i = 0; i < nCategories; i++) {
		m_categories[i] = categories[i];
	}
	setClickEnabled(sl_true);
}

SButton::~SButton()
{
	delete[] m_categories;
}

String SButton::getText()
{
	return m_text;
}

void SButton::setText(const String& text)
{
	m_text = text;
	invalidate();
}

Ref<Font> SButton::getFont()
{
	return m_font;
}

void SButton::setFont(const Ref<Font>& font)
{
	m_font = font;
	invalidate();
}

sl_bool SButton::isDefaultButton()
{
	return m_flagDefaultButton;
}

void SButton::setDefaultButton(sl_bool flag)
{
	m_flagDefaultButton = flag;
	if (flag) {
		setCurrentCategory(1);
	} else {
		setCurrentCategory(0);
	}
	invalidate();
}

sl_uint32 SButton::getCategoriesCount()
{
	return m_nCategories;
}

ButtonState SButton::getButtonState()
{
	return m_state;
}

sl_uint32 SButton::getCurrentCategory()
{
	return m_category;
}

void SButton::setCurrentCategory(sl_uint32 n)
{
	if (n < m_nCategories) {
		m_category = n;
		invalidate();
	}
}

const Size& SButton::getIconSize()
{
	return m_iconSize;
}

void SButton::setIconSize(const Size& size)
{
	m_iconSize = size;
	invalidate();
}

void SButton::setIconSize(sl_real width, sl_real height)
{
	setIconSize(Size(width, height));
}

void SButton::setIconSize(sl_real size)
{
	setIconSize(Size(size, size));
}

Alignment SButton::getContentAlignment()
{
	return m_contentAlignment;
}

void SButton::setContentAlignment(Alignment align)
{
	m_contentAlignment = align;
	invalidate();
}

Alignment SButton::getIconAlignment()
{
	return m_iconAlignment;
}

void SButton::setIconAlignment(Alignment align)
{
	m_iconAlignment = align;
	invalidate();
}

Alignment SButton::getTextAlignment()
{
	return m_textAlignment;
}

void SButton::setTextAlignment(Alignment align)
{
	m_textAlignment = align;
	invalidate();
}

sl_bool SButton::isTextBeforeIcon()
{
	return m_flagTextBeforeIcon;
}

void SButton::setTextBeforeIcon(sl_bool flag)
{
	m_flagTextBeforeIcon = flag;
	invalidate();
}

LayoutOrientation SButton::getLayoutOrientation()
{
	return m_layoutOrientation;
}

void SButton::setLayoutOrientation(LayoutOrientation orientation)
{
	m_layoutOrientation = orientation;
	invalidate();
}

void SButton::setIconMargin(sl_real left, sl_real top, sl_real right, sl_real bottom)
{
	m_iconMarginLeft = left;
	m_iconMarginTop = top;
	m_iconMarginRight = right;
	m_iconMarginBottom = bottom;
	invalidate();
}

void SButton::setIconMargin(sl_real margin)
{
	setIconMargin(margin, margin, margin, margin);
}

sl_real SButton::getIconMarginLeft()
{
	return m_iconMarginLeft;
}

void SButton::setIconMarginLeft(sl_real margin)
{
	setIconMargin(margin, m_iconMarginTop, m_iconMarginRight, m_iconMarginBottom);
}

sl_real SButton::getIconMarginTop()
{
	return m_iconMarginTop;
}

void SButton::setIconMarginTop(sl_real margin)
{
	setIconMargin(m_iconMarginLeft, margin, m_iconMarginRight, m_iconMarginBottom);
}

sl_real SButton::getIconMarginRight()
{
	return m_iconMarginRight;
}

void SButton::setIconMarginRight(sl_real margin)
{
	setIconMargin(m_iconMarginLeft, m_iconMarginTop, margin, m_iconMarginBottom);
}

sl_real SButton::getIconMarginBottom()
{
	return m_iconMarginBottom;
}

void SButton::setIconMarginBottom(sl_real margin)
{
	setIconMargin(m_iconMarginLeft, m_iconMarginTop, m_iconMarginRight, margin);
}

void SButton::setTextMargin(sl_real left, sl_real top, sl_real right, sl_real bottom)
{
	m_textMarginLeft = left;
	m_textMarginTop = top;
	m_textMarginRight = right;
	m_textMarginBottom = bottom;
	invalidate();
}

void SButton::setTextMargin(sl_real margin)
{
	setTextMargin(margin, margin, margin, margin);
}

sl_real SButton::getTextMarginLeft()
{
	return m_textMarginLeft;
}

void SButton::setTextMarginLeft(sl_real margin)
{
	setTextMargin(margin, m_textMarginTop, m_textMarginRight, m_textMarginBottom);
}

sl_real SButton::getTextMarginTop()
{
	return m_textMarginTop;
}

void SButton::setTextMarginTop(sl_real margin)
{
	setTextMargin(m_textMarginLeft, margin, m_textMarginRight, m_textMarginBottom);
}

sl_real SButton::getTextMarginRight()
{
	return m_textMarginRight;
}

void SButton::setTextMarginRight(sl_real margin)
{
	setTextMargin(m_textMarginLeft, m_textMarginTop, margin, m_textMarginBottom);
}

sl_real SButton::getTextMarginBottom()
{
	return m_textMarginBottom;
}

void SButton::setTextMarginBottom(sl_real margin)
{
	setTextMargin(m_textMarginLeft, m_textMarginTop, m_textMarginRight, margin);
}

Color SButton::getTextColor(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[state].textColor;
	} else {
		return Color::zero();
	}
}

void SButton::setTextColor(const Color& color, ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		m_categories[category].properties[state].textColor = color;
		invalidate();
	}
}

Color SButton::getBackgroundColor(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[state].backgroundColor;
	} else {
		return Color::zero();
	}
}

void SButton::setBackgroundColor(const Color& color, ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		m_categories[category].properties[state].backgroundColor = color;
		invalidate();
	}
}

Ref<Drawable> SButton::getBackground(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[state].background;
	} else {
		return Ref<Drawable>::null();
	}
}

void SButton::setBackground(const Ref<Drawable>& background, ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		m_categories[category].properties[state].background = background;
		invalidate();
	}
}

Ref<Drawable> SButton::getIcon(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[state].icon;
	} else {
		return Ref<Drawable>::null();
	}
}

void SButton::setIcon(const Ref<Drawable>& icon, ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		m_categories[category].properties[state].icon = icon;
		invalidate();
	}
}

Ref<Pen> SButton::getBorder(ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		return m_categories[category].properties[state].border;
	} else {
		return Ref<Pen>::null();
	}
}

void SButton::setBorder(const Ref<Pen>& pen, ButtonState state, sl_uint32 category)
{
	if (category < m_nCategories) {
		m_categories[category].properties[state].border = pen;
		invalidate();
	}
}

Color SButton::getBackgroundColor()
{
	return SView::getBackgroundColor();
}

void SButton::setBackgroundColor(const Color& color)
{
	SView::setBackgroundColor(color);
}

Ref<Drawable> SButton::getBackground()
{
	return SView::getBackground();
}

void SButton::setBackground(const Ref<Drawable>& background)
{
	SView::setBackground(background);
}

void SButton::setEnabled(sl_bool flag)
{
	SView::setEnabled(flag);
	_invalidateButtonState();
	invalidate();
}

void SButton::setDownState(sl_bool flag)
{
	SView::setDownState(flag);
	_invalidateButtonState();
	invalidate();
}

void SButton::setHoverState(sl_bool flag)
{
	SView::setHoverState(flag);
	_invalidateButtonState();
	invalidate();
}

void SButton::onDraw(Canvas* canvas)
{
	SButtonCategoryProperties& params = m_categories[m_category].properties[m_state];
	Color textColor = params.textColor;
	Ref<Drawable> icon = params.icon;
	if (textColor.isZero()) {
		textColor = m_categories[m_category].properties[buttonState_Normal].textColor;
	}
	if (icon.isNull()) {
		icon = m_categories[m_category].properties[buttonState_Normal].icon;
	}
	drawContent(canvas, icon, m_text, textColor);
}

void SButton::onDrawBackground(Canvas* canvas)
{
	SButtonCategoryProperties& params = m_categories[m_category].properties[m_state];
	Color color = params.backgroundColor;
	Ref<Drawable> background = params.background;
	if (color.isZero()) {
		color = m_categories[m_category].properties[buttonState_Normal].backgroundColor;
		if (color.isZero()) {
			color = SView::getBackgroundColor();
		}
	}
	if (background.isNull()) {
		background = m_categories[m_category].properties[buttonState_Normal].background;
		if (background.isNull()) {
			background = SView::getBackground();
		}
	}
	drawBackground(canvas, color, background);
}

void SButton::onDrawBorder(Canvas* canvas)
{
	SButtonCategoryProperties& params = m_categories[m_category].properties[m_state];
	Ref<Pen> pen = params.border;
	if (pen.isNull()) {
		pen = m_categories[m_category].properties[buttonState_Normal].border;
		if (pen.isNull()) {
			pen = getBorderPen();
		}
	}
	drawBorder(canvas, pen);
}

Ref<Font> SButton::_getFont()
{
	Ref<Font> ret = m_font;
	if (ret.isNull()) {
		ret = Font::getDefault();
	}
	return ret;
}

void SButton::_invalidateButtonState()
{
	if (isEnabled()) {
		if (isDownState()) {
			m_state = buttonState_Down;
		} else if (isHoverState()) {
			m_state = buttonState_Hover;
		} else {
			m_state = buttonState_Normal;
		}
	} else {
		m_state = buttonState_Disabled;
	}
}

void SButton::layoutIconAndText(GraphicsContext* gc, sl_real widthFrame, sl_real heightFrame, Size& sizeContent, Rectangle& frameIcon, Rectangle& frameText)
{
	sl_real widthIcon = m_iconSize.x + m_iconMarginLeft + m_iconMarginRight;
	sl_real heightIcon = m_iconSize.y + m_iconMarginTop + m_iconMarginBottom;
	
	Ref<Font> font = _getFont();
	Size sizeText = gc->getFontTextSize(font, m_text);
	sl_real widthText = sizeText.x + m_textMarginLeft + m_textMarginRight;
	sl_real heightText = sizeText.y + m_textMarginTop + m_textMarginBottom;
	
	Alignment alignIcon = m_iconAlignment;
	sl_uint32 horzIcon = alignIcon & alignHorizontalMask;
	sl_uint32 vertIcon = alignIcon & alignVerticalMask;
	
	Alignment alignText = m_textAlignment;
	sl_uint32 horzText = alignText & alignHorizontalMask;
	sl_uint32 vertText = alignText & alignVerticalMask;
	
	sl_real xIcon = 0;
	sl_real yIcon = 0;
	
	sl_real xText = 0;
	sl_real yText = 0;
	
	sl_real widthContent = 0;
	sl_real heightContent = 0;
	
	if (m_layoutOrientation == layoutOrientation_Horizontal) {
		
		if (horzIcon != horzText) {
			widthContent = widthFrame;
			if (widthContent >= 0) {
				if (m_flagTextBeforeIcon) {
					xText = GraphicsUtil::calculateAlignX(0, widthContent, widthText, alignText);
					if (horzText == alignRight) {
						xIcon = GraphicsUtil::calculateAlignX(0, xText, widthIcon, alignIcon);
					} else {
						xIcon = GraphicsUtil::calculateAlignX(xText + widthText, widthContent, widthIcon, alignIcon);
					}
				} else {
					xIcon = GraphicsUtil::calculateAlignX(0, widthContent, widthIcon, alignIcon);
					if (horzIcon == alignRight) {
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
					if (vertText == alignBottom) {
						yIcon = GraphicsUtil::calculateAlignY(0, yText, heightIcon, alignIcon);
					} else {
						yIcon = GraphicsUtil::calculateAlignY(yText + heightText, heightContent, heightIcon, alignIcon);
					}
				} else {
					yIcon = GraphicsUtil::calculateAlignY(0, heightContent, heightIcon, alignIcon);
					if (vertIcon == alignBottom) {
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

Size SButton::measureContentSize(GraphicsContext* gc)
{
	Size size;
	Rectangle rcIcon, rcText;
	layoutIconAndText(gc, -1, -1, size, rcIcon, rcText);
	return size;
}

void SButton::drawContent(Canvas* canvas, const Ref<Drawable>& icon, const String& text, const Color& textColor)
{
	Ref<GraphicsContext> gc = canvas->getGraphicsContext();
	if (gc.isNull()) {
		return;
	}
	Rectangle bound = getContentBounds();
	if (bound.getWidth() <= 0 || bound.getHeight() <= 0) {
		return;
	}
	Size sizeContent;
	Rectangle rcIcon, rcText;
	layoutIconAndText(gc.get(), bound.getWidth(), bound.getHeight(), sizeContent, rcIcon, rcText);
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
		canvas->drawText(text, rcText.left, rcText.top, _getFont(), textColor);
	}
}

SLIB_UI_NAMESPACE_END
