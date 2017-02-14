#ifndef CHECKHEADER_SLIB_CORE_DETAIL_SAFE_STATIC
#define CHECKHEADER_SLIB_CORE_DETAIL_SAFE_STATIC

#include "../safe_static.h"

namespace slib
{
	
	template <class T>
	class _SafeStaticDestructor
	{
	public:
		T* object;
		volatile sl_bool flagFreed;

	public:
		SLIB_INLINE _SafeStaticDestructor(T* p)
		: object(p), flagFreed(sl_false)
		{
		}

		~_SafeStaticDestructor()
		{
			flagFreed = sl_true;
			object->~T();
		}

	};

}

#endif

