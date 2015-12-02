#ifndef CHECKHEADER_SLIB_UI_CURSOR
#define CHECKHEADER_SLIB_UI_CURSOR

#include "definition.h"

#include "../core/object.h"

SLIB_UI_NAMESPACE_BEGIN
class SLIB_EXPORT Cursor : public Object
{
	SLIB_DECLARE_OBJECT(Cursor, Object)
protected:
	Cursor();
	
public:
	static Ref<Cursor> getArrow();
	static Ref<Cursor> getIBeam();
	static Ref<Cursor> getCross();
	static Ref<Cursor> getHand();
	static Ref<Cursor> getResizeLeftRight();
	static Ref<Cursor> getResizeUpDown();
	
	static void setCurrent(const Ref<Cursor>& cursor);
	static Ref<Cursor> getCurrent();
	
	static void show();
	static void hide();
};
SLIB_UI_NAMESPACE_END

#endif
