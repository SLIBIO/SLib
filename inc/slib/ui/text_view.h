#ifndef CHECKHEADER_SLIB_UI_TEXT_VIEW
#define CHECKHEADER_SLIB_UI_TEXT_VIEW

#include "definition.h"

#include "view.h"

#include "../core/linked_object.h"

namespace slib
{
	
	class SLIB_EXPORT TextView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		TextView();
		
		~TextView();

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
