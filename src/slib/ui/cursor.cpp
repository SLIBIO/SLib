#include "../../../inc/slib/core/definition.h"

#include "../../../inc/slib/ui/cursor.h"

SLIB_NAMESPACE_BEGIN

#if !defined(SLIB_PLATFORM_IS_WIN32) && !defined(SLIB_PLATFORM_IS_OSX)

Ref<Cursor> Cursor::getArrow()
{
	return Ref<Cursor>::null();
}

Ref<Cursor> Cursor::getIBeam()
{
	return Ref<Cursor>::null();
}

Ref<Cursor> Cursor::getCross()
{
	return Ref<Cursor>::null();
}

Ref<Cursor> Cursor::getHand()
{
	return Ref<Cursor>::null();
}

Ref<Cursor> Cursor::getResizeLeftRight()
{
	return Ref<Cursor>::null();
}

Ref<Cursor> Cursor::getResizeUpDown()
{
	return Ref<Cursor>::null();
}

void Cursor::setCurrent(const Ref<Cursor>& cursor)
{
}

Ref<Cursor> Cursor::getCurrent()
{
	return Ref<Cursor>::null();
}

void Cursor::show()
{
}

void Cursor::hide()
{
}

#endif

SLIB_UI_NAMESPACE_END
