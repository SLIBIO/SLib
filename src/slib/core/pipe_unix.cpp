#include "../../../inc/slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_UNIX

#define _FILE_OFFSET_BITS 64
#include <unistd.h>
#include <errno.h>

#include "../../../inc/slib/core/pipe.h"

SLIB_NAMESPACE_BEGIN

sl_bool Pipe::_open(sl_pipe& hRead, sl_pipe& hWrite)
{
	int fd[2];
	if (0 == pipe(fd)) {
		hRead = (int)(fd[0]);
		hWrite = (int)(fd[1]);
		return sl_true;
	}
	return sl_false;
}

void Pipe::_close(sl_pipe handle)
{
	::close((int)handle);
}

sl_int32 Pipe::read32(void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		int fd = (int)m_hRead;
		ssize_t n = ::read(fd, buf, size);
		if (n >= 0) {
			return (sl_int32)n;
		} else {
			int err = errno;
			if (err == EAGAIN || err == EWOULDBLOCK) {
				return 0;
			}
			return -1;
		}
	}
	return -1;
}

sl_int32 Pipe::write32(const void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		int fd = (int)m_hWrite;
		ssize_t n = ::write(fd, buf, size);
		if (n >= 0) {
			return (sl_int32)n;
		} else {
			int err = errno;
			if (err == EAGAIN || err == EWOULDBLOCK) {
				return 0;
			}
			return -1;
		}
	}
	return -1;
}

SLIB_NAMESPACE_END

#endif
