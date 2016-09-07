#ifndef CHECKHEADER_SLIB_UI_BUTTON
#define CHECKHEADER_SLIB_UI_BUTTON

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT ButtonCategoryProperties
{
public:
	Color textColor;
	Color backgroundColor;
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
	
	
	const Size& getIconSize();
	
	virtual void setIconSize(const Size& size, sl_bool flagRedraw = sl_true);
	
	void setIconSize(sl_real width, sl_real height, sl_bool flagRedraw = sl_true);
	
	void setIconSize(sl_real size, sl_bool flagRedraw = sl_true);
	
	sl_real getIconWidth();
	
	void setIconWidth(sl_real width, sl_bool flagRedraw = sl_true);
	
	sl_real getIconHeight();
	
	void setIconHeight(sl_real height, sl_bool flagRedraw = sl_true);
	
	
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
	
	
	virtual void setIconMargin(sl_real left, sl_real top, sl_real right, sl_real bottom, sl_bool flagRedraw = sl_true);
	
	void setIconMargin(sl_real margin, sl_bool flagRedraw = sl_true);
	
	sl_real getIconMarginLeft();
	
	void setIconMarginLeft(sl_real margin, sl_bool flagRedraw = sl_true);
	
	sl_real getIconMarginTop();
	
	void setIconMarginTop(sl_real margin, sl_bool flagRedraw = sl_true);
	
	sl_real getIconMarginRight();
	
	void setIconMarginRight(sl_real margin, sl_bool flagRedraw = sl_true);
	
	sl_real getIconMarginBottom();
	
	void setIconMarginBottom(sl_real margin, sl_bool flagRedraw = sl_true);
	
	
	virtual void setTextMargin(sl_real left, sl_real top, sl_real right, sl_real bottom, sl_bool flagRedraw = sl_true);
	
	void setTextMargin(sl_real margin, sl_bool flagRedraw = sl_true);
	
	sl_real getTextMarginLeft();
	
	void setTextMarginLeft(sl_real margin, sl_bool flagRedraw = sl_true);
	
	sl_real getTextMarginTop();
	
	void setTextMarginTop(sl_real margin, sl_bool flagRedraw = sl_true);
	
	sl_real getTextMarginRight();
	
	void setTextMarginRight(sl_real margin, sl_bool flagRedraw = sl_true);
	
	sl_real getTextMarginBottom();
	
	void setTextMarginBottom(sl_real margin, sl_bool flagRedraw = sl_true);
	
	
	Color getTextColor(ButtonState state, sl_uint32 category = 0);
	
	virtual void setTextColor(const Color& color, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	Color getTextColor();
	
	virtual void setTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	void resetStateTextColors(sl_bool flagRedraw = sl_true);
	
	
	Ref<Drawable> getIcon(ButtonState state, sl_uint32 category = 0);
	
	virtual void setIcon(const Ref<Drawable>& icon, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getIcon();
	
	virtual void setIcon(const Ref<Drawable>& icon, sl_bool flagRedraw = sl_true);
	
	void resetStateIcons(sl_bool flagRedraw = sl_true);
	
	
	Color getBackgroundColor(ButtonState state, sl_uint32 category = 0);
	
	virtual void setBackgroundColor(const Color& color, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);

	Color getBackgroundColor();
	
	// override
	void setBackgroundColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	void resetStateBackgroundColors(sl_bool flagRedraw = sl_true);
	
	
	Ref<Drawable> getBackground(ButtonState state, sl_uint32 category = 0);
	
	virtual void setBackground(const Ref<Drawable>& background, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	Ref<Drawable> getBackground();
	
	// override
	void setBackground(const Ref<Drawable>& background, sl_bool flagRedraw = sl_true);

	void resetStateBackgrounds(sl_bool flagRedraw = sl_true);
	
	
	Ref<Pen> getBorder(ButtonState state, sl_uint32 category = 0);
	
	virtual void setBorder(const Ref<Pen>& pen, ButtonState state, sl_uint32 category = 0, sl_bool flagRedraw = sl_true);
	
	Ref<Pen> getBorder();
	
	// override
	void setBorder(const Ref<Pen>& pen, sl_bool flagRedraw = sl_true);
	
	void resetStateBorders(sl_bool flagRedraw = sl_true);
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
	// override
	Size measureContentSize(GraphicsContext* gc);
	
	// override
	void setEnabled(sl_bool flagEnabled, sl_bool flagRedraw = sl_true);
	
	// override
	void setDownState(sl_bool flagState, sl_bool flagRedraw = sl_true);
	
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
	virtual void layoutIconAndText(GraphicsContext* gc, sl_real widthFrame, sl_real heightFrame, Size& sizeContent, Rectangle& frameIcon, Rectangle& frameText);
	
	virtual void drawContent(Canvas* canvas, const Ref<Drawable>& icon, const String& text, const Color& textColor);
	
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
	
	Size m_iconSize;
	Alignment m_gravity;
	Alignment m_iconAlignment;
	Alignment m_textAlignment;
	sl_bool m_flagTextBeforeIcon;
	LayoutOrientation m_layoutOrientation;
	sl_real m_iconMarginLeft;
	sl_real m_iconMarginTop;
	sl_real m_iconMarginRight;
	sl_real m_iconMarginBottom;
	sl_real m_textMarginLeft;
	sl_real m_textMarginTop;
	sl_real m_textMarginRight;
	sl_real m_textMarginBottom;
	
	Color m_textColorDefault;
	SafeRef<Drawable> m_iconDefault;
	
	ButtonCategory* m_categories;
	sl_uint32 m_nCategories;
	
};


SLIB_UI_NAMESPACE_END

#endif
