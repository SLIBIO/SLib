#include "../../../inc/slib/device/vibrator.h"

#if defined(SLIB_PLATFORM_IS_DESKTOP)

namespace slib
{

	sl_bool Vibrator::vibrate(sl_int32 millisec)
	{
		return sl_false;
	}

}

#endif
