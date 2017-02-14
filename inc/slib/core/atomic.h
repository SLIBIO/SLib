#ifndef CHECKHEADER_SLIB_CORE_ATOMIC
#define CHECKHEADER_SLIB_CORE_ATOMIC

#include "definition.h"

#include "cpp.h"
#include "spin_lock.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Atomic
	{
	public:
		Atomic();

		Atomic(const T& value);

		Atomic(T&& value);

	public:
		Atomic<T>& operator=(const T& other);

		Atomic<T>& operator=(T&& other);

		operator T() const;

	protected:
		T m_value;
		SpinLock m_lock;

	};
	
	
	template <>
	class SLIB_EXPORT Atomic<sl_int32>
	{
	public:
		Atomic();

		Atomic(sl_int32 value);

	public:
		sl_int32 operator=(sl_int32 value);

		operator sl_int32 () const;

	public:
		sl_int32 increase();

		sl_int32 decrease();

		sl_int32 add(sl_int32 other);

		sl_bool waitZero(sl_int32 timeout = -1);

	private:
		volatile sl_int32 m_value;

	};
	
	typedef Atomic<sl_int32> AtomicInt32;
	
	
	template <class T>
	struct RemoveAtomic;
	
	template <class T>
	struct RemoveAtomic< Atomic<T> > { typedef T Type; };
	
	template <class T>
	struct PropertyTypeHelper< Atomic<T> >
	{
		typedef typename PropertyTypeHelper<T>::ArgType ArgType;
		typedef typename RemoveConstReference< typename PropertyTypeHelper<T>::RetType >::Type RetType;
	};
	
}

#include "detail/atomic.h"

#endif
