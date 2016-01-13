#ifndef CHECKHEADER_SLIB_CORE_SCOPED_POINTER
#define CHECKHEADER_SLIB_CORE_SCOPED_POINTER

#include "definition.h"

#include "base.h"

SLIB_NAMESPACE_BEGIN

template <class TYPE>
class SLIB_EXPORT ScopedPtr
{
	SLIB_DECLARE_CLASS_NOCOPY(ScopedPtr)
public:
	SLIB_INLINE ScopedPtr()
	{
		m_ptr = sl_null;
	}

	SLIB_INLINE ScopedPtr(TYPE* ptr)
	{
		m_ptr = ptr;
	}

	SLIB_INLINE ~ScopedPtr()
	{
		release();
	}

public:
	SLIB_INLINE void release()
	{
		if (m_ptr) {
			delete m_ptr;
			m_ptr = sl_null;
		}
	}

	SLIB_INLINE sl_bool isNull()
	{
		return (m_ptr == sl_null);
	}

	SLIB_INLINE sl_bool isNotNull()
	{
		return (m_ptr != sl_null);
	}

	SLIB_INLINE TYPE* getValue()
	{
		return m_ptr;
	}

	SLIB_INLINE void setValue(TYPE* ptr)
	{
		release();
		m_ptr = ptr;
	}

	SLIB_INLINE void setNull()
	{
		m_ptr = sl_null;
	}

	SLIB_INLINE TYPE& operator*()
	{
		return *(m_ptr);
	}

	SLIB_INLINE TYPE* operator->()
	{
		return (m_ptr);
	}
	
protected:
	TYPE* m_ptr;
	
};

template <class TYPE>
class SLIB_EXPORT ScopedPtrNew : public ScopedPtr<TYPE>
{
public:
	SLIB_INLINE ScopedPtrNew() : ScopedPtr<TYPE>(new TYPE)
	{
	}
	
	SLIB_INLINE TYPE& operator*()
	{
		return *(this->m_ptr);
	}
	
	SLIB_INLINE TYPE* operator->()
	{
		return (this->m_ptr);
	}

};

template <class TYPE>
class SLIB_EXPORT ScopedArray
{
	SLIB_DECLARE_CLASS_NOCOPY(ScopedArray)
public:
	SLIB_INLINE ScopedArray()
	{
		m_ptr = sl_null;
		m_size = 0;
	}

	SLIB_INLINE ScopedArray(TYPE* ptr, sl_size size)
	{
		m_ptr = ptr;
		m_size = size;
	}
	
	SLIB_INLINE ScopedArray(sl_size size)
	{
		m_ptr = new TYPE[size];
		m_size = size;
	}

	SLIB_INLINE ~ScopedArray()
	{
		release();
	}

public:
	SLIB_INLINE void release()
	{
		if (m_ptr) {
			delete[] m_ptr;
			m_ptr = sl_null;
		}
	}

	SLIB_INLINE sl_bool isNull()
	{
		return (m_ptr == sl_null);
	}

	SLIB_INLINE sl_bool isNotNull()
	{
		return (m_ptr != sl_null);
	}

	SLIB_INLINE TYPE* getBuffer()
	{
		return m_ptr;
	}
	
	SLIB_INLINE sl_size getSize()
	{
		return m_size;
	}

	SLIB_INLINE void setBuffer(TYPE* ptr, sl_size size)
	{
		m_ptr = ptr;
		m_size = size;
	}

	SLIB_INLINE void setNull()
	{
		m_ptr = sl_null;
	}

	SLIB_INLINE TYPE& operator[](sl_size index)
	{
		return m_ptr[index];
	}
	
	SLIB_INLINE TYPE* operator+(sl_size offset)
	{
		return getBuffer() + offset;
	}
	
protected:
	TYPE* m_ptr;
	sl_size m_size;

};

#define SLIB_SCOPED_ARRAY(TYPE, NAME, SIZE) \
	ScopedArray<TYPE> _scoped_array__##NAME(SIZE); \
	TYPE* NAME = _scoped_array__##NAME.getBuffer();


template <class TYPE, sl_size sizeStack = 0>
class SLIB_EXPORT ScopedBuffer
{
	SLIB_DECLARE_CLASS_NOCOPY(ScopedBuffer)
public:
	SLIB_INLINE ScopedBuffer(sl_size size)
	{
		if (size < sizeStack) {
			m_ptr = m_stack;
		} else {
			m_ptr = new TYPE[size];
		}
		m_size = size;
	}

	SLIB_INLINE ~ScopedBuffer()
	{
		release();
	}

public:
	SLIB_INLINE void release()
	{
		if (m_ptr) {
			if (m_ptr != m_stack) {
				delete[] m_ptr;
			}
			m_ptr = sl_null;
		}
	}

	SLIB_INLINE sl_bool isNull()
	{
		return (m_ptr == sl_null);
	}

	SLIB_INLINE sl_bool isNotNull()
	{
		return (m_ptr != sl_null);
	}

	SLIB_INLINE TYPE* getBuffer()
	{
		return m_ptr;
	}

	SLIB_INLINE sl_size getSize()
	{
		return m_size;
	}

	SLIB_INLINE TYPE& operator[](sl_size index)
	{
		return m_ptr[index];
	}

	SLIB_INLINE TYPE* operator+(sl_size offset)
	{
		return getBuffer() + offset;
	}
	
protected:
	TYPE* m_ptr;
	TYPE m_stack[sizeStack];
	sl_size m_size;
	
};

#define SLIB_SCOPED_BUFFER(TYPE, STACK, NAME, SIZE) \
	ScopedBuffer<TYPE, STACK> _scoped_buf__##NAME(SIZE); \
	TYPE* NAME = _scoped_buf__##NAME.getBuffer();


SLIB_NAMESPACE_END

#endif
