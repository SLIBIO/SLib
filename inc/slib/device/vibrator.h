#ifndef CHECKHEADER_SLIB_DEVICE_VIBRATOR
#define CHECKHEADER_SLIB_DEVICE_VIBRATOR

#include "definition.h"

#include "../core/object.h"

namespace slib
{
	
	class SLIB_EXPORT Vibrator
	{
	public:
		static sl_bool vibrate(sl_int32 millisec = 500);

	};

}

#endif
