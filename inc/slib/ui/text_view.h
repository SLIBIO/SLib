#ifndef CHECKHEADER_SLIB_UI_TEXT_VIEW
#define CHECKHEADER_SLIB_UI_TEXT_VIEW

#include "definition.h"

#include "view.h"

#include "../core/linked_object.h"

namespace slib
{
/**********************************************
 SDM : Smart Document Model
 **********************************************/
	
	enum class SDM_ItemType
	{
		Text = 0,
		Image = 1,
		View = 10,
		Frame = 100
	};
	
	class SDM_Frame;
	
	class SLIB_EXPORT SDM_Item : public LinkedObject<SDM_Item>
	{
	protected:
		SDM_Item(SDM_ItemType type);
		
	public:
		SDM_ItemType getType();
		
		Ref<SDM_Frame> getFrame();
		
		sl_bool isNewLine();
		
		void setNewLine(sl_bool flag, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	protected:
		void _relayout(UIUpdateMode mode);
		
		void _redraw(UIUpdateMode mode);
		
	protected:
		SDM_ItemType m_type;
		AtomicWeakRef<SDM_Frame> m_frame;
		sl_bool m_flagNewLine;
		
		friend class SDM_Frame;
		
	};
	
	class SLIB_EXPORT SDM_Text : public SDM_Item
	{
	protected:
		SDM_Text();
		
	public:
		static Ref<SDM_Text> create(const Ref<Font>& font);
		
	public:
		String getText();
		
		void setText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Ref<Font> getFont();
		
		void setFont(const Ref<Font>& font, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getTextColor();
		
		void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getBackgroundColor();
		
		void setBackgroundColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		AtomicString m_text;
		AtomicRef<Font> m_font;
		Color m_textColor;
		Color m_backgroundColor;
		
	};
	
	class SLIB_EXPORT SDM_Image : public SDM_Item
	{
	protected:
		SDM_Image();
		
	public:
		static Ref<SDM_Image> create();
		
	public:
		Ref<Drawable> getSource();
		
		void setSource(const Ref<Drawable>& source, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		ScaleMode getScaleMode();
		
		void setScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode = UIUpdateMode::Redraw);
		
		Alignment getGravity();
		
		void setGravity(Alignment gravity, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		SizeMode getWidthMode();
		
		SizeMode getHeightMode();
		
		sl_ui_len getWidth();
		
		void setWidth(sl_ui_len width, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isWidthFilling();
		
		void setWidthFilling(UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_len getHeight();
		
		void setHeight(sl_ui_len height, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_bool isHeightFilling();
		
		void setHeightFilling(UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		AtomicRef<Drawable> m_source;
		ScaleMode m_scaleMode;
		Alignment m_gravity;
		SizeMode mode;
		sl_ui_len m_width;
		sl_ui_len m_height;
		
	};
	
	class SLIB_EXPORT SDM_View : public SDM_Item
	{
	protected:
		SDM_View();
		
	public:
		static Ref<SDM_View> create(const Ref<View>& view);
		
	public:
		Ref<View> getView();
		
		void setView(const Ref<View>& view, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	protected:
		AtomicRef<View> m_view;
		
	};
	
	class SLIB_EXPORT SDM_Frame : public SDM_Item
	{
	protected:
		SDM_Frame();
		
	public:
		static Ref<SDM_Frame> create();
		
	public:
		Ref<Drawable> getBackground();
		
		void setBackground(const Ref<Drawable>& background, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setBackgroundColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		ScaleMode getBackgroundScaleMode();
		
		void setBackgroundScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode = UIUpdateMode::Redraw);
		
		Alignment getBackgroundAlignment();
		
		void setBackgroundAlignment(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getAlignment();
		
		void setAlignment(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getGravity();
		
		void setGravity(Alignment gravity, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getMarginLeft();
		
		void setMarginLeft(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getMarginTop();
		
		void setMarginTop(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getMarginRight();
		
		void setMarginRight(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getMarginBottom();
		
		void setMarginBottom(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setMargin(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setMargin(sl_ui_pos margin, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getPaddingLeft();
		
		void setPaddingLeft(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getPaddingTop();
		
		void setPaddingTop(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getPaddingRight();
		
		void setPaddingRight(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		sl_ui_pos getPaddingBottom();
		
		void setPaddingBottom(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPadding(sl_ui_pos left, sl_ui_pos top, sl_ui_pos right, sl_ui_pos bottom, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		void setPadding(sl_ui_pos padding, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	protected:
		void _relayout(SDM_Item* item, UIUpdateMode mode);
		
		void _redraw(SDM_Item* item);
		
	protected:
		LinkedObjectList<SDM_Item> m_items;
		
		AtomicRef<Drawable> m_background;
		ScaleMode m_backgroundScaleMode;
		Alignment m_backgroundAlignment;
		
		Alignment m_alignment;
		Alignment m_gravity;
		
		sl_ui_pos m_marginLeft;
		sl_ui_pos m_marginTop;
		sl_ui_pos m_marginRight;
		sl_ui_pos m_marginBottom;
		
		sl_ui_pos m_paddingLeft;
		sl_ui_pos m_paddingTop;
		sl_ui_pos m_paddingRight;
		sl_ui_pos m_paddingBottom;
		
		friend class SDM_Item;
		friend class SDM_Document;
		
	};
	
	class SLIB_EXPORT SDM_Document : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		SDM_Document();
		
	public:
		static Ref<SDM_Document> create();
		
	public:
		Ref<SDM_Frame> getBody();
		
		Ref<View> getView();
		
		void setView(const Ref<View>& view);
		
	protected:
		AtomicRef<SDM_Frame> m_body;
		AtomicRef<View> m_view;
		
		friend class SDM_Frame;
		
	};
	
	class SLIB_EXPORT TextView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TextView();
		
	public:
		sl_bool isEditable();
		
		void setEditable(sl_bool flagEditable);
		
	protected:
		// override
		void onDraw(Canvas* canvas);
		
		// override
		void onMouseEvent(UIEvent* ev);
		
		// override
		void onKeyEvent(UIEvent* event);
		
	protected:
		sl_bool m_flagEditable;
		
	};
}

#endif
