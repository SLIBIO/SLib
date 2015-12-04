#ifndef CHECKHEADER_SLIB_DEVICE_VIBRATOR
#define CHECKHEADER_SLIB_DEVICE_VIBRATOR

#include "definition.h"

#include "../core/object.h"

SLIB_DEVICE_NAMESPACE_BEGIN

class SLIB_EXPORT Vibrator : public Object
{
public:
	static sl_bool vibrate(sl_int32 millisec);
	static sl_bool cancel();
};

SLIB_DEVICE_NAMESPACE_END

#endif
