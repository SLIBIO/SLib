#ifndef CHECKHEADER_SLIB_UI_SCREEN
#define CHECKHEADER_SLIB_UI_SCREEN

#include "definition.h"

#include "../core/object.h"
#include "../math/rectangle.h"

namespace slib
{
	class SLIB_EXPORT Screen : public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		virtual UIRect getRegion() = 0;
		
	};
}

#endif
