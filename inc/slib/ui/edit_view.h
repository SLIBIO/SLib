#ifndef CHECKHEADER_SLIB_UI_EDIT_VIEW
#define CHECKHEADER_SLIB_UI_EDIT_VIEW

#include "definition.h"

#include "view.h"

#include "../core/callback.h"

SLIB_UI_NAMESPACE_BEGIN

class EditView;

class SLIB_EXPORT IEditViewListener
{
public:
	virtual String onChange(EditView* edit, const String& newValue) = 0;
	
};

class SLIB_EXPORT EditView : public View
{
	SLIB_DECLARE_OBJECT(EditView, View)
public:
	EditView();
	
public:
	String getText();
	
	virtual void setText(const String& text);

	
	sl_bool isBorder();
	
	virtual void setBorder(sl_bool flag);
	
	
	Alignment getTextAlignment();
	
	virtual void setTextAlignment(Alignment align);
	
	
	String getHintText();
	
	virtual void setHintText(const String& str);
	
	
	sl_bool isReadOnly();
	
	virtual void setReadOnly(sl_bool flag);
	
	
	sl_bool isMultiLine();

	virtual void setMultiLine(sl_bool flag);
	
	
	Color getTextColor();
	
	virtual void setTextColor(const Color& color);
	
	
	Color getBackgroundColor();
	
	virtual void setBackgroundColor(const Color& color);
	
	
	Ref<Font> getFont();
	
	virtual void setFont(const Ref<Font>& font);
	
public:
	SLIB_PTR_PROPERTY(IEditViewListener, Listener)
	
	SLIB_REF_PROPERTY(Runnable, EnterAction)
	
protected:
	// override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
protected:
	virtual String onChange(const String& newValue);

	virtual void onEnterAction();
	
public:
	// override
	void dispatchKeyEvent(UIEvent* ev);
	
	String dispatchChange(const String& newValue);
	
	void dispatchEnterAction();

protected:
	SafeString m_text;
	sl_bool m_flagBorder;
	Alignment m_textAlignment;
	SafeString m_hintText;
	sl_bool m_flagReadOnly;
	sl_bool m_flagMultiLine;
	Color m_textColor;
	Color m_backgroundColor;
	
	SafeRef<Font> m_font;
	SafeRef<FontInstance> m_fontInstance;
	
};

class PasswordView : public EditView
{
	SLIB_DECLARE_OBJECT(PasswordView, EditView)
public:
	PasswordView();
	
public:
	sl_bool isMultiLine();
	
	virtual void setMultiLine(sl_bool flag);

protected:
    // override
	Ref<ViewInstance> createInstance(ViewInstance* parent);
	
};

class TextArea : public EditView
{
	SLIB_DECLARE_OBJECT(TextArea, EditView)
public:
	TextArea();
	
public:
	sl_bool isMultiLine();
	
	virtual void setMultiLine(sl_bool flag);
	
protected:
    // override
	Ref<ViewInstance> createInstance(ViewInstance* parent);

};

SLIB_UI_NAMESPACE_END

#endif
