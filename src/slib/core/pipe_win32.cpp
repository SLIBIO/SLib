#include "../../../inc/slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_WIN32

#include <windows.h>

#include "../../../inc/slib/core/pipe.h"

SLIB_NAMESPACE_BEGIN
sl_bool Pipe::_open(sl_pipe& hRead, sl_pipe& hWrite)
{
	HANDLE _hRead, _hWrite;
	BOOL ret = ::CreatePipe(&_hRead, &_hWrite, NULL, 4096);
	if (ret) {
		hRead = (sl_pipe)_hRead;
		hWrite = (sl_pipe)_hWrite;
		return sl_true;
	}
	return sl_false;
}

void Pipe::_close(sl_pipe handle)
{
	::CloseHandle((HANDLE)handle);
}

sl_int32 Pipe::read32(void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		DWORD ret = 0;
		HANDLE handle = (HANDLE)m_hRead;
		if (::ReadFile(handle, buf, size, &ret, NULL)) {
			return ret;
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
		DWORD ret = 0;
		HANDLE handle = (HANDLE)m_hWrite;
		if (::WriteFile(handle, (LPVOID)buf, size, &ret, NULL)) {
			return ret;
		}
	}
	return -1;
}
SLIB_NAMESPACE_END
#endif
