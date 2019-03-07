/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/ui/button.h"

#include "slib/graphics/util.h"
#include "slib/core/safe_static.h"
#include "slib/core/new_helper.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(Button, View)

	const sl_real _g_button_colorMatrix_hover_buf[20] = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1,
		0.2f, 0.3f, 0.4f, 0
	};
	const ColorMatrix& _g_button_colorMatrix_hover = *((const ColorMatrix*)((void*)_g_button_colorMatrix_hover_buf));

	const sl_real _g_button_colorMatrix_pressed_buf[20] = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1,
		0.3f, 0.4f, 0.6f, 0

	};
	const ColorMatrix& _g_button_colorMatrix_pressed = *((const ColorMatrix*)((void*)_g_button_colorMatrix_pressed_buf));

	const sl_real _g_button_colorMatrix_disabled_buf[20] = {
		0.2f, 0.2f, 0.2f, 0,
		0.2f, 0.2f, 0.2f, 0,
		0.2f, 0.2f, 0.2f, 0,
		0, 0, 0, 1,
		0, 0, 0, 0
	};
	const ColorMatrix& _g_button_colorMatrix_disabled = *((const ColorMatrix*)((void*)_g_button_colorMatrix_disabled_buf));

	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ButtonCategoryProperties)
	
	ButtonCategoryProperties::ButtonCategoryProperties()
	{
		textColor = Color::zero();
		flagFilter = sl_false;
	}
	
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(ButtonCategory)
	
	ButtonCategory::ButtonCategory()
	{
	}
	

	class _priv_Button_Categories
	{
	public:
		ButtonCategory categories[2];
		
	public:
		_priv_Button_Categories()
		{
			categories[1].properties[(int)(ButtonState::Normal)].border = Pen::create(PenStyle::Solid, 3, Color(0, 100, 250));
		}
		
	public:
		static ButtonCategory* getCategories()
		{
			SLIB_SAFE_STATIC(_priv_Button_Categories, ret)
			if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
				return sl_null;
			}
			return ret.categories;
		}
	};

	Button::Button() : Button(2)
	{
	}

	Button::Button(sl_uint32 nCategories, ButtonCategory* categories)
	{
		setCreatingNativeWidget(sl_false);
		setFocusable(sl_true);
		
		m_flagMultiLine = sl_false;
		m_flagDefaultButton = sl_false;
		
		m_state = ButtonState::Normal;
		m_category = 0;
		
		m_iconSize.x = 0;
		m_iconSize.y = 0;
		m_gravity = Alignment::MiddleCenter;
		m_iconAlignment = Alignment::MiddleCenter;
		m_textAlignment = Alignment::MiddleCenter;
		m_flagTextBeforeIcon = sl_false;
		m_layoutOrientation = LayoutOrientation::Horizontal;
		
		m_iconMarginLeft = 1;
		m_iconMarginTop = 1;
		m_iconMarginRight = 1;
		m_iconMarginBottom = 1;
		
		m_textMarginLeft = 1;
		m_textMarginTop = 1;
		m_textMarginRight = 1;
		m_textMarginBottom = 1;
		
		setPadding(1, 1, 1, 1, UIUpdateMode::Init);
		
		m_flagUseDefaultColorFilter = sl_true;

		if (nCategories == 0) {
			nCategories = 1;
		}
		m_nCategories = nCategories;
		m_categories = NewHelper<ButtonCategory>::create(nCategories);
		if (!categories) {
			categories = _priv_Button_Categories::getCategories();
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
		
		setUsingFont(sl_true);
		
		m_textColorDefault = Color(0, 100, 200);
		
	}

	Button::~Button()
	{
		NewHelper<ButtonCategory>::free(m_categories, m_nCategories);
	}

	String Button::getText()
	{
		return m_text;
	}

	void Button::setText(const String& text, UIUpdateMode mode)
	{
		m_text = text;
		if (isNativeWidget()) {
			_setText_NW(text);
		}
		invalidateLayoutOfWrappingControl(mode);
	}

	sl_bool Button::isDefaultButton()
	{
		return m_flagDefaultButton;
	}

	void Button::setDefaultButton(sl_bool flag, UIUpdateMode mode)
	{
		m_flagDefaultButton = flag;
		if (flag) {
			setCurrentCategory(1, UIUpdateMode::None);
		} else {
			setCurrentCategory(0, UIUpdateMode::None);
		}
		if (isNativeWidget()) {
			_setDefaultButton_NW(flag);
		} else {
			invalidate(mode);
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

	void Button::setCurrentCategory(sl_uint32 n, UIUpdateMode mode)
	{
		if (n < m_nCategories) {
			m_category = n;
			invalidate(mode);
		}
	}

	sl_bool Button::isMultiLine()
	{
		return m_flagMultiLine;
	}

	void Button::setMultiLine(sl_bool flag, UIUpdateMode mode)
	{
		m_flagMultiLine = flag;
		invalidateLayoutOfWrappingControl(mode);
	}

	const UISize& Button::getIconSize()
	{
		return m_iconSize;
	}

	void Button::setIconSize(const UISize& size, UIUpdateMode mode)
	{
		m_iconSize = size;
		invalidateLayoutOfWrappingControl(mode);
	}

	void Button::setIconSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode)
	{
		setIconSize(UISize(width, height), mode);
	}

	void Button::setIconSize(sl_ui_len size, UIUpdateMode mode)
	{
		setIconSize(UISize(size, size), mode);
	}

	sl_ui_len Button::getIconWidth()
	{
		return m_iconSize.x;
	}

	void Button::setIconWidth(sl_ui_len width, UIUpdateMode mode)
	{
		setIconSize(UISize(width, m_iconSize.y), mode);
	}

	sl_ui_len Button::getIconHeight()
	{
		return m_iconSize.y;
	}

	void Button::setIconHeight(sl_ui_len height, UIUpdateMode mode)
	{
		setIconSize(UISize(m_iconSize.x, height), mode);
	}

	Alignment Button::getGravity()
	{
		return m_gravity;
	}

	void Button::setGravity(Alignment align, UIUpdateMode mode)
	{
		m_gravity = align;
		invalidate(mode);
	}

	Alignment Button::getIconAlignment()
	{
		return m_iconAlignment;
	}

	void Button::setIconAlignment(Alignment align, UIUpdateMode mode)
	{
		m_iconAlignment = align;
		invalidateLayoutOfWrappingControl(mode);
	}

	Alignment Button::getTextAlignment()
	{
		return m_textAlignment;
	}

	void Button::setTextAlignment(Alignment align, UIUpdateMode mode)
	{
		m_textAlignment = align;
		invalidateLayoutOfWrappingControl(mode);
	}

	sl_bool Button::isTextBeforeIcon()
	{
		return m_flagTextBeforeIcon;
	}

	void Button::setTextBeforeIcon(sl_bool flag, UIUpdateMode mode)
	{
		m_flagTextBeforeIcon = flag;
		invalidateLayoutOfWrappingControl(mode);
	}

	LayoutOrientation Button::getLayoutOrientation()
	{
		return m_layoutOrientation;
	}

	void Button::setLayoutOrientation(LayoutOrientation orientation, UIUpdateMode mode)
	{
		m_layoutOrientation = orientation;
		invalidateLayoutOfWrappingControl(mode);
	}

	void Button::setIconMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		m_iconMarginLeft = left;
		m_iconMarginTop = top;
		m_iconMarginRight = right;
		m_iconMarginBottom = bottom;
		invalidateLayoutOfWrappingControl(mode);
	}

	void Button::setIconMargin(sl_ui_pos margin, UIUpdateMode mode)
	{
		setIconMargin(margin, margin, margin, margin, mode);
	}

	sl_ui_pos Button::getIconMarginLeft()
	{
		return m_iconMarginLeft;
	}

	void Button::setIconMarginLeft(sl_ui_pos margin, UIUpdateMode mode)
	{
		setIconMargin(margin, m_iconMarginTop, m_iconMarginRight, m_iconMarginBottom, mode);
	}

	sl_ui_pos Button::getIconMarginTop()
	{
		return m_iconMarginTop;
	}

	void Button::setIconMarginTop(sl_ui_pos margin, UIUpdateMode mode)
	{
		setIconMargin(m_iconMarginLeft, margin, m_iconMarginRight, m_iconMarginBottom, mode);
	}

	sl_ui_pos Button::getIconMarginRight()
	{
		return m_iconMarginRight;
	}

	void Button::setIconMarginRight(sl_ui_pos margin, UIUpdateMode mode)
	{
		setIconMargin(m_iconMarginLeft, m_iconMarginTop, margin, m_iconMarginBottom, mode);
	}

	sl_ui_pos Button::getIconMarginBottom()
	{
		return m_iconMarginBottom;
	}

	void Button::setIconMarginBottom(sl_ui_pos margin, UIUpdateMode mode)
	{
		setIconMargin(m_iconMarginLeft, m_iconMarginTop, m_iconMarginRight, margin, mode);
	}

	void Button::setTextMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		m_textMarginLeft = left;
		m_textMarginTop = top;
		m_textMarginRight = right;
		m_textMarginBottom = bottom;
		invalidateLayoutOfWrappingControl(mode);
	}

	void Button::setTextMargin(sl_ui_pos margin, UIUpdateMode mode)
	{
		setTextMargin(margin, margin, margin, margin, mode);
	}

	sl_ui_pos Button::getTextMarginLeft()
	{
		return m_textMarginLeft;
	}

	void Button::setTextMarginLeft(sl_ui_pos margin, UIUpdateMode mode)
	{
		setTextMargin(margin, m_textMarginTop, m_textMarginRight, m_textMarginBottom, mode);
	}

	sl_ui_pos Button::getTextMarginTop()
	{
		return m_textMarginTop;
	}

	void Button::setTextMarginTop(sl_ui_pos margin, UIUpdateMode mode)
	{
		setTextMargin(m_textMarginLeft, margin, m_textMarginRight, m_textMarginBottom, mode);
	}

	sl_ui_pos Button::getTextMarginRight()
	{
		return m_textMarginRight;
	}

	void Button::setTextMarginRight(sl_ui_pos margin, UIUpdateMode mode)
	{
		setTextMargin(m_textMarginLeft, m_textMarginTop, margin, m_textMarginBottom, mode);
	}

	sl_ui_pos Button::getTextMarginBottom()
	{
		return m_textMarginBottom;
	}

	void Button::setTextMarginBottom(sl_ui_pos margin, UIUpdateMode mode)
	{
		setTextMargin(m_textMarginLeft, m_textMarginTop, m_textMarginRight, margin, mode);
	}

	Color Button::getTextColor(ButtonState state, sl_uint32 category)
	{
		if (category < m_nCategories && (int)state < (int)(ButtonState::Count)) {
			return m_categories[category].properties[(int)state].textColor;
		} else {
			return Color::zero();
		}
	}

	void Button::setTextColor(const Color& color, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (category < m_nCategories) {
			if ((int)state < (int)(ButtonState::Count)) {
				m_categories[category].properties[(int)state].textColor = color;
			} else {
				for (int i = 0; i < (int)(ButtonState::Count); i++) {
					m_categories[category].properties[i].textColor = color;
				}
			}
			invalidate(mode);
		}
	}

	Color Button::getTextColor()
	{
		return m_textColorDefault;
	}

	void Button::setTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColorDefault = color;
		invalidate(mode);
	}

	Ref<Drawable> Button::getIcon(ButtonState state, sl_uint32 category)
	{
		if (category < m_nCategories && (int)state < (int)(ButtonState::Count)) {
			return m_categories[category].properties[(int)state].icon;
		} else {
			return sl_null;
		}
	}

	void Button::setIcon(const Ref<Drawable>& icon, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (category < m_nCategories) {
			if ((int)state < (int)(ButtonState::Count)) {
				m_categories[category].properties[(int)state].icon = icon;
			} else {
				for (int i = 0; i < (int)(ButtonState::Count); i++) {
					m_categories[category].properties[i].icon = icon;
				}
			}
			invalidateLayoutOfWrappingControl(mode);
		}
	}

	Ref<Drawable> Button::getIcon()
	{
		return m_iconDefault;
	}

	void Button::setIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
	{
		m_iconDefault = icon;
		invalidateLayoutOfWrappingControl(mode);
	}

	Ref<Drawable> Button::getBackground(ButtonState state, sl_uint32 category)
	{
		if (category < m_nCategories && (int)state < (int)(ButtonState::Count)) {
			return m_categories[category].properties[(int)state].background;
		} else {
			return sl_null;
		}
	}

	void Button::setBackground(const Ref<Drawable>& background, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (category < m_nCategories) {
			if ((int)state < (int)(ButtonState::Count)) {
				m_categories[category].properties[(int)state].background = background;
			} else {
				for (int i = 0; i < (int)(ButtonState::Count); i++) {
					m_categories[category].properties[i].background = background;
				}
			}
			invalidate(mode);
		}
	}

	void Button::setBackground(const Color& color, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		setBackground(Drawable::createColorDrawable(color), state, category, mode);
	}

	Ref<Drawable> Button::getBackground()
	{
		return View::getBackground();
	}

	void Button::setBackground(const Ref<Drawable>& background, UIUpdateMode mode)
	{
		View::setBackground(background, mode);
	}

	Ref<Pen> Button::getBorder(ButtonState state, sl_uint32 category)
	{
		if (category < m_nCategories && (int)state < (int)(ButtonState::Count)) {
			return m_categories[category].properties[(int)state].border;
		} else {
			return sl_null;
		}
	}

	void Button::setBorder(const Ref<Pen>& pen, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (category < m_nCategories) {
			if ((int)state < (int)(ButtonState::Count)) {
				m_categories[category].properties[(int)state].border = pen;
			} else {
				for (int i = 0; i < (int)(ButtonState::Count); i++) {
					m_categories[category].properties[i].border = pen;
				}
			}
			invalidate(mode);
		}
	}

	Ref<Pen> Button::getBorder()
	{
		return View::getBorder();
	}

	void Button::setBorder(const Ref<Pen>& pen, UIUpdateMode mode)
	{
		View::setBorder(pen, mode);
	}

	void Button::setBorder(sl_bool flagBorder, UIUpdateMode mode)
	{
		View::setBorder(flagBorder, mode);
	}

	ColorMatrix* Button::getColorFilter(ButtonState state, sl_uint32 category)
	{
		if (category < m_nCategories && (int)state < (int)(ButtonState::Count)) {
			ButtonCategoryProperties& props = m_categories[category].properties[(int)state];
			if (props.flagFilter) {
				return &(props.filter);
			}
		}
		return sl_null;
	}
	
	void Button::setColorFilter(ColorMatrix* filter, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		setUsingDefaultColorFilter(sl_false, UIUpdateMode::None);
		if (category < m_nCategories) {
			if ((int)state < (int)(ButtonState::Count)) {
				ButtonCategoryProperties& props = m_categories[category].properties[(int)state];
				if (filter) {
					props.flagFilter = sl_true;
					props.filter = *filter;
				} else {
					props.flagFilter = sl_false;
				}
			} else {
				for (int i = 0; i < (int)(ButtonState::Count); i++) {
					ButtonCategoryProperties& props = m_categories[category].properties[i];
					if (filter) {
						props.flagFilter = sl_true;
						props.filter = *filter;
					} else {
						props.flagFilter = sl_false;
					}
				}
			}
			invalidate(mode);
		}
	}
	
	ColorMatrix* Button::getColorFilter()
	{
		return getColorFilter(ButtonState::Normal);
	}
	
	void Button::setColorFilter(ColorMatrix* filter, UIUpdateMode mode)
	{
		setColorFilter(filter, ButtonState::Normal, 0, mode);
	}
	
	void Button::setColorOverlay(const Color& color, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (color.isZero()) {
			setColorFilter(sl_null, state, category, mode);
		} else {
			ColorMatrix cm;
			cm.setOverlay(color);
			setColorFilter(&cm, state, category, mode);
		}
	}
	
	void Button::setColorOverlay(const Color& color, UIUpdateMode mode)
	{
		setColorOverlay(color, ButtonState::Normal, 0, mode);
	}
	
	sl_bool Button::isUsingDefaultColorFilter()
	{
		return m_flagUseDefaultColorFilter;
	}

	void Button::setUsingDefaultColorFilter(sl_bool flag, UIUpdateMode mode)
	{
		m_flagUseDefaultColorFilter = flag;
		invalidate(mode);
	}

	void Button::setEnabled(sl_bool flagEnabled, UIUpdateMode mode)
	{
		if (isEnabled() != flagEnabled) {
			View::setEnabled(flagEnabled, UIUpdateMode::None);
			_invalidateButtonState();
			invalidate(mode);
		}
	}

	void Button::setPressedState(sl_bool flagState, UIUpdateMode mode)
	{
		if (isPressedState() != flagState) {
			View::setPressedState(flagState, UIUpdateMode::None);
			_invalidateButtonState();
			invalidate(mode);
		}
	}

	void Button::setHoverState(sl_bool flagState, UIUpdateMode mode)
	{
		if (isHoverState() != flagState) {
			View::setHoverState(flagState, UIUpdateMode::None);
			_invalidateButtonState();
			invalidate(mode);
		}
	}

	void Button::onDraw(Canvas* canvas)
	{
		ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
		Color textColor = params.textColor;
		{
			const ColorMatrix* cm = getCurrentColorFilter(textColor.isZero() && m_flagUseDefaultColorFilter);
			if (textColor.isZero()) {
				textColor = m_textColorDefault;
			}
			if (cm) {
				textColor = cm->transformColor(textColor);
			}
		}
		Ref<Drawable> icon = params.icon;
		{
			const ColorMatrix* cm = getCurrentColorFilter(icon.isNull() && m_flagUseDefaultColorFilter);
			if (icon.isNull()) {
				icon = m_iconDefault;
			}
			if (icon.isNotNull() && cm) {
				icon = icon->filter(*cm);
			}
		}
		drawButtonContent(canvas, icon, m_text, textColor);
	}

	void Button::onDrawBackground(Canvas* canvas)
	{
		ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
		Ref<Drawable> background = params.background;
		sl_bool flagUseDefaultColorFilter = m_flagUseDefaultColorFilter;
		if (background.isNull()) {
			Ref<DrawAttributes>& attrs = m_drawAttrs;
			if (attrs.isNotNull()) {
				switch (m_state) {
					case ButtonState::Hover:
						background = attrs->backgroundHover;
						break;
					case ButtonState::Pressed:
						background = attrs->backgroundPressed;
						break;
					default:
						break;
				}
				if (background.isNull()) {
					background = attrs->background;
				} else {
					flagUseDefaultColorFilter = sl_false;
				}
			}
		} else {
			flagUseDefaultColorFilter = sl_false;
		}
		if (background.isNotNull()) {
			const ColorMatrix* cm = getCurrentColorFilter(flagUseDefaultColorFilter);
			if (cm) {
				background = background->filter(*cm);
			}
			drawBackground(canvas, background);
		}
	}

	void Button::onDrawBorder(Canvas* canvas)
	{
		ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
		Ref<Pen> pen = params.border;
		if (pen.isNotNull()) {
			drawBorder(canvas, pen);
		} else {
			View::onDrawBorder(canvas);
		}
	}

	void Button::onUpdateLayout()
	{
		sl_bool flagHorizontal = isWidthWrapping();
		sl_bool flagVertical = isHeightWrapping();
		
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		UISize size = measureLayoutContentSize(flagHorizontal ? 0 : getLayoutWidth(), flagVertical ? 0 : getLayoutHeight());
		if (flagHorizontal) {
			setLayoutWidth(size.x + getPaddingLeft() + getPaddingRight());
		}
		if (flagVertical) {
			setLayoutHeight(size.y + getPaddingTop() + getPaddingBottom());
		}
	}

	void Button::onKeyEvent(UIEvent* ev)
	{
		if (ev->getAction() == UIAction::KeyDown) {
			if (ev->getKeycode() == Keycode::Enter) {
				dispatchClick();
				ev->preventDefault();
			}
		}
	}

	UISize Button::measureContentSize(sl_ui_len widthFrame, sl_ui_len heightFrame)
	{
		UISize size;
		UIRect rcIcon, rcText;
		layoutIconAndText(widthFrame, heightFrame, size, rcIcon, rcText);
		if (size.x < 0) {
			size.x = 0;
		}
		if (size.y < 0) {
			size.y = 0;
		}
		return size;
	}
	
	UISize _priv_Button_macOS_measureSize(Button* view);

	UISize Button::measureLayoutContentSize(sl_ui_len widthFrame, sl_ui_len heightFrame)
	{
#if defined(SLIB_UI_IS_MACOS)
		if (isCreatingNativeWidget()) {
			return _priv_Button_macOS_measureSize(this);
		}
#endif
		return measureContentSize(widthFrame, heightFrame);
	}

	void Button::layoutIconAndText(sl_ui_len widthFrame, sl_ui_len heightFrame, UISize& sizeContent, UIRect& frameIcon, UIRect& frameText)
	{
		sl_ui_pos widthText = 0;
		sl_ui_pos heightText = 0;
		sl_ui_pos widthIcon = 0;
		sl_ui_pos heightIcon = 0;
		String text = m_text;
		Ref<Font> font;
		if (text.isNotEmpty()) {
			font = getFont();
			if (font.isNotNull()) {
				UISize sizeText = measureText(text, font, m_flagMultiLine);
				widthText = sizeText.x + m_textMarginLeft + m_textMarginRight;
				if (widthText < 0) {
					widthText = 0;
				}
				heightText = sizeText.y + m_textMarginTop + m_textMarginBottom;
				if (heightText < 0) {
					heightText = 0;
				}
			}
		}
		sl_bool flagUseIcon = m_iconDefault.isNotNull() || m_iconSize.x > 0 || m_iconSize.y > 0;
		if (!flagUseIcon) {
			for (sl_uint32 i = 0; i < m_nCategories; i++) {
				ButtonCategory& category = m_categories[i];
				for (sl_uint32 j = 0; j < (sl_uint32)(ButtonState::Count); j++) {
					if (category.properties[j].icon.isNotNull()) {
						flagUseIcon = sl_true;
					}
				}
			}
		}
		if (flagUseIcon) {
			widthIcon = m_iconSize.x;
			heightIcon = m_iconSize.y;
			sl_ui_len marginWidth = m_iconMarginLeft + m_iconMarginRight;
			sl_ui_len marginHeight = m_iconMarginTop + m_iconMarginBottom;
			if (widthIcon <= 0) {
				if (heightIcon <= 0) {
					if (text.isNotEmpty()) {
						sl_ui_len defaultHeight = heightText;
						if (defaultHeight <= 0) {
							if (font.isNotNull()) {
								defaultHeight = (sl_ui_len)(font->getFontHeight());
							} else {
								defaultHeight = 20;
							}
						}
						defaultHeight = (sl_ui_len)(defaultHeight * 0.9f);
						widthIcon = defaultHeight;
						heightIcon = defaultHeight;
					} else {
						if (widthFrame <= 0) {
							if (heightFrame <= 0) {
								font = getFont();
								if (font.isNotNull()) {
									widthIcon = (sl_ui_len)(font->getFontHeight());
								} else {
									widthIcon = 20;
								}
								heightIcon = widthIcon;
							} else {
								widthIcon = heightFrame;
								heightIcon = heightFrame;
							}
						} else {
							if (heightFrame <= 0) {
								widthIcon = widthFrame;
								heightIcon = widthFrame;
							} else {
								widthIcon = widthFrame;
								heightIcon = heightFrame;
							}
						}
					}
					widthIcon -= marginWidth;
					heightIcon -= marginHeight;
					widthIcon = Math::min(widthIcon, heightIcon);
					heightIcon = widthIcon;
				} else {
					widthIcon = heightIcon;
				}
			} else {
				if (heightIcon <= 0) {
					heightIcon = widthIcon;
				}
			}
			widthIcon += marginWidth;
			if (widthIcon < 0) {
				widthIcon = 0;
			}
			heightIcon += marginHeight;
			if (heightIcon < 0) {
				heightIcon = 0;
			}
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

	void Button::drawButtonContent(Canvas* canvas, const Ref<Drawable>& icon, const String& text, const Color& textColor)
	{
		if (text.isEmpty() && icon.isNull()) {
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
		layoutIconAndText(widthFrame, heightFrame, sizeContent, rcIcon, rcText);
		UIPoint pt = GraphicsUtil::calculateAlignPosition(bound, (sl_real)(sizeContent.x), (sl_real)(sizeContent.y), m_gravity);
		if (icon.isNotNull() && rcIcon.getWidth() > 0 && rcIcon.getHeight() > 0) {
			rcIcon.left += pt.x;
			rcIcon.top += pt.y;
			rcIcon.right += pt.x;
			rcIcon.bottom += pt.y;
			if (m_iconSize.x > 0 && m_iconSize.y > 0) {
				canvas->draw(rcIcon, icon);
			} else {
				canvas->draw(rcIcon, icon, ScaleMode::Contain, Alignment::MiddleCenter);
			}
		}
		if (text.isNotEmpty() && rcText.getWidth() > 0 && rcText.getHeight() > 0) {
			rcText.left += pt.x;
			rcText.top += pt.y;
			rcText.right += pt.x;
			rcText.bottom += pt.y;
			if (m_flagMultiLine) {
				canvas->drawText(text, rcText, getFont(), textColor, m_textAlignment, sl_true);
			} else {
				canvas->drawText(text, (sl_real)(rcText.left), (sl_real)(rcText.top), getFont(), textColor);
			}
		}
	}
	
	const ColorMatrix* Button::getCurrentColorFilter(sl_bool flagUseDefaultFilter)
	{
		ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
		if (params.flagFilter) {
			return &(params.filter);
		}
		if (flagUseDefaultFilter) {
			switch (m_state) {
				case ButtonState::Hover:
					return &_g_button_colorMatrix_hover;
				case ButtonState::Pressed:
					return &_g_button_colorMatrix_pressed;
				case ButtonState::Disabled:
					return &_g_button_colorMatrix_disabled;
				default:
					break;
			}
		} else if (m_state != ButtonState::Normal) {
			ButtonCategoryProperties& paramsDefault = m_categories[m_category].properties[(int)(ButtonState::Normal)];
			if (paramsDefault.flagFilter) {
				return &(paramsDefault.filter);
			}
		}
		return sl_null;
	}

	void Button::_invalidateButtonState()
	{
		if (isEnabled()) {
			if (isPressedState()) {
				m_state = ButtonState::Pressed;
			} else if (isHoverState()) {
				m_state = ButtonState::Hover;
			} else {
				m_state = ButtonState::Normal;
			}
		} else {
			m_state = ButtonState::Disabled;
		}
	}

#if !defined(SLIB_UI_IS_MACOS) && !defined(SLIB_UI_IS_WIN32)
	Ref<ViewInstance> Button::createNativeWidget(ViewInstance* parent)
	{
		return sl_null;
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

}
