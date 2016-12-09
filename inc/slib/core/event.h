#ifndef CHECKHEADER_SLIB_CORE_EVENT
#define CHECKHEADER_SLIB_CORE_EVENT

#include "definition.h"

#include "ref.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Event : public Referable
{
	SLIB_DECLARE_OBJECT
	
protected:
	Event();
	
	~Event();

public:
	static Ref<Event> create(sl_bool flagAutoReset = sl_true);
	
public:
	void set();
	
	void reset();
	
	// milliseconds. negative means INFINITE
	sl_bool wait(sl_int32 timeout = -1);

protected:
	virtual void __set() = 0;
	
	virtual void __reset() = 0;
	
	virtual sl_bool __wait(sl_int32 timeout) = 0;

};

SLIB_NAMESPACE_END

#endif
