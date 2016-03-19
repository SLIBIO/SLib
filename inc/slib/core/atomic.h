#ifndef CHECKHEADER_SLIB_CORE_ATOMIC
#define CHECKHEADER_SLIB_CORE_ATOMIC

#include "definition.h"

#include "base.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT AtomicInt
{
public:
	sl_reg value;
	
public:
	sl_reg increase();
	
	sl_reg decrease();

	sl_reg add(sl_reg other);
	
	sl_bool waitZero(sl_int32 timeout = -1);
	
};

SLIB_NAMESPACE_END

#endif//
