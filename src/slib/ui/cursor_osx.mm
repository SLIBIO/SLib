#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "../../../inc/slib/ui/cursor.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

class _OSX_Cursor : public Cursor
{
public:
	NSCursor* m_cursor;
	
public:
	static Ref<_OSX_Cursor> create(NSCursor* cursor)
	{
		Ref<_OSX_Cursor> ret;
		if (cursor != nil) {
			ret = new _OSX_Cursor;
			if (ret.isNotNull()) {
				ret->m_cursor = cursor;
			}
		}
		return ret;
	}
};

Ref<Cursor> UIPlatform::createCursor(NSCursor* cursor)
{
	return _OSX_Cursor::create(cursor);
}

NSCursor* UIPlatform::getCursorHandle(const Ref<Cursor>& cursor)
{
	if (cursor.isNull()) {
		return nil;
	}
	_OSX_Cursor* c = (_OSX_Cursor*)(cursor.ptr);
	return c->m_cursor;
}

Ref<Cursor> Cursor::getArrow()
{
	return UIPlatform::createCursor([NSCursor arrowCursor]);
}

Ref<Cursor> Cursor::getIBeam()
{
	return UIPlatform::createCursor([NSCursor IBeamCursor]);
}

Ref<Cursor> Cursor::getCross()
{
	return UIPlatform::createCursor([NSCursor crosshairCursor]);
}

Ref<Cursor> Cursor::getHand()
{
	return UIPlatform::createCursor([NSCursor pointingHandCursor]);
}

Ref<Cursor> Cursor::getResizeLeftRight()
{
	return UIPlatform::createCursor([NSCursor resizeLeftRightCursor]);
}

Ref<Cursor> Cursor::getResizeUpDown()
{
	return UIPlatform::createCursor([NSCursor resizeUpDownCursor]);
}

void Cursor::setCurrent(const Ref<Cursor>& cursor)
{
	if (cursor.isNull()) {
		return;
	}
	_OSX_Cursor* c = (_OSX_Cursor*)(cursor.ptr);
	[c->m_cursor set];
}

Ref<Cursor> Cursor::getCurrent()
{
	return UIPlatform::createCursor([NSCursor currentCursor]);
}

void Cursor::show()
{
	[NSCursor unhide];
}

void Cursor::hide()
{
	[NSCursor hide];
}

SLIB_UI_NAMESPACE_END

#endif
