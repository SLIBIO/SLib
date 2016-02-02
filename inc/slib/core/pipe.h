#ifndef CHECKHEADER_SLIB_CORE_PIPE
#define CHECKHEADER_SLIB_CORE_PIPE

#include "definition.h"
#include "io.h"
#include "event.h"

typedef sl_reg sl_pipe;
#define SLIB_PIPE_INVALID_HANDLE (sl_pipe)(-1)

SLIB_NAMESPACE_BEGIN
class SLIB_EXPORT Pipe : public Object, public IStream
{
	SLIB_DECLARE_OBJECT(Pipe, Object)
private:
	Pipe();
	~Pipe();

public:
	static Ref<Pipe> create();

public:
	/* attributes */
	SLIB_INLINE sl_bool isOpened() const
	{
		return m_hRead != SLIB_PIPE_INVALID_HANDLE;
	}

	SLIB_INLINE sl_pipe getReadHandle() const
	{
		return m_hRead;
	}

	SLIB_INLINE sl_pipe getWriteHandle() const
	{
		return m_hWrite;
	}

	sl_int32 read32(void* buf, sl_uint32 size);
	
	sl_int32 write32(const void* buf, sl_uint32 size);
	
	void close();

protected:
	static sl_bool _open(sl_pipe& hRead, sl_pipe& hWrite);
	
	static void _close(sl_pipe handle);
	
private:
	sl_pipe m_hRead;
	sl_pipe m_hWrite;

};

class SLIB_EXPORT PipeEvent : public Event
{
protected:
	PipeEvent();
	
public:
	static Ref<PipeEvent> create();

public:
	Ref<Pipe> getPipe();
	
	sl_pipe getReadPipeHandle();
	
	sl_pipe getWritePipeHandle();

protected:
	// override
	void __set();
	
	// override
	void __reset();
	
	// override
	sl_bool __wait(sl_int32 timeout);

protected:
	Ref<Pipe> m_pipe;
	sl_bool m_flagSet;
	SpinLock m_lock;

};

SLIB_NAMESPACE_END

#endif
