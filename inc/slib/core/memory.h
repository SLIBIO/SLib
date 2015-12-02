#ifndef CHECKHEADER_SLIB_CORE_MEMORY
#define CHECKHEADER_SLIB_CORE_MEMORY

#include "definition.h"
#include "object.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT MemoryObject : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(MemoryObject)
public:
	void* m_buf;
	sl_size m_size;
	sl_bool m_flagStatic;
	Ref<Referable> m_refer;
	
private:
	MemoryObject() {}
	
public:
	~MemoryObject();
	
public:
	static MemoryObject* create(const void* buf, sl_size size);
	static MemoryObject* createStatic(const void* buf, sl_size size, const Ref<Referable>& refer);
};

/** auto-referencing object **/
class SLIB_EXPORT Memory
{
	SLIB_DECLARE_OBJECT_TYPE_FROM(Memory, MemoryObject)
	SLIB_DECLARE_OBJECT_WRAPPER(Memory, Memory, MemoryObject, Ref<MemoryObject>)

public:
	SLIB_INLINE Memory(sl_size size) : m_object(MemoryObject::create(sl_null, size))
	{
	}
	SLIB_INLINE Memory(const void* buf, sl_size size) : m_object(MemoryObject::create(buf, size))
	{
	}

	SLIB_INLINE static Memory create(sl_size size)
	{
		Memory ret(size);
		return ret;
	}
	SLIB_INLINE static Memory create(const void* buf, sl_size size)
	{
		Memory ret(buf, size);
		return ret;
	}
	SLIB_INLINE static Memory createStatic(const void* buf, sl_size size)
	{
		return createStatic(buf, size, sl_null);
	}
	SLIB_INLINE static Memory createStatic(const void* buf, sl_size size, Ref<Referable> reference)
	{
		Memory ret;
		ret.m_object = MemoryObject::createStatic(buf, size, reference);
		return ret;
	}

	Memory sub(sl_size offset) const;
	Memory sub(sl_size offset, sl_size size) const;

	SLIB_INLINE void* getBuf() const
	{
		Ref<MemoryObject> object = m_object;
		if (object.isNotNull()) {
			return object->m_buf;
		} else {
			return sl_null;
		}
	}
	SLIB_INLINE sl_size getSize() const
	{
		Ref<MemoryObject> object = m_object;
		if (object.isNotNull()) {
			return (object->m_size);
		} else {
			return 0;
		}
	}
	SLIB_INLINE sl_size size() const
	{
		return getSize();
	}
	SLIB_INLINE sl_uint8& operator[](sl_size index)
	{
		return ((sl_uint8*)(m_object->m_buf))[index];
	}

	SLIB_INLINE sl_bool isEmpty() const
	{
		return isNull();
	}
	SLIB_INLINE sl_bool isNotEmpty() const
	{
		return isNotNull();
	}
	
	sl_size read(sl_size offset, sl_size len, void* output);
	sl_size write(sl_size offset, sl_size len, const void* source);
	sl_size copy(sl_size offsetTarget, Memory source, sl_size offsetSource = 0, sl_size len = SLIB_SIZE_MAX);

	SLIB_INLINE sl_size read(void* output, sl_size len)
	{
		return read(0, len, output);
	}

	SLIB_INLINE sl_size write(const void* source, sl_size len)
	{
		return write(0, len, source);
	}

	SLIB_INLINE sl_size copy(Memory source, sl_size offsetSource = 0, sl_size len = SLIB_SIZE_MAX)
	{
		return copy(0, source, offsetSource, len);
	}

	SLIB_INLINE Memory duplicate() const
	{
		Ref<MemoryObject> object = m_object;
		if (object.isNotNull()) {
			return Memory::create(object->m_buf, object->m_size);
		}
		return Memory::null();
	}
};
SLIB_NAMESPACE_END

#include "queue.h"
SLIB_NAMESPACE_BEGIN
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

#endif// CHECKHEADER_SLIB_CORE_MEMORY
