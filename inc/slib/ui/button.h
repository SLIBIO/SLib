#ifndef CHECKHEADER_SLIB_UI_BUTTON
#define CHECKHEADER_SLIB_UI_BUTTON

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT ButtonCategoryProperties
{
public:
	Color textColor;
	SafeRef<Drawable> background;
	SafeRef<Pen> border;
	SafeRef<Drawable> icon;
	
public:
	ButtonCategoryProperties();
};

class SLIB_EXPORT ButtonCategory
{
public:
	ButtonCategoryProperties properties[(int)(ButtonState::Count)];
};

class SLIB_EXPORT Button : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	// 2 categories
	Button();
	
	Button(sl_uint32 nCategories, ButtonCategory* categories = sl_null);
	
	~Button();
	
public:
	String getText();
	
	virtual void setText(const String& text, sl_bool flagRedraw = sl_true);
	
	sl_bool isDefaultButton();
	
	virtual void setDefaultButton(sl_bool flag, sl_bool flagRedraw = sl_true);
	
	
	sl_uint32 getCategoriesCount();
	
	sl_uint32 getCurrentCategory();
	
	void setCurrentCategory(sl_uint32 n, sl_bool flagRedraw = sl_true);
	
	
	ButtonState getButtonState();
	
	
	const UISize& getIconSize();
	
	virtual void setIconSize(const UISize& size, sl_bool flagRedraw = sl_true);
	
	void setIconSize(sl_ui_len width, sl_ui_len height, sl_bool flagRedraw = sl_true);
	
	void setIconSize(sl_ui_len size, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getIconWidth();
	
	void setIconWidth(sl_ui_len width, sl_bool flagRedraw = sl_true);
	
	sl_ui_len getIconHeight();
	
	void setIconHeight(sl_ui_len height, sl_bool flagRedraw = sl_true);
	
	
	Alignment getGravity();
	
	virtual void setGravity(Alignment align, sl_bool flagRedraw = sl_true);
	
	Alignment getIconAlignment();
	
	virtual void setIconAlignment(Alignment align, sl_bool flagRedraw = sl_true);
	
	Alignment getTextAlignment();
	
	virtual void setTextAlignment(Alignment align, sl_bool flagRedraw = sl_true);
	
	sl_bool isTextBeforeIcon();
	
	virtual void setTextBeforeIcon(sl_bool flag, sl_bool flagRedraw = sl_true);
	
	LayoutOrientation getLayoutOrientation();
	
	virtual void setLayoutOrientation(LayoutOrientation orientation, sl_bool flagRedraw = sl_true);
	
	
	virtual void setIconMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw = sl_true);
	
	void setIconMargin(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getIconMarginLeft();
	
	void setIconMarginLeft(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getIconMarginTop();
	
	void setIconMarginTop(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getIconMarginRight();
	
	void setIconMarginRight(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getIconMarginBottom();
	
	void setIconMarginBottom(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	
	virtual void setTextMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, sl_bool flagRedraw = sl_true);
	
	void setTextMargin(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getTextMarginLeft();
	
	void setTextMarginLeft(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getTextMarginTop();
	
	void setTextMarginTop(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getTextMarginRight();
	
	void setTextMarginRight(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	sl_ui_pos getTextMarginBottom();
	
	void setTextMarginBottom(sl_ui_pos margin, sl_bool flagRedraw = sl_true);
	
	
	Color getTextColor(ButtonState state, sl_uint32 category = 0);
	
	virtual void setTextColor(const Color& color, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	Color getTextColor();
	
	virtual void setTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	
	Ref<Drawable> getIcon(ButtonState state, sl_uint32 category = 0);
	
	virtual void setIcon(const Ref<Drawable>& icon, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getIcon();
	
	virtual void setIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	
	Ref<Drawable> getBackground(ButtonState state, sl_uint32 category = 0);
	
	virtual void setBackground(const Ref<Drawable>& background, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	void setBackground(const Color& backgroundColor, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getBackground();
	
	// override
	void setBackground(const Ref<Drawable>& background, sl_bool flagRedraw = sl_true);

	
	Ref<Pen> getBorder(ButtonState state, sl_uint32 category = 0);
	
	virtual void setBorder(const Ref<Pen>& pen, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	Ref<Pen> getBorder();
	
	// override
	void setBorder(const Ref<Pen>& pen, sl_bool flagRedraw = sl_true);

	void setBorder(sl_bool flagBorder, sl_bool flagRedraw = sl_true);
	
	
	sl_bool isUsingDefaultColorFilter();
	
	void setUsingDefaultColorFilter(sl_bool flag, sl_bool flagRedraw = sl_true);
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	// override
	void setEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	// override
	void setPressedState(sl_bool flagState, sl_bool flagRedraw = sl_true);
	
	// override
	void setHoverState(sl_bool flagState, sl_bool flagRedraw = sl_true);
	
protected:
	// override
	void onDraw(Canvas* canvas);
	
	// override
	void onDrawBackground(Canvas* canvas);
	
	// override
	void onDrawBorder(Canvas* canvas);
	
	// override
	void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);
	
protected:
	virtual UISize measureContentSize();

	virtual void layoutIconAndText(sl_ui_len widthFrame, sl_ui_len heightFrame, UISize& sizeContent, UIRect& frameIcon, UIRect& frameText);
	
	virtual void drawButtonContent(Canvas* canvas, const Ref<Drawable>& icon, const String& text, const Color& textColor);
	
private:
	void _invalidateButtonState();
	
public:
	virtual void _setText_NW(const String& text);
	
	virtual void _setDefaultButton_NW(sl_bool flag);

	// override
	void _setFont_NW(const Ref<Font>& font);

private:
	SafeString m_text;
	sl_bool m_flagDefaultButton;

	ButtonState m_state;
	sl_uint32 m_category;
	
	UISize m_iconSize;
	Alignment m_gravity;
	Alignment m_iconAlignment;
	Alignment m_textAlignment;
	sl_bool m_flagTextBeforeIcon;
	LayoutOrientation m_layoutOrientation;
	sl_ui_pos m_iconMarginLeft;
	sl_ui_pos m_iconMarginTop;
	sl_ui_pos m_iconMarginRight;
	sl_ui_pos m_iconMarginBottom;
	sl_ui_pos m_textMarginLeft;
	sl_ui_pos m_textMarginTop;
	sl_ui_pos m_textMarginRight;
	sl_ui_pos m_textMarginBottom;
	
	Color m_textColorDefault;
	SafeRef<Drawable> m_iconDefault;
	sl_bool m_flagUseDefaultColorFilter;

	ButtonCategory* m_categories;
	sl_uint32 m_nCategories;
	
	
};


SLIB_UI_NAMESPACE_END

#endif
