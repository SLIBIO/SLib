#ifndef CHECKHEADER_SLIB_UI_LABEL_VIEW
#define CHECKHEADER_SLIB_UI_LABEL_VIEW

#include "definition.h"

#include "view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT LabelView : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	LabelView();
	
public:
	String getText();
	
	virtual void setText(const String& text, sl_bool flagRedraw = sl_true);
	
	Color getTextColor();
	
	virtual void setTextColor(const Color& color, sl_bool flagRedraw = sl_true);
	
	Alignment getTextAlignment();
	
	virtual void setTextAlignment(Alignment align, sl_bool flagRedraw = sl_true);
	
public:
	// override
	Ref<ViewInstance> createNativeWidget(ViewInstance* parent);
	
protected:
	// override
	void onDraw(Canvas* canvas);
	
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
	SafeString m_text;
	Color m_textColor;
	Alignment m_textAlignment;

};

SLIB_UI_NAMESPACE_END

#endif
