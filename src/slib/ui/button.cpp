/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/button.h"

#include "slib/graphics/util.h"
#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(Button, View)

	const sl_real _g_button_colorMatrix_hover_buf[20] = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1,
		0, 0, 0.5f, 0
	};
	const ColorMatrix& _g_button_colorMatrix_hover = *((const ColorMatrix*)((void*)_g_button_colorMatrix_hover_buf));

	const sl_real _g_button_colorMatrix_pressed_buf[20] = {
		0.6f, 0, 0, 0,
		0, 0.6f, 0, 0,
		0, 0, 0.6f, 0,
		0, 0, 0, 1,
		0, 0, 0, 0
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

	ButtonCategoryProperties::ButtonCategoryProperties()
	{
		textColor = Color::zero();
	}

	class _Button_Categories
	{
	public:
		ButtonCategory categories[2];
		
	public:
		_Button_Categories()
		{
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
		setSavingCanvasState(sl_false);
	}

	Button::Button(sl_uint32 nCategories, ButtonCategory* categories)
	{
		setCreatingNativeWidget(sl_false);
		setFocusable(sl_true);
		
		setAlwaysOnDrawBackground(sl_true, UIUpdateMode::Init);
		setAlwaysOnDrawBorder(sl_true, UIUpdateMode::Init);
		
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
		
		m_iconMarginLeft = 0;
		m_iconMarginTop = 0;
		m_iconMarginRight = 0;
		m_iconMarginBottom = 0;
		
		m_textMarginLeft = 2;
		m_textMarginTop = 2;
		m_textMarginRight = 2;
		m_textMarginBottom = 2;
		
		m_flagUseDefaultColorFilter = sl_true;

		if (nCategories == 0) {
			nCategories = 1;
		}
		m_nCategories = nCategories;
		m_categories = NewHelper<ButtonCategory>::create(nCategories);
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
		
		setUsingFont(sl_true);
		
		setBorder(Pen::create(PenStyle::Solid, 1, Color(100, 100, 100)), UIUpdateMode::Init);

		m_textColorDefault = Color::Black;
		
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
		invalidateLayoutFromResizeContent(mode);
	}

	sl_bool Button::isDefaultButton()
	{
		return m_flagDefaultButton;
	}

	void Button::setDefaultButton(sl_bool flag, UIUpdateMode mode)
	{
		m_flagDefaultButton = flag;
		if (flag) {
			setCurrentCategory(1, UIUpdateMode::NoRedraw);
		} else {
			setCurrentCategory(0, UIUpdateMode::NoRedraw);
		}
		if (isNativeWidget()) {
			_setDefaultButton_NW(flag);
		} else {
			if (mode == UIUpdateMode::Redraw) {
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

	void Button::setCurrentCategory(sl_uint32 n, UIUpdateMode mode)
	{
		if (n < m_nCategories) {
			m_category = n;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	sl_bool Button::isMultiLine()
	{
		return m_flagMultiLine;
	}

	void Button::setMultiLine(sl_bool flag, UIUpdateMode mode)
	{
		m_flagMultiLine = flag;
		invalidateLayoutFromResizeContent(mode);
	}

	const UISize& Button::getIconSize()
	{
		return m_iconSize;
	}

	void Button::setIconSize(const UISize& size, UIUpdateMode mode)
	{
		m_iconSize = size;
		invalidateLayoutFromResizeContent(mode);
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
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}

	Alignment Button::getIconAlignment()
	{
		return m_iconAlignment;
	}

	void Button::setIconAlignment(Alignment align, UIUpdateMode mode)
	{
		m_iconAlignment = align;
		invalidateLayoutFromResizeContent(mode);
	}

	Alignment Button::getTextAlignment()
	{
		return m_textAlignment;
	}

	void Button::setTextAlignment(Alignment align, UIUpdateMode mode)
	{
		m_textAlignment = align;
		invalidateLayoutFromResizeContent(mode);
	}

	sl_bool Button::isTextBeforeIcon()
	{
		return m_flagTextBeforeIcon;
	}

	void Button::setTextBeforeIcon(sl_bool flag, UIUpdateMode mode)
	{
		m_flagTextBeforeIcon = flag;
		invalidateLayoutFromResizeContent(mode);
	}

	LayoutOrientation Button::getLayoutOrientation()
	{
		return m_layoutOrientation;
	}

	void Button::setLayoutOrientation(LayoutOrientation orientation, UIUpdateMode mode)
	{
		m_layoutOrientation = orientation;
		invalidateLayoutFromResizeContent(mode);
	}

	void Button::setIconMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode)
	{
		m_iconMarginLeft = left;
		m_iconMarginTop = top;
		m_iconMarginRight = right;
		m_iconMarginBottom = bottom;
		invalidateLayoutFromResizeContent(mode);
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
		invalidateLayoutFromResizeContent(mode);
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
		if (category < m_nCategories) {
			return m_categories[category].properties[(int)state].textColor;
		} else {
			return Color::zero();
		}
	}

	void Button::setTextColor(const Color& color, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (category < m_nCategories) {
			m_categories[category].properties[(int)state].textColor = color;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	Color Button::getTextColor()
	{
		return m_textColorDefault;
	}

	void Button::setTextColor(const Color& color, UIUpdateMode mode)
	{
		m_textColorDefault = color;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}

	Ref<Drawable> Button::getIcon(ButtonState state, sl_uint32 category)
	{
		if (category < m_nCategories) {
			return m_categories[category].properties[(int)state].icon;
		} else {
			return sl_null;
		}
	}

	void Button::setIcon(const Ref<Drawable>& icon, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (category < m_nCategories) {
			m_categories[category].properties[(int)state].icon = icon;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	Ref<Drawable> Button::getIcon()
	{
		return m_iconDefault;
	}

	void Button::setIcon(const Ref<Drawable>& icon, UIUpdateMode mode)
	{
		m_iconDefault = icon;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}

	Ref<Drawable> Button::getBackground(ButtonState state, sl_uint32 category)
	{
		if (category < m_nCategories) {
			return m_categories[category].properties[(int)state].background;
		} else {
			return sl_null;
		}
	}

	void Button::setBackground(const Ref<Drawable>& background, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (category < m_nCategories) {
			m_categories[category].properties[(int)state].background = background;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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
		if (category < m_nCategories) {
			return m_categories[category].properties[(int)state].border;
		} else {
			return sl_null;
		}
	}

	void Button::setBorder(const Ref<Pen>& pen, ButtonState state, sl_uint32 category, UIUpdateMode mode)
	{
		if (category < m_nCategories) {
			m_categories[category].properties[(int)state].border = pen;
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
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

	sl_bool Button::isUsingDefaultColorFilter()
	{
		return m_flagUseDefaultColorFilter;
	}

	void Button::setUsingDefaultColorFilter(sl_bool flag, UIUpdateMode mode)
	{
		m_flagUseDefaultColorFilter = flag;
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}

	void Button::setEnabled(sl_bool flagEnabled, UIUpdateMode mode)
	{
		if (isEnabled() != flagEnabled) {
			View::setEnabled(flagEnabled, UIUpdateMode::NoRedraw);
			_invalidateButtonState();
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	void Button::setPressedState(sl_bool flagState, UIUpdateMode mode)
	{
		if (isPressedState() != flagState) {
			View::setPressedState(flagState, UIUpdateMode::NoRedraw);
			_invalidateButtonState();
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	void Button::setHoverState(sl_bool flagState, UIUpdateMode mode)
	{
		if (isHoverState() != flagState) {
			View::setHoverState(flagState, UIUpdateMode::NoRedraw);
			_invalidateButtonState();
			if (mode == UIUpdateMode::Redraw) {
				invalidate();
			}
		}
	}

	void Button::onDraw(Canvas* canvas)
	{
		ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
		Color textColor = params.textColor;
		Ref<Drawable> icon = params.icon;
		if (textColor.isZero() || icon.isNull()) {
			const ColorMatrix* cm = sl_null;
			if (m_flagUseDefaultColorFilter) {
				switch (m_state) {
					case ButtonState::Hover:
						cm = &_g_button_colorMatrix_hover;
						break;
					case ButtonState::Pressed:
						cm = &_g_button_colorMatrix_pressed;
						break;
					case ButtonState::Disabled:
						cm = &_g_button_colorMatrix_disabled;
						break;
					default:
						break;
				}
			}
			if (textColor.isZero()) {
				textColor = m_textColorDefault;
				if (cm) {
					textColor = cm->transformColor(textColor);
				}
			}
			if (icon.isNull()) {
				icon = m_iconDefault;
				if (icon.isNotNull()) {
					if (cm) {
						icon = icon->filter(*cm);
					}
				}
			}
		}
		drawButtonContent(canvas, icon, m_text, textColor);
	}

	void Button::onDrawBackground(Canvas* canvas)
	{
		ButtonCategoryProperties& params = m_categories[m_category].properties[(int)m_state];
		Ref<Drawable> background = params.background;
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
				}
			}
		}
		
		if (m_flagUseDefaultColorFilter) {
			const ColorMatrix* cm = sl_null;
			switch (m_state) {
				case ButtonState::Hover:
					cm = &_g_button_colorMatrix_hover;
					break;
				case ButtonState::Pressed:
					cm = &_g_button_colorMatrix_pressed;
					break;
				case ButtonState::Disabled:
					cm = &_g_button_colorMatrix_disabled;
					break;
				default:
					break;
			}
			if (cm) {
				if (background.isNotNull()) {
					background = background->filter(*cm);
				}
			}
		}
		drawBackground(canvas, background);

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

	void Button::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		if (!flagVertical && !flagHorizontal) {
			return;
		}
		
		UISize size = measureContentSize();
		
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

	void Button::onKeyEvent(UIEvent* ev)
	{
		if (ev->getAction() == UIAction::KeyDown) {
			if (ev->getKeycode() == Keycode::Enter) {
				dispatchClick();
				ev->preventDefault();
			}
		}
	}

	void Button::onClick(UIEvent* ev)
	{
		dispatchOK(ev);
	}

	UISize Button::measureContentSize()
	{
		UISize size;
		UIRect rcIcon, rcText;
		layoutIconAndText(0, 0, size, rcIcon, rcText);
		return size;
	}

	void Button::layoutIconAndText(sl_ui_len widthFrame, sl_ui_len heightFrame, UISize& sizeContent, UIRect& frameIcon, UIRect& frameText)
	{
		sl_ui_pos widthIcon = m_iconSize.x + m_iconMarginLeft + m_iconMarginRight;
		if (widthIcon < 0) {
			widthIcon = 0;
		}
		sl_ui_pos heightIcon = m_iconSize.y + m_iconMarginTop + m_iconMarginBottom;
		if (heightIcon < 0) {
			heightIcon = 0;
		}
		
		UISize sizeText = measureText(m_text, getFont(), m_flagMultiLine);

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
			canvas->draw(rcIcon, icon);
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

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

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
