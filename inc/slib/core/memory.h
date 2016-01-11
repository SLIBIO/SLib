#ifndef CHECKHEADER_SLIB_CORE_MEMORY
#define CHECKHEADER_SLIB_CORE_MEMORY

#include "definition.h"

#include "array.h"
#include "queue.h"

SLIB_NAMESPACE_BEGIN

class SafeMemory;

typedef CArray<sl_uint8> CMemory;

/** auto-referencing object **/
class SLIB_EXPORT Memory
{
	typedef CMemory _Obj;
	typedef Ref<_Obj> _Ref;
	SLIB_DECLARE_OBJECT_TYPE_FROM(Memory, _Obj)
	SLIB_DECLARE_OBJECT_WRAPPER(Memory, Memory, _Obj, _Ref)
	
public:
	SLIB_INLINE Memory(sl_size size) : m_object(_Obj::create(size))
	{
	}
	
	SLIB_INLINE Memory(const void* buf, sl_size size) : m_object(_Obj::create((sl_uint8*)buf, size))
	{
	}
	
	SLIB_INLINE Memory(const void* buf, sl_size size, const Referable* refer) : m_object(_Obj::createStatic((const sl_uint8*)buf, size, refer))
	{
	}
	
public:
	Memory(const SafeMemory& other);
	
	Memory& operator=(const SafeMemory& other);
	
	SLIB_INLINE _Obj* getObject() const
	{
		return m_object.get();
	}
	
public:
	SLIB_INLINE static Memory create(sl_size count)
	{
		return Memory(count);
	}
	
	SLIB_INLINE static Memory create(const void* buf, sl_size count)
	{
		return Memory(buf, count);
	}
	
	SLIB_INLINE static Memory createStatic(const void* buf, sl_size count)
	{
		return Memory(buf, count, sl_null);
	}
	
	SLIB_INLINE static Memory createStatic(const void* buf, sl_size count, const Referable* refer)
	{
		return Memory(buf, count, refer);
	}
	
public:
	SLIB_INLINE sl_uint8* getData() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->data();
		}
		return sl_null;
	}
	
	SLIB_INLINE sl_uint8* data() const
	{
		return getData();
	}
	
	SLIB_INLINE void* getBuf() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->data();
		}
		return sl_null;
	}
	
	SLIB_INLINE void* buf() const
	{
		return getBuf();
	}
	
	SLIB_INLINE sl_size getSize() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->count();
		}
		return 0;
	}
	
	SLIB_INLINE sl_size size() const
	{
		return getSize();
	}
	
	SLIB_INLINE sl_bool isEmpty() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->count() == 0;
		}
		return sl_true;
	}
	
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->count() != 0;
		}
		return sl_false;
	}
	
public:
	SLIB_INLINE sl_uint8& operator[](sl_reg index) const
	{
		return *(m_object->data() + index);
	}
	
public:
	SLIB_INLINE Memory sub(sl_size start, sl_size size = SLIB_SIZE_MAX) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->sub(start, size);
		}
		return Memory::null();
	}
	
	SLIB_INLINE sl_size read(sl_size startSource, sl_size len, void* bufDst) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->read(startSource, len, (sl_uint8*)bufDst);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size write(sl_size startTarget, sl_size len, const void* bufSrc) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->write(startTarget, len, (const sl_uint8*)bufSrc);
		}
		return 0;
	}
	
	sl_size copy(sl_size startTarget, const Memory& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->copy(startTarget, source.getObject(), startSource, len);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size copy(const Memory& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(0, source, start, len);
	}
	
	SLIB_INLINE Memory duplicate() const
	{
		_Obj* obj = m_object.get();
		if (obj) {
			return obj->duplicate();
		}
		return Memory::null();
	}
	
};

/** auto-referencing object **/
class SLIB_EXPORT SafeMemory
{
	typedef CMemory _Obj;
	typedef SafeRef<_Obj> _Ref;
	typedef Ref<_Obj> _LocalRef;
	SLIB_DECLARE_OBJECT_TYPE_FROM(SafeMemory, _Obj)
	SLIB_DECLARE_OBJECT_WRAPPER(SafeMemory, SafeMemory, _Obj, _Ref)
	
public:
	SLIB_INLINE SafeMemory(sl_size size) : m_object(_Obj::create(size))
	{
	}
	
	SLIB_INLINE SafeMemory(const void* buf, sl_size size) : m_object(_Obj::create((const sl_uint8*)buf, size))
	{
	}
	
	SLIB_INLINE SafeMemory(const void* buf, sl_size size, const Referable* refer) : m_object(_Obj::createStatic((const sl_uint8*)buf, size, refer))
	{
	}
	
public:
	SLIB_INLINE SafeMemory(const Memory& other) : m_object(other.getRef())
	{
	}
	
	SLIB_INLINE SafeMemory& operator=(const Memory& other)
	{
		m_object = other.getRef();
		return *this;
	}
	
public:
	SLIB_INLINE sl_size getSize() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->count();
		}
		return 0;
	}
	
	SLIB_INLINE sl_size size() const
	{
		return getSize();
	}
	
	SLIB_INLINE sl_bool isEmpty() const
	{
		return getSize() == 0;
	}
	
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return getSize() != 0;
	}
	
public:
	SLIB_INLINE Memory sub(sl_size start, sl_size count = SLIB_SIZE_MAX) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->sub(start, count);
		}
		return Memory::null();
	}
	
	SLIB_INLINE sl_size read(sl_size startSource, sl_size len, void* bufDst) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->read(startSource, len, (sl_uint8*)bufDst);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size write(sl_size startTarget, sl_size len, const void* bufSrc) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->write(startTarget, len, (const sl_uint8*)bufSrc);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size copy(sl_size startTarget, const Memory& source, sl_size startSource = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->copy(startTarget, source.getObject(), startSource, len);
		}
		return 0;
	}
	
	SLIB_INLINE sl_size copy(const Memory& source, sl_size start = 0, sl_size len = SLIB_SIZE_MAX) const
	{
		return copy(0, source, start, len);
	}
	
	SLIB_INLINE Memory duplicate() const
	{
		_LocalRef obj(m_object);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return Memory::null();
	}
	
};

SLIB_INLINE Memory::Memory(const SafeMemory& other) : m_object(other.getRef())
{
}

SLIB_INLINE Memory& Memory::operator=(const SafeMemory& other)
{
	m_object = other.getRef();
	return *this;
}

class SLIB_EXPORT MemoryBuffer : public Object
{
public:
	MemoryBuffer();
	~MemoryBuffer();

public:
	SLIB_INLINE sl_size getSize() const
	{
		return m_size;
	}

	sl_bool add(const Memory& mem);
	
	sl_bool add(const void* buf, sl_size size);

	void link(MemoryBuffer& buf);

	void clear();

	Memory merge() const;

protected:
	Queue<Memory> m_queue;
	sl_size m_size;
};

class SLIB_EXPORT MemoryQueue : public MemoryBuffer
{
public:
	MemoryQueue();
	~MemoryQueue();

public:
	SLIB_INLINE sl_bool push(const Memory& mem)
	{
		return add(mem);
	}

	SLIB_INLINE sl_bool push(const void* buf, sl_size size)
	{
		return add(buf, size);
	}

	Memory pop();

	sl_size pop(void* buf, sl_size size);

	Memory merge() const;

private:
	Memory m_memCurrent;
	sl_size m_posCurrent;
};

SLIB_NAMESPACE_END

#endif
