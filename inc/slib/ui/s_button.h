#ifndef CHECKHEADER_SLIB_UI_S_BUTTON
#define CHECKHEADER_SLIB_UI_S_BUTTON

#include "definition.h"

#include "s_view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT SButtonCategoryProperties
{
public:
	Color textColor;
	Color backgroundColor;
	SafeRef<Drawable> background;
	SafeRef<Pen> border;
	SafeRef<Drawable> icon;
	
public:
	SButtonCategoryProperties();
};

class SLIB_EXPORT SButtonCategory
{
public:
	SButtonCategoryProperties properties[(int)(ButtonState::Count)];
};

class SLIB_EXPORT SButton : public SView
{
	SLIB_DECLARE_OBJECT
	
public:
	SButton(sl_uint32 nCategories = 2, SButtonCategory* categories = sl_null);
	
	~SButton();
	
public:
	String getText();
	
	virtual void setText(const String& text);
	
	
	Ref<Font> getFont();
	
	virtual void setFont(const Ref<Font>& font);
	
	
	sl_bool isDefaultButton();
	
	virtual void setDefaultButton(sl_bool flag);
	
public:
	sl_uint32 getCategoriesCount();
	
	sl_uint32 getCurrentCategory();
	
	void setCurrentCategory(sl_uint32 n);
	
	
	ButtonState getButtonState();
	
	
	const Size& getIconSize();
	
	virtual void setIconSize(const Size& size);
	
	void setIconSize(sl_real width, sl_real height);
	
	void setIconSize(sl_real size);
	
	
	Alignment getContentAlignment();
	
	virtual void setContentAlignment(Alignment align);
	
	
	Alignment getIconAlignment();
	
	virtual void setIconAlignment(Alignment align);
	
	
	Alignment getTextAlignment();
	
	virtual void setTextAlignment(Alignment align);
	
	
	sl_bool isTextBeforeIcon();
	
	virtual void setTextBeforeIcon(sl_bool flag);
	
	
	LayoutOrientation getLayoutOrientation();
	
	virtual void setLayoutOrientation(LayoutOrientation orientation);
	
	
	virtual void setIconMargin(sl_real left, sl_real top, sl_real right, sl_real bottom);
	
	void setIconMargin(sl_real margin);
	
	
	sl_real getIconMarginLeft();
	
	void setIconMarginLeft(sl_real margin);
	
	
	sl_real getIconMarginTop();
	
	void setIconMarginTop(sl_real margin);
	
	
	sl_real getIconMarginRight();
	
	void setIconMarginRight(sl_real margin);
	
	
	sl_real getIconMarginBottom();
	
	void setIconMarginBottom(sl_real margin);

	
	virtual void setTextMargin(sl_real left, sl_real top, sl_real right, sl_real bottom);
	
	void setTextMargin(sl_real margin);
	
	
	sl_real getTextMarginLeft();
	
	void setTextMarginLeft(sl_real margin);
	
	
	sl_real getTextMarginTop();
	
	void setTextMarginTop(sl_real margin);
	
	
	sl_real getTextMarginRight();
	
	void setTextMarginRight(sl_real margin);
	
	
	sl_real getTextMarginBottom();
	
	void setTextMarginBottom(sl_real margin);
	
	
	Color getTextColor(ButtonState state, sl_uint32 category = 0);
	
	virtual void setTextColor(const Color& color, ButtonState state, sl_uint32 category = 0);
	
	
	Color getBackgroundColor(ButtonState state, sl_uint32 category = 0);
	
	virtual void setBackgroundColor(const Color& color, ButtonState state, sl_uint32 category = 0);
	
	
	Ref<Drawable> getBackground(ButtonState state, sl_uint32 category = 0);
	
	virtual void setBackground(const Ref<Drawable>& background, ButtonState state, sl_uint32 category = 0);
	
	
	Ref<Drawable> getIcon(ButtonState state, sl_uint32 category = 0);
	
	virtual void setIcon(const Ref<Drawable>& icon, ButtonState state, sl_uint32 category = 0);
	
	
	Ref<Pen> getBorder(ButtonState state, sl_uint32 category = 0);
	
	virtual void setBorder(const Ref<Pen>& pen, ButtonState state, sl_uint32 category = 0);

	
	Color getBackgroundColor();
	
	// override
	void setBackgroundColor(const Color& color);
	
	Ref<Drawable> getBackground();
	
	// override
	void setBackground(const Ref<Drawable>& background);

public:
	// override
	Size measureContentSize(GraphicsContext* gc);
	
	// override
	void setEnabled(sl_bool flag);
	
	// override
	void setDownState(sl_bool flag);
	
	// override
	void setHoverState(sl_bool flag);
	
protected:
	// override
	void onDraw(Canvas* canvas);
	
	// override
	void onDrawBackground(Canvas* canvas);
	
	// override
	void onDrawBorder(Canvas* canvas);
	
protected:
	virtual void layoutIconAndText(GraphicsContext* gc, sl_real widthFrame, sl_real heightFrame, Size& sizeContent, Rectangle& frameIcon, Rectangle& frameText);
	
	virtual void drawContent(Canvas* canvas, const Ref<Drawable>& icon, const String& text, const Color& textColor);

private:
	Ref<Font> _getFont();
	
	void _invalidateButtonState();
	
private:
	SafeString m_text;
	SafeRef<Font> m_font;
	sl_bool m_flagDefaultButton;
	
private:
	ButtonState m_state;
	sl_uint32 m_category;
	
	Size m_iconSize;
	Alignment m_contentAlignment;
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
	
	SButtonCategory* m_categories;
	sl_uint32 m_nCategories;
	
};

SLIB_UI_NAMESPACE_END

#endif
