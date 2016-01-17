#ifndef CHECKHEADER_SLIB_DEVICE_VIBRATOR
#define CHECKHEADER_SLIB_DEVICE_VIBRATOR

#include "definition.h"

#include "../core/object.h"

SLIB_DEVICE_NAMESPACE_BEGIN

class SLIB_EXPORT Vibrator : public Object
{
	SLIB_DECLARE_OBJECT(Vibrator, Object);
	
public:
	static Ref<Vibrator> create();
	
public:
	virtual sl_bool vibrate(sl_int32 millisec) = 0;
	
	virtual sl_bool cancel() = 0;
};

SLIB_DEVICE_NAMESPACE_END

#endif
