#ifndef CHECKHEADER_SLIB_CORE_DISPATCH
#define CHECKHEADER_SLIB_CORE_DISPATCH

#include "definition.h"

#include "timer.h"

namespace slib
{
	
	class DispatchLoop;
	
	class SLIB_EXPORT Dispatch
	{
	public:
		static sl_bool dispatch(const Ref<DispatchLoop>& loop, const Function<void()>& task);

		static sl_bool dispatch(const Function<void()>& task);

		static sl_bool setTimeout(const Ref<DispatchLoop>& loop, const Function<void()>& task, sl_uint64 delay_ms);

		static sl_bool setTimeout(const Function<void()>& task, sl_uint64 delay_ms);
	
		static Ref<Timer> setInterval(const Ref<DispatchLoop>& loop, const Function<void()>& task, sl_uint64 interval_ms);

		static Ref<Timer> setInterval(const Function<void()>& task, sl_uint64 interval_ms);

	};
	
	class SLIB_EXPORT Dispatcher : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		Dispatcher();

		~Dispatcher();

	public:
		virtual sl_bool dispatch(const Function<void()>& callback, sl_uint64 delay_ms = 0) = 0;

	};

}

#endif
