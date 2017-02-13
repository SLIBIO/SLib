#ifndef CHECKHEADER_SLIB_UI_CURSOR
#define CHECKHEADER_SLIB_UI_CURSOR

#include "definition.h"

#include "../core/object.h"

namespace slib
{
	class SLIB_EXPORT Cursor : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		static Ref<Cursor> getArrow();
		
		static Ref<Cursor> getIBeam();
		
		static Ref<Cursor> getCross();
		
		static Ref<Cursor> getHand();
		
		static Ref<Cursor> getResizeLeftRight();
		
		static Ref<Cursor> getResizeUpDown();
		
	public:
		static void setCurrent(const Ref<Cursor>& cursor);
		
		static Ref<Cursor> getCurrent();
		
	public:
		static void show();
		
		static void hide();
		
	};	
}

#endif
