#ifndef CHECKHEADER_SLIB_CORE_ATOMIC
#define CHECKHEADER_SLIB_CORE_ATOMIC

#include "definition.h"

#include "base.h"

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT AtomicInt
{
private:
	sl_reg m_value;
	
public:
	SLIB_INLINE sl_reg getValue() const
	{
		return m_value;
	}

	SLIB_INLINE void setValue(sl_reg value)
	{
		m_value = value;
	}

	SLIB_INLINE sl_reg increase()
	{
		return Base::interlockedIncrement(&m_value);
	}
	
	SLIB_INLINE sl_reg decrease()
	{
		return Base::interlockedDecrement(&m_value);
	}

	SLIB_INLINE sl_reg add(sl_reg other)
	{
		return Base::interlockedAdd(&m_value, other);
	}
	
	sl_bool waitZero(sl_int32 timeout = -1);
	
};
SLIB_NAMESPACE_END

#endif//
