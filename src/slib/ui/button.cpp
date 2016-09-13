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
	static ButtonCategory* getCategories()
	{
		SLIB_SAFE_STATIC(_Button_Categories, ret)
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		return ret.categories;
	}
};

Button::Button() : Button(2)
{
	setCreatingNativeWidget(sl_true);
}

Button::Button(sl_uint32 nCategories, ButtonCategory* categories)
{
	setCreatingNativeWidget(sl_false);
	
	setAlwaysOnDrawBackground(sl_true, sl_false);
	setAlwaysOnDrawBorder(sl_true, sl_false);
	
	m_flagDefaultButton = sl_false;

	m_textColorDefault = Color::Black;
	
	m_state = ButtonState::Normal;
	m_category = 0;
	
	m_iconSize.x = 0;
	m_iconSize.y = 0;
	m_gravity = Alignment::MiddleCenter;
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
		categories = _Button_Categories::getCategories();
		if (nCategories > 2) {
			nCategories = 2;
		}
		if (!categories) {
			nCategories = 1;
		}
	}
	if (categories) {
		for (sl_uint32 i = 0; i < nCategories; i++) {
			m_categories[i] = categories[i];
		}
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

const UISize& Button::getIconSize()
{
	return m_iconSize;
}

void Button::setIconSize(const UISize& size, sl_bool flagRedraw)
{
	m_iconSize = size;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::setIconSize(sl_ui_len width, sl_ui_len height, sl_bool flagRedraw)
{
	setIconSize(UISize(width, height), flagRedraw);
}

void Button::setIconSize(sl_ui_len size, sl_bool flagRedraw)
{
	setIconSize(UISize(size, size),flagRedraw);
}

sl_ui_len Button::getIconWidth()
{
	return m_iconSize.x;
}

void Button::setIconWidth(sl_ui_len width, sl_bool flagRedraw)
{
	setIconSize(UISize(width, m_iconSize.y), flagRedraw);
}

sl_ui_len Button::getIconHeight()
{
	return m_iconSize.y;
}

void Button::setIconHeight(sl_ui_len height, sl_bool flagRedraw)
{
	setIconSize(UISize(m_iconSize.x, height), flagRedraw);
}

Alignment Button::getGravity()
{
	return m_gravity;
}

void Button::setGravity(Alignment align, sl_bool flagRedraw)
{
	m_gravity = align;
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

void Button::setIconMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw)
{
	m_iconMarginLeft = left;
	m_iconMarginTop = top;
	m_iconMarginRight = right;
	m_iconMarginBottom = bottom;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::setIconMargin(sl_ui_pos margin, sl_bool flagRedraw)
{
	setIconMargin(margin, margin, margin, margin, flagRedraw);
}

sl_ui_pos Button::getIconMarginLeft()
{
	return m_iconMarginLeft;
}

void Button::setIconMarginLeft(sl_ui_pos margin, sl_bool flagRedraw)
{
	setIconMargin(margin, m_iconMarginTop, m_iconMarginRight, m_iconMarginBottom, flagRedraw);
}

sl_ui_pos Button::getIconMarginTop()
{
	return m_iconMarginTop;
}

void Button::setIconMarginTop(sl_ui_pos margin, sl_bool flagRedraw)
{
	setIconMargin(m_iconMarginLeft, margin, m_iconMarginRight, m_iconMarginBottom, flagRedraw);
}

sl_ui_pos Button::getIconMarginRight()
{
	return m_iconMarginRight;
}

void Button::setIconMarginRight(sl_ui_pos margin, sl_bool flagRedraw)
{
	setIconMargin(m_iconMarginLeft, m_iconMarginTop, margin, m_iconMarginBottom, flagRedraw);
}

sl_ui_pos Button::getIconMarginBottom()
{
	return m_iconMarginBottom;
}

void Button::setIconMarginBottom(sl_ui_pos margin, sl_bool flagRedraw)
{
	setIconMargin(m_iconMarginLeft, m_iconMarginTop, m_iconMarginRight, margin, flagRedraw);
}

void Button::setTextMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw)
{
	m_textMarginLeft = left;
	m_textMarginTop = top;
	m_textMarginRight = right;
	m_textMarginBottom = bottom;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::setTextMargin(sl_ui_pos margin, sl_bool flagRedraw)
{
	setTextMargin(margin, margin, margin, margin, flagRedraw);
}

sl_ui_pos Button::getTextMarginLeft()
{
	return m_textMarginLeft;
}

void Button::setTextMarginLeft(sl_ui_pos margin, sl_bool flagRedraw)
{
	setTextMargin(margin, m_textMarginTop, m_textMarginRight, m_textMarginBottom, flagRedraw);
}

sl_ui_pos Button::getTextMarginTop()
{
	return m_textMarginTop;
}

void Button::setTextMarginTop(sl_ui_pos margin, sl_bool flagRedraw)
{
	setTextMargin(m_textMarginLeft, margin, m_textMarginRight, m_textMarginBottom, flagRedraw);
}

sl_ui_pos Button::getTextMarginRight()
{
	return m_textMarginRight;
}

void Button::setTextMarginRight(sl_ui_pos margin, sl_bool flagRedraw)
{
	setTextMargin(m_textMarginLeft, m_textMarginTop, margin, m_textMarginBottom, flagRedraw);
}

sl_ui_pos Button::getTextMarginBottom()
{
	return m_textMarginBottom;
}

void Button::setTextMarginBottom(sl_ui_pos margin, sl_bool flagRedraw)
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

Color Button::getTextColor()
{
	return m_textColorDefault;
}

void Button::setTextColor(const Color& color, sl_bool flagRedraw)
{
	m_textColorDefault = color;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::resetStateTextColors(sl_bool flagRedraw)
{
	for (sl_uint32 i = 0; i < m_nCategories; i++) {
		for (int k = 0; k < (int)(ButtonState::Count); k++) {
			m_categories[i].properties[k].textColor = Color::zero();
		}
	}
	if (flagRedraw) {
		invalidate();
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

Ref<Drawable> Button::getIcon()
{
	return m_iconDefault;
}

void Button::setIcon(const Ref<Drawable>& icon, sl_bool flagRedraw)
{
	m_iconDefault = icon;
	if (flagRedraw) {
		invalidate();
	}
}

void Button::resetStateIcons(sl_bool flagRedraw)
{
	for (sl_uint32 i = 0; i < m_nCategories; i++) {
		for (int k = 0; k < (int)(ButtonState::Count); k++) {
			m_categories[i].properties[k].icon.setNull();
		}
	}
	if (flagRedraw) {
		invalidate();
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

Color Button::getBackgroundColor()
{
	return View::getBackgroundColor();
}

void Button::setBackgroundColor(const Color& color, sl_bool flagRedraw)
{
	View::setBackgroundColor(color, flagRedraw);
}

void Button::resetStateBackgroundColors(sl_bool flagRedraw)
{
	for (sl_uint32 i = 0; i < m_nCategories; i++) {
		for (int k = 0; k < (int)(ButtonState::Count); k++) {
			m_categories[i].properties[k].backgroundColor = Color::zero();
		}
	}
	if (flagRedraw) {
		invalidate();
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

Ref<Drawable> Button::getBackground()
{
	return View::getBackground();
}

void Button::setBackground(const Ref<Drawable>& background, sl_bool flagRedraw)
{
	View::setBackground(background, flagRedraw);
}

void Button::resetStateBackgrounds(sl_bool flagRedraw)
{
	for (sl_uint32 i = 0; i < m_nCategories; i++) {
		for (int k = 0; k < (int)(ButtonState::Count); k++) {
			m_categories[i].properties[k].background.setNull();
		}
	}
	if (flagRedraw) {
		invalidate();
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

Ref<Pen> Button::getBorder()
{
	return View::getBorder();
}

void Button::setBorder(const Ref<Pen>& pen, sl_bool flagRedraw)
{
	View::setBorder(pen, flagRedraw);
}

void Button::resetStateBorders(sl_bool flagRedraw)
{
	for (sl_uint32 i = 0; i < m_nCategories; i++) {
		for (int k = 0; k < (int)(ButtonState::Count); k++) {
			m_categories[i].properties[k].border.setNull();
		}
	}
	if (flagRedraw) {
		invalidate();
	}
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
		textColor = m_textColorDefault;
	}
	if (icon.isNull()) {
		icon = m_iconDefault;
	}
	drawContent(canvas, icon, m_text, textColor);
}

void Button::onDrawBackground(Canvas* canvas)
{
	ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
	Color color = params.backgroundColor;
	Ref<Drawable> background = params.background;
	if (color.isZero()) {
		color = View::getBackgroundColor();
	}
	if (background.isNull()) {
		background = View::getBackground();
	}
	if (background.isNotNull() || color.a > 0) {
		drawBackground(canvas, color, background);		
	}
}

void Button::onDrawBorder(Canvas* canvas)
{
	ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
	Ref<Pen> pen = params.border;
	if (pen.isNull()) {
		pen = getBorder();
	}
	if (pen.isNotNull()) {
		drawBorder(canvas, pen);
	}
}

void Button::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagVertical && !flagHorizontal) {
		return;
	}
	
	Ref<GraphicsContext> gc = getGraphicsContext();
	if (gc.isNull()) {
		return;
	}
	
	UISize size = measureContentSize(gc.ptr);
	
	if (flagHorizontal) {
		if (size.x < 0) {
			size.x = 0;
		}
		sl_ui_pos width = size.x + getPaddingLeft() + getPaddingRight();
		if (width < 0) {
			width = 0;
		}
		setMeasuredWidth(width);
	}
	if (flagVertical) {
		if (size.y < 0) {
			size.y = 0;
		}
		sl_ui_pos height = size.y + getPaddingTop() + getPaddingBottom();
		if (height < 0) {
			height = 0;
		}
		setMeasuredHeight(height);
	}
	
}

UISize Button::measureContentSize(GraphicsContext* gc)
{
	UISize size;
	UIRect rcIcon, rcText;
	layoutIconAndText(gc, 0, 0, size, rcIcon, rcText);
	return size;
}

void Button::layoutIconAndText(GraphicsContext* gc, sl_ui_len widthFrame, sl_ui_len heightFrame, UISize& sizeContent, UIRect& frameIcon, UIRect& frameText)
{
	sl_ui_pos widthIcon = m_iconSize.x + m_iconMarginLeft + m_iconMarginRight;
	if (widthIcon < 0) {
		widthIcon = 0;
	}
	sl_ui_pos heightIcon = m_iconSize.y + m_iconMarginTop + m_iconMarginBottom;
	if (heightIcon < 0) {
		heightIcon = 0;
	}
	
	Ref<Font> font = getFont();
	UISize sizeText = gc->getFontTextSize(font, m_text);
	if (font.isNotNull()) {
		sizeText.y = (sl_ui_pos)(font->getSize());
	}
	sl_ui_pos widthText = sizeText.x + m_textMarginLeft + m_textMarginRight;
	if (widthText < 0) {
		widthText = 0;
	}
	sl_ui_pos heightText = sizeText.y + m_textMarginTop + m_textMarginBottom;
	if (heightText < 0) {
		heightText = 0;
	}
	
	Alignment alignIcon = m_iconAlignment;
	Alignment horzIcon = alignIcon & Alignment::HorizontalMask;
	Alignment vertIcon = alignIcon & Alignment::VerticalMask;
	
	Alignment alignText = m_textAlignment;
	Alignment horzText = alignText & Alignment::HorizontalMask;
	Alignment vertText = alignText & Alignment::VerticalMask;
	
	sl_ui_pos xIcon = 0;
	sl_ui_pos yIcon = 0;
	
	sl_ui_pos xText = 0;
	sl_ui_pos yText = 0;
	
	sl_ui_pos widthContent = 0;
	sl_ui_pos heightContent = 0;
	
	if (m_layoutOrientation == LayoutOrientation::Horizontal) {
		
		if (horzIcon != horzText) {
			widthContent = widthFrame;
			if (widthContent > 0) {
				if (m_flagTextBeforeIcon) {
					xText = (sl_ui_pos)(GraphicsUtil::calculateAlignX(0, (sl_real)widthContent, (sl_real)widthText, alignText));
					if (horzText == Alignment::Right) {
						xIcon = (sl_ui_pos)(GraphicsUtil::calculateAlignX(0, (sl_real)xText, (sl_real)widthIcon, alignIcon));
					} else {
						xIcon = (sl_ui_pos)(GraphicsUtil::calculateAlignX((sl_real)(xText + widthText), (sl_real)widthContent, (sl_real)widthIcon, alignIcon));
					}
				} else {
					xIcon = (sl_ui_pos)(GraphicsUtil::calculateAlignX(0, (sl_real)widthContent, (sl_real)widthIcon, alignIcon));
					if (horzIcon == Alignment::Right) {
						xText = (sl_ui_pos)(GraphicsUtil::calculateAlignX(0, (sl_real)xIcon, (sl_real)widthText, alignText));
					} else {
						xText = (sl_ui_pos)(GraphicsUtil::calculateAlignX((sl_real)(xIcon + widthIcon), (sl_real)widthContent, (sl_real)widthText, alignText));
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
			yIcon = (sl_ui_pos)(GraphicsUtil::calculateAlignY(0, (sl_real)heightContent, (sl_real)heightIcon, alignIcon));
			yText = (sl_ui_pos)(GraphicsUtil::calculateAlignY(0, (sl_real)heightContent, (sl_real)heightText, alignText));
		}
		
	} else {
		if (horzIcon != horzText) {
			widthContent = widthFrame;
		} else {
			widthContent = SLIB_MAX(widthIcon, widthText);
		}
		if (widthContent >= 0) {
			xIcon = (sl_ui_pos)(GraphicsUtil::calculateAlignX(0, (sl_real)widthContent, (sl_real)widthIcon, alignIcon));
			xText = (sl_ui_pos)(GraphicsUtil::calculateAlignX(0, (sl_real)widthContent, (sl_real)widthText, alignText));
		}
		
		if (vertIcon != vertText) {
			heightContent = heightFrame;
			if (heightContent > 0) {
				if (m_flagTextBeforeIcon) {
					yText = (sl_ui_pos)(GraphicsUtil::calculateAlignY(0, (sl_real)heightContent, (sl_real)heightText, alignText));
					if (vertText == Alignment::Bottom) {
						yIcon = (sl_ui_pos)(GraphicsUtil::calculateAlignY(0, (sl_real)yText, (sl_real)heightIcon, alignIcon));
					} else {
						yIcon = (sl_ui_pos)(GraphicsUtil::calculateAlignY((sl_real)(yText + heightText), (sl_real)heightContent, (sl_real)heightIcon, alignIcon));
					}
				} else {
					yIcon = (sl_ui_pos)(GraphicsUtil::calculateAlignY(0, (sl_real)heightContent, (sl_real)heightIcon, alignIcon));
					if (vertIcon == Alignment::Bottom) {
						yText = (sl_ui_pos)(GraphicsUtil::calculateAlignY(0, (sl_real)yIcon, (sl_real)heightText, alignText));
					} else {
						yText = (sl_ui_pos)(GraphicsUtil::calculateAlignY((sl_real)(yIcon + heightIcon), (sl_real)heightContent, (sl_real)heightText, alignText));
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
	
	if (widthContent < 0) {
		widthContent = 0;
	}
	if (heightContent < 0) {
		heightContent = 0;
	}
	sizeContent.x = widthContent;
	sizeContent.y = heightContent;
	
	frameIcon.left = xIcon + m_iconMarginLeft;
	frameIcon.top = yIcon + m_iconMarginTop;
	frameIcon.right = xIcon + widthIcon - m_iconMarginRight;
	frameIcon.bottom = yIcon + heightIcon - m_iconMarginBottom;
	frameIcon.fixSizeError();
	
	frameText.left = xText + m_textMarginLeft;
	frameText.top = yText + m_textMarginTop;
	frameText.right = xText + widthText - m_textMarginRight;
	frameText.bottom = yText + heightText - m_textMarginBottom;
	frameText.fixSizeError();
	
}

void Button::drawContent(Canvas* canvas, const Ref<Drawable>& icon, const String& text, const Color& textColor)
{
	if (text.isEmpty() && icon.isNull()) {
		return;
	}
	Ref<GraphicsContext> gc = canvas->getGraphicsContext();
	if (gc.isNull()) {
		return;
	}
	UIRect bound = getBoundsInnerPadding();
	sl_ui_pos widthFrame = bound.getWidth();
	sl_ui_pos heightFrame = bound.getHeight();
	if (widthFrame <= 0 || heightFrame <= 0) {
		return;
	}
	
	UISize sizeContent;
	UIRect rcIcon, rcText;
	layoutIconAndText(gc.ptr, widthFrame, heightFrame, sizeContent, rcIcon, rcText);
	UIPoint pt = GraphicsUtil::calculateAlignPosition(bound, (sl_real)(sizeContent.x), (sl_real)(sizeContent.y), m_gravity);
	if (icon.isNotNull() && rcIcon.getWidth() > 0 && rcIcon.getHeight() > 0) {
		rcIcon.left += pt.x;
		rcIcon.top += pt.y;
		rcIcon.right += pt.x;
		rcIcon.bottom += pt.y;
		canvas->draw(rcIcon, icon);
	}
	if (text.isNotEmpty() && rcText.getWidth() > 0 && rcText.getHeight() > 0) {
		rcText.left += pt.x;
		rcText.top += pt.y;
		rcText.right += pt.x;
		rcText.bottom += pt.y;
		canvas->drawText(text, (sl_real)(rcText.left), (sl_real)(rcText.top), getFont(), textColor);
	}
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
