#ifndef CHECKHEADER_SLIB_MOBILE_VIBRATOR
#define CHECKHEADER_SLIB_MOBILE_VIBRATOR

#include "definition.h"
#include "../core/object.h"
#include "../math/geograph.h"

SLIB_MOBILE_NAMESPACE_BEGIN

class SLIB_EXPORT Vibrator : public Object
{
public:
	static sl_bool vibrate(sl_int32 millisec);
	static sl_bool cancel();
};

SLIB_MOBILE_NAMESPACE_END

#endif
