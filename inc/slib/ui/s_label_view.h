#ifndef CHECKHEADER_SLIB_UI_S_LABEL_VIEW
#define CHECKHEADER_SLIB_UI_S_LABEL_VIEW

#include "definition.h"
#include "s_view.h"

SLIB_UI_NAMESPACE_BEGIN
class SLIB_EXPORT SLabelView : public SView
{
	SLIB_DECLARE_OBJECT(SLabelView, SView)
public:
	SLabelView();
	
protected:
	String getText();
	virtual void setText(const String& text);
	
	Color getTextColor();
	virtual void setTextColor(const Color& color);
	
	Alignment getTextAlignment();
	virtual void setTextAlignment(Alignment align);

	Ref<Font> getFont();
	virtual void setFont(const Ref<Font>& font);
	
public:
	// override
	void onDraw(Canvas* canvas);
	
protected:
	String m_text;
	Color m_textColor;
	Alignment m_textAlignment;
	Ref<Font> m_font;
};
SLIB_UI_NAMESPACE_END

#endif
