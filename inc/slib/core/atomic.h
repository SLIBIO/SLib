#ifndef CHECKHEADER_SLIB_CORE_ATOMIC
#define CHECKHEADER_SLIB_CORE_ATOMIC

#include "definition.h"

#include "cpp.h"
#include "spin_lock.h"

SLIB_NAMESPACE_BEGIN

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
class RemoveAtomic;

template <class T>
struct RemoveAtomic< Atomic<T> > { typedef T Type; };

template <class T>
struct PropertyTypeHelper< Atomic<T> >
{
	typedef typename PropertyTypeHelper<T>::ArgType ArgType;
	typedef typename RemoveConstReference< typename PropertyTypeHelper<T>::RetType >::Type RetType;
};


template <class T>
Atomic<T>::Atomic()
{
}

template <class T>
Atomic<T>::Atomic(const T& value)
: m_value(value)
{
}

template <class T>
Atomic<T>::Atomic(T&& value)
: m_value(Move(value))
{
}

template <class T>
Atomic<T>& Atomic<T>::operator=(const T& other)
{
	SLIB_ALIGN(8) char t[sizeof(T)];
	{
		SpinLocker lock(&m_lock);
		((T*)t)->T::T(Move(m_value));
		m_value = other;
	}
	((T*)t)->~T();
	return *this;
}

template <class T>
Atomic<T>& Atomic<T>::operator=(T&& other)
{
	SLIB_ALIGN(8) char t[sizeof(T)];
	{
		SpinLocker lock(&m_lock);
		((T*)t)->T::T(Move(m_value));
		m_value = Move(other);
	}
	((T*)t)->~T();
	return *this;
}

template <class T>
Atomic<T>::operator T() const
{
	SLIB_ALIGN(8) char t[sizeof(T)];
	{
		SpinLocker lock(&m_lock);
		((T*)t)->T::T(m_value);
	}
	return *((T*)t);
}

SLIB_NAMESPACE_END

#endif
