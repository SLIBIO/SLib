#ifndef CHECKHEADER_SLIB_UI_CORE_COMMON
#define CHECKHEADER_SLIB_UI_CORE_COMMON

#include "../../../inc/slib/ui/core.h"

namespace slib
{
	class _UIDispatcher
	{
	public:
		static sl_bool addCallback(const Function<void()>& callback);
		
		static void processCallbacks();
		
		static sl_bool addDelayedCallback(const Function<void()>& callback, sl_reg& ptr);
		
		static void processDelayedCallback(sl_reg ptr);
		
		static void removeAllCallbacks();
		
	};	
}

#endif
