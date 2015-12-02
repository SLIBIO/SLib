#ifndef CHECKHEADER_SLIB_UI_LABEL_VIEW
#define CHECKHEADER_SLIB_UI_LABEL_VIEW

#include "definition.h"
#include "view.h"

#include "../core/callback.h"

SLIB_UI_NAMESPACE_BEGIN
class SLIB_EXPORT LabelView : public View
{
	SLIB_DECLARE_OBJECT(LabelView, View)
public:
	LabelView();
	
public:
	String getText();
	virtual void setText(const String& text);
	
	Color getTextColor();
	virtual void setTextColor(const Color& color);
	
	Alignment getTextAlignment();
	virtual void setTextAlignment(Alignment align);
	
	Ref<Font> getFont();
	virtual void setFont(const Ref<Font>& font);
	
public:
	Color getBackgroundColor();
	virtual void setBackgroundColor(const Color& color);
	
	sl_bool isBorder();
	virtual void setBorder(sl_bool flag);
	
public:
	SLIB_PROPERTY_INLINE(Ref<Runnable>, OnClick)
	
public:
	// override
	void onClick();

protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
protected:
	String m_text;
	sl_bool m_flagBorder;
	Alignment m_textAlignment;
	Color m_textColor;
	Color m_backgroundColor;
	Ref<Font> m_font;
	Ref<FontInstance> m_fontInstance;
};
SLIB_UI_NAMESPACE_END

#endif
