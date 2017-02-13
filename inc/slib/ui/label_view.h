#ifndef CHECKHEADER_SLIB_UI_LABEL_VIEW
#define CHECKHEADER_SLIB_UI_LABEL_VIEW

#include "definition.h"

#include "view.h"

#include "../graphics/text.h"

namespace slib
{
	struct TextLine {
		String16 text;
		sl_ui_len width;
		sl_ui_len height;
	};
	
	class SLIB_EXPORT LabelView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		LabelView();
		
		~LabelView();
		
	public:
		String getText();
		
		virtual void setText(const String& text, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Color getTextColor();
		
		virtual void setTextColor(const Color& color, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		Alignment getGravity();
		
		virtual void setGravity(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		MultiLineMode getMultiLineMode();
		
		virtual void setMultiLineMode(MultiLineMode multiLineMode, UIUpdateMode updateMode = UIUpdateMode::Redraw);
		
	public:
		// override
		Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
		
	protected:
		// override
		void onDraw(Canvas* canvas);
		
		// override
		void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);
		
	private:
		void _setText_NW(const String& text);
		
		void _setTextColor_NW(const Color& color);
		
		void _setTextAlignment_NW(Alignment align);
		
		// override
		void _setFont_NW(const Ref<Font>& font);
		
		// override
		void _setBorder_NW(sl_bool flag);
		
		// override
		void _setBackgroundColor_NW(const Color& color);
		
	protected:
		AtomicString m_text;
		Color m_textColor;
		Alignment m_textAlignment;
		MultiLineMode m_multiLineMode;
		
		SimpleTextBox m_textBox;
		
	};
}

#endif
