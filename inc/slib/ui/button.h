#ifndef CHECKHEADER_SLIB_UI_BUTTON
#define CHECKHEADER_SLIB_UI_BUTTON

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT ButtonCategoryProperties
	{
	public:
		Color textColor;
		AtomicRef<Drawable> background;
		AtomicRef<Pen> border;
		AtomicRef<Drawable> icon;
		
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
		
		virtual void setText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isDefaultButton();
		
		virtual void setDefaultButton(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		sl_uint32 getCategoriesCount();
		
		sl_uint32 getCurrentCategory();
		
		void setCurrentCategory(sl_uint32 n, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		ButtonState getButtonState();
		
		
		sl_bool isMultiLine();
		
		void setMultiLine(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);

		const UISize& getIconSize();
		
		virtual void setIconSize(const UISize& size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setIconSize(sl_ui_len width, sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setIconSize(sl_ui_len size, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getIconWidth();
		
		void setIconWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getIconHeight();
		
		void setIconHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Alignment getGravity();
		
		virtual void setGravity(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getIconAlignment();
		
		virtual void setIconAlignment(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getTextAlignment();
		
		virtual void setTextAlignment(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isTextBeforeIcon();
		
		virtual void setTextBeforeIcon(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		LayoutOrientation getLayoutOrientation();
		
		virtual void setLayoutOrientation(LayoutOrientation orientation, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		virtual void setIconMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setIconMargin(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getIconMarginLeft();
		
		void setIconMarginLeft(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getIconMarginTop();
		
		void setIconMarginTop(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getIconMarginRight();
		
		void setIconMarginRight(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getIconMarginBottom();
		
		void setIconMarginBottom(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		virtual void setTextMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setTextMargin(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTextMarginLeft();
		
		void setTextMarginLeft(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTextMarginTop();
		
		void setTextMarginTop(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTextMarginRight();
		
		void setTextMarginRight(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getTextMarginBottom();
		
		void setTextMarginBottom(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Color getTextColor(ButtonState state, sl_uint32 category = 0);
		
		virtual void setTextColor(const Color& color, ButtonState state, sl_uint32 category = 0, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getTextColor();
		
		virtual void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Drawable> getIcon(ButtonState state, sl_uint32 category = 0);
		
		virtual void setIcon(const Ref<Drawable>& icon, ButtonState state, sl_uint32 category = 0, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getIcon();
		
		virtual void setIcon(const Ref<Drawable>& icon, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		Ref<Drawable> getBackground(ButtonState state, sl_uint32 category = 0);
		
		virtual void setBackground(const Ref<Drawable>& background, ButtonState state, sl_uint32 category = 0, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setBackground(const Color& backgroundColor, ButtonState state, sl_uint32 category = 0, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Drawable> getBackground();
		
		// override
		void setBackground(const Ref<Drawable>& background, UIUpdateMode mode = UIUpdateMode::Redraw);

		
		Ref<Pen> getBorder(ButtonState state, sl_uint32 category = 0);
		
		virtual void setBorder(const Ref<Pen>& pen, ButtonState state, sl_uint32 category = 0, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Pen> getBorder();
		
		// override
		void setBorder(const Ref<Pen>& pen, UIUpdateMode mode = UIUpdateMode::Redraw);

		void setBorder(sl_bool flagBorder, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		sl_bool isUsingDefaultColorFilter();
		
		void setUsingDefaultColorFilter(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		// override
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
		
		// override
		void setEnabled(sl_bool flagEnabled, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		// override
		void setPressedState(sl_bool flagState, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		// override
		void setHoverState(sl_bool flagState, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	protected:
		// override
		void onDraw(Canvas* canvas);
		
		// override
		void onDrawBackground(Canvas* canvas);
		
		// override
		void onDrawBorder(Canvas* canvas);
		
		// override
		void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);
		
		// override
		void onKeyEvent(UIEvent* ev);

		// override
		void onClick(UIEvent* ev);
		
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
		AtomicString m_text;
		sl_bool m_flagMultiLine;
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
		AtomicRef<Drawable> m_iconDefault;
		sl_bool m_flagUseDefaultColorFilter;

		ButtonCategory* m_categories;
		sl_uint32 m_nCategories;
		
		
	};

}

#endif
