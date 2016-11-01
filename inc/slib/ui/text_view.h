#ifndef CHECKHEADER_SLIB_UI_TEXT_VIEW
#define CHECKHEADER_SLIB_UI_TEXT_VIEW

#include "definition.h"

#include "view.h"

#include "../core/queue.h"

SLIB_UI_NAMESPACE_BEGIN

enum class TextItemType
{
	Span = 0,
	View = 1
};

class SLIB_EXPORT TextItem : public Referable
{
public:
	TextItemType type;
	UIRect displayRegion;
	
};

class SLIB_EXPORT TextSpan : public TextItem
{
public:
	String text;
	Ref<Font> font;
	
};

class SLIB_EXPORT TextParagraph : public Referable
{
public:
	TextParagraph();
	
public:
	Queue< Ref<TextItem> > items;
	UIRect displayRegion;
	
};

class SLIB_EXPORT TextDocument : public Referable
{
	SLIB_DECLARE_OBJECT
	
public:
	TextDocument();
	
public:
	CList< Ref<TextParagraph> > paragraphs;
	
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

SLIB_UI_NAMESPACE_END

#endif
