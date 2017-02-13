#include "../../../inc/slib/ui/cursor.h"

namespace slib
{
	SLIB_DEFINE_OBJECT(Cursor, Object)
	
#if !defined(SLIB_PLATFORM_IS_WIN32) && !defined(SLIB_PLATFORM_IS_OSX)
	
	Ref<Cursor> Cursor::getArrow()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getIBeam()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getCross()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getHand()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getResizeLeftRight()
	{
		return sl_null;
	}
	
	Ref<Cursor> Cursor::getResizeUpDown()
	{
		return sl_null;
	}
	
	void Cursor::setCurrent(const Ref<Cursor>& cursor)
	{
	}
	
	Ref<Cursor> Cursor::getCurrent()
	{
		return sl_null;
	}
	
	void Cursor::show()
	{
	}
	
	void Cursor::hide()
	{
	}
	
#endif
}
