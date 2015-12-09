#ifndef CHECKHEADER_SLIB_UI_SCREEN
#define CHECKHEADER_SLIB_UI_SCREEN

#include "definition.h"

#include "../core/object.h"
#include "../math/rectangle.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT Screen : public Object
{
	SLIB_DECLARE_OBJECT(Screen, Object)
protected:
	Screen() {}
	
public:
	virtual Rectangle getRegion() = 0;
};

SLIB_UI_NAMESPACE_END

#endif
