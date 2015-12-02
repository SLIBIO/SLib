#include "../../../inc/slib/core/pipe.h"
#include "../../../inc/slib/core/file.h"

#if defined(SLIB_PLATFORM_IS_UNIX)
#include <poll.h>
#endif

SLIB_NAMESPACE_BEGIN
/********************************************
	Pipe
********************************************/
Pipe::Pipe()
{
	m_hRead = sl_null;
	m_hWrite = sl_null;
}

Pipe::~Pipe()
{
	close();
}

Ref<Pipe> Pipe::create()
{
	sl_pipe hRead = SLIB_PIPE_INVALID_HANDLE;
	sl_pipe hWrite = SLIB_PIPE_INVALID_HANDLE;
	if (_open(hRead, hWrite)) {
		Ref<Pipe> ret = new Pipe;
		if (ret.isNotNull()) {
			ret->m_hRead = hRead;
			ret->m_hWrite = hWrite;
			return ret;
		}
		_close(hRead);
		_close(hWrite);
	}
	return Ref<Pipe>::null();
}

void Pipe::close()
{
	if (m_hRead != SLIB_PIPE_INVALID_HANDLE) {
		_close(m_hRead);
		m_hRead = SLIB_PIPE_INVALID_HANDLE;
	}
	if (m_hWrite != SLIB_PIPE_INVALID_HANDLE) {
		_close(m_hWrite);
		m_hWrite = SLIB_PIPE_INVALID_HANDLE;
	}
}

/********************************************
	PipeEvent
********************************************/
PipeEvent::PipeEvent()
{
	m_nSizeWritten = 0;
}

PipeEvent::~PipeEvent()
{
}

Ref<Pipe> PipeEvent::getPipe()
{
	return m_pipe;
}

void PipeEvent::__set()
{
	if (m_nSizeWritten < 100) {
		char c = 1;
		m_pipe->write(&c, 1);
		m_nSizeWritten++;
	}
}

void PipeEvent::__reset()
{
#if !defined(SLIB_PLATFORM_IS_UNIX)
	if (m_nSizeWritten > 0)
#endif
	{
		static char t[200];
		m_pipe->read(t, 200);
		m_nSizeWritten = 0;
	}
}

sl_bool PipeEvent::__wait(sl_int32 timeout)
{
#if defined(SLIB_PLATFORM_IS_UNIX)
	pollfd fd;
	Base::zeroMemory(&fd, sizeof(pollfd));
	fd.fd = (int)(getReadPipeHandle());
	fd.events = POLLIN | POLLPRI | POLLERR | POLLHUP;
	int t = timeout >= 0 ? (int)timeout : -1;
	int ret = ::poll(&fd, 1, t);
	if (ret > 0) {
		return sl_true;
	} else {
		return sl_false;
	}
#else
	static char t[200];
	m_pipe->read(t, 200);
	return sl_true;
#endif
}

sl_pipe PipeEvent::getReadPipeHandle()
{
	return m_pipe->getReadHandle();
}

sl_pipe PipeEvent::getWritePipeHandle()
{
	return m_pipe->getWriteHandle();
}

Ref<PipeEvent> PipeEvent::create()
{
	Ref<Pipe> pipe = Pipe::create();
	if (pipe.isNotNull()) {
		Ref<PipeEvent> ret = new PipeEvent;
		if (ret.isNotNull()) {
			ret->m_pipe = pipe;
#if defined(SLIB_PLATFORM_IS_UNIX)
			File::setNonBlocking((int)(pipe->getReadHandle()), sl_true);
			File::setNonBlocking((int)(pipe->getWriteHandle()), sl_true);
#endif
			return ret;
		}
	}
	return Ref<PipeEvent>::null();
}

SLIB_NAMESPACE_END
