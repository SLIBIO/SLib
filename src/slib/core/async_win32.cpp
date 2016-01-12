#include "async_config.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include <windows.h>

#include "../../../inc/slib/core/async.h"

SLIB_NAMESPACE_BEGIN

class _Win32AsyncFileStreamInstance : public AsyncStreamInstance
{
public:
	Ref<AsyncStreamRequest> m_requestOperating;
	sl_uint64 m_offset;
	OVERLAPPED m_overlappedRead;
	OVERLAPPED m_overlappedWrite;

	_Win32AsyncFileStreamInstance()
	{
		m_offset = 0;
	}

	~_Win32AsyncFileStreamInstance()
	{
		close();
	}

	static Ref<_Win32AsyncFileStreamInstance> open(const String& path, File::Mode mode)
	{
		Ref<_Win32AsyncFileStreamInstance> ret;
		sl_file handle = _openHandle(path, mode);
		if (handle != SLIB_FILE_INVALID_HANDLE) {
			ret = new _Win32AsyncFileStreamInstance();
			if (ret.isNotNull()) {
				ret->setHandle(handle);
				if (mode == File::modeAppend) {
					ret->seek(File::getSize(handle));
				}
				return ret;
			}
			_closeHandle(handle);
		}
		return ret;
	}

	void close()
	{
		_closeHandle(getHandle());
		setHandle(SLIB_FILE_INVALID_HANDLE);
	}

	void onOrder()
	{
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		if (m_requestOperating.isNull()) {
			Ref<AsyncStreamRequest> req;
			if (m_requestsRead.pop(&req)) {
				if (req.isNotNull()) {
					Base::resetMemory(&m_overlappedRead, 0, sizeof(m_overlappedRead));
					m_overlappedRead.Offset = (DWORD)m_offset;
					m_overlappedRead.OffsetHigh = (DWORD)(m_offset >> 32);
					if (::ReadFile((HANDLE)handle, req->data(), req->size(), NULL, &m_overlappedRead)) {
						doInput(req.get(), 0, sl_true);
					} else {
						DWORD dwErr = ::GetLastError();
						if (dwErr == ERROR_IO_PENDING) {
							m_requestOperating = req;
						} else {
							doInput(req.get(), 0, sl_true);
						}
					}
				}
			}
		}
		if (m_requestOperating.isNull()) {
			Ref<AsyncStreamRequest> req;
			if (m_requestsWrite.pop(&req)) {
				if (req.isNotNull()) {
					Base::resetMemory(&m_overlappedWrite, 0, sizeof(m_overlappedWrite));
					m_overlappedWrite.Offset = (DWORD)m_offset;
					m_overlappedWrite.OffsetHigh = (DWORD)(m_offset >> 32);
					if (::WriteFile((HANDLE)handle, req->data(), req->size(), NULL, &m_overlappedWrite)) {
						doOutput(req.get(), 0, sl_true);
					} else {
						DWORD dwErr = ::GetLastError();
						if (dwErr == ERROR_IO_PENDING) {
							m_requestOperating = req;
						} else {
							doOutput(req.get(), 0, sl_true);
						}
					}
				}
			}
		}
	}

	void onEvent(EventDesc* pev)
	{
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		OVERLAPPED* pOverlapped = (OVERLAPPED*)(pev->pOverlapped);
		DWORD dwSize = 0;
		sl_bool flagError = sl_false;
		if (!::GetOverlappedResult((HANDLE)handle, pOverlapped, &dwSize, FALSE)) {
			flagError = sl_true;
			close();
		}
		m_offset += dwSize;

		Ref<AsyncStreamRequest> req = m_requestOperating;
		m_requestOperating.setNull();

		if (req.isNotNull()) {
			if (pOverlapped == &m_overlappedRead) {
				doInput(req.get(), dwSize, flagError);
			} else if (pOverlapped == &m_overlappedWrite) {
				doOutput(req.get(), dwSize, flagError);
			}
		}
	}

	void doInput(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
	{
		Ref<AsyncObject> object = getObject();
		if (object.isNull()) {
			return;
		}
		PtrLocker<IAsyncStreamListener> listener(req->getListener());
		if (listener.isNotNull()) {
			listener->onRead((AsyncStream*)(object.get()), req->data(), size, req->getDataReference(), flagError);
		}
	}

	void doOutput(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
	{
		Ref<AsyncObject> object = getObject();
		if (object.isNull()) {
			return;
		}
		PtrLocker<IAsyncStreamListener> listener(req->getListener());
		if (listener.isNotNull()) {
			listener->onWrite((AsyncStream*)(object.get()), req->data(), size, req->getDataReference(), flagError);
		}
	}

	sl_bool isSeekable()
	{
		return sl_true;
	}

	sl_bool seek(sl_uint64 pos)
	{
		m_offset = pos;
		return sl_true;
	}

	sl_uint64 getSize()
	{
		sl_file handle = getHandle();
		return File::getSize(handle);
	}

	static sl_file _openHandle(const String& _filePath, File::Mode mode)
	{
		String16 filePath = _filePath;
		if (filePath.isEmpty()) {
			return (sl_file)-1;
		}

		DWORD dwDesiredAccess = 0;
		DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		DWORD dwCreateDisposition = 0;
		DWORD dwFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;

		switch (mode) {
		case File::modeRead:
			dwDesiredAccess = GENERIC_READ;
			dwCreateDisposition = OPEN_EXISTING;
			break;
		case File::modeWrite:
			dwDesiredAccess = GENERIC_WRITE;
			dwCreateDisposition = CREATE_ALWAYS;
			break;
		case File::modeReadWrite:
			dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			dwCreateDisposition = CREATE_ALWAYS;
			break;
		case File::modeAppend:
			dwDesiredAccess = GENERIC_WRITE;
			dwCreateDisposition = OPEN_ALWAYS;
			break;
		case File::modeRandomAccess:
			dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			dwCreateDisposition = OPEN_ALWAYS;
			dwFlags |= FILE_FLAG_RANDOM_ACCESS;
			break;
		}

		HANDLE handle = ::CreateFileW(
			(LPCWSTR)(filePath.getBuf())
			, dwDesiredAccess
			, dwShareMode
			, NULL
			, dwCreateDisposition
			, dwFlags
			, NULL
			);

		return (sl_file)handle;
	}

	static void _closeHandle(sl_file handle)
	{
		if (handle != SLIB_FILE_INVALID_HANDLE) {
			::CloseHandle((HANDLE)handle);
		}
	}
};

Ref<AsyncStream> AsyncFile::openIOCP(const String& path, File::Mode mode, const Ref<AsyncLoop>& loop)
{
	Ref<_Win32AsyncFileStreamInstance> ret = _Win32AsyncFileStreamInstance::open(path, mode);
	return AsyncStream::create(ret.get(), loop);
}

Ref<AsyncStream> AsyncFile::openIOCP(const String& path, File::Mode mode)
{
	return AsyncFile::openIOCP(path, mode, AsyncLoop::getDefault());
}
SLIB_NAMESPACE_END

#endif
