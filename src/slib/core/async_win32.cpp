/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "async_config.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include <windows.h>

#include "slib/core/async.h"

namespace slib
{

	class _priv_Win32AsyncFileStreamInstance : public AsyncStreamInstance
	{
	public:
		Ref<AsyncStreamRequest> m_requestOperating;
		sl_uint64 m_offset;
		OVERLAPPED m_overlappedRead;
		OVERLAPPED m_overlappedWrite;

		_priv_Win32AsyncFileStreamInstance()
		{
			m_offset = 0;
		}

		~_priv_Win32AsyncFileStreamInstance()
		{
			close();
		}

		static Ref<_priv_Win32AsyncFileStreamInstance> open(const String& path, FileMode mode)
		{
			Ref<_priv_Win32AsyncFileStreamInstance> ret;
			sl_file handle = _openHandle(path, mode);
			if (handle != SLIB_FILE_INVALID_HANDLE) {
				ret = new _priv_Win32AsyncFileStreamInstance();
				if (ret.isNotNull()) {
					ret->setHandle(handle);
					if (mode & FileMode::SeekToEnd) {
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

		void onOrder() override
		{
			sl_file handle = getHandle();
			if (handle == SLIB_FILE_INVALID_HANDLE) {
				return;
			}
			if (m_requestOperating.isNull()) {
				Ref<AsyncStreamRequest> req;
				if (popReadRequest(req)) {
					if (req.isNotNull()) {
						if (req->data && req->size) {
							Base::resetMemory(&m_overlappedRead, 0, sizeof(m_overlappedRead));
							m_overlappedRead.Offset = (DWORD)m_offset;
							m_overlappedRead.OffsetHigh = (DWORD)(m_offset >> 32);
							if (::ReadFile((HANDLE)handle, req->data, req->size, NULL, &m_overlappedRead)) {
								doInput(req.get(), 0, sl_true);
							} else {
								DWORD dwErr = ::GetLastError();
								if (dwErr == ERROR_IO_PENDING) {
									m_requestOperating = req;
								} else {
									doInput(req.get(), 0, sl_true);
								}
							}
						} else {
							doInput(req.get(), req->size, sl_false);
						}
					}
				}
			}
			if (m_requestOperating.isNull()) {
				Ref<AsyncStreamRequest> req;
				if (popWriteRequest(req)) {
					if (req.isNotNull()) {
						if (req->data && req->size) {
							Base::resetMemory(&m_overlappedWrite, 0, sizeof(m_overlappedWrite));
							m_overlappedWrite.Offset = (DWORD)m_offset;
							m_overlappedWrite.OffsetHigh = (DWORD)(m_offset >> 32);
							if (::WriteFile((HANDLE)handle, req->data, req->size, NULL, &m_overlappedWrite)) {
								doOutput(req.get(), 0, sl_true);
							} else {
								DWORD dwErr = ::GetLastError();
								if (dwErr == ERROR_IO_PENDING) {
									m_requestOperating = req;
								} else {
									doOutput(req.get(), 0, sl_true);
								}
							}
						} else {
							doOutput(req.get(), req->size, sl_false);
						}
					}
				}
			}
		}

		void onEvent(EventDesc* pev) override
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
			if (dwSize > 0) {
				m_offset += dwSize;
			} else {
				flagError = sl_true;
			}

			Ref<AsyncStreamRequest> req = m_requestOperating;
			m_requestOperating.setNull();

			if (req.isNotNull()) {
				if (pOverlapped == &m_overlappedRead) {
					doInput(req.get(), dwSize, flagError);
				} else if (pOverlapped == &m_overlappedWrite) {
					doOutput(req.get(), dwSize, flagError);
				}
			}

			requestOrder();
		}

		void doInput(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
		{
			Ref<AsyncIoObject> object = getObject();
			if (object.isNotNull()) {
				req->runCallback(static_cast<AsyncStream*>(object.get()), size, flagError);
			}
		}

		void doOutput(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError)
		{
			Ref<AsyncIoObject> object = getObject();
			if (object.isNotNull()) {
				req->runCallback(static_cast<AsyncStream*>(object.get()), size, flagError);
			}
		}

		sl_bool isSeekable() override
		{
			return sl_true;
		}

		sl_bool seek(sl_uint64 pos) override
		{
			m_offset = pos;
			return sl_true;
		}

		sl_uint64 getSize() override
		{
			sl_file handle = getHandle();
			return File::getSize(handle);
		}

		static sl_file _openHandle(const String& _filePath, FileMode mode)
		{
			String16 filePath = _filePath;
			if (filePath.isEmpty()) {
				return (sl_file)-1;
			}

			DWORD dwShareMode = (mode & FileMode::Read) ? FILE_SHARE_READ : 0;
			DWORD dwDesiredAccess = 0;
			DWORD dwCreateDisposition = 0;
			DWORD dwFlags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;

			if (mode & FileMode::Write) {
				dwDesiredAccess = GENERIC_WRITE;
				if (mode & FileMode::Read) {
					dwDesiredAccess |= GENERIC_READ;
				}
				if (mode & FileMode::NotCreate) {
					if (mode & FileMode::NotTruncate) {
						dwCreateDisposition = OPEN_EXISTING;
					} else {
						dwCreateDisposition = TRUNCATE_EXISTING;
					}
				} else {
					if (mode & FileMode::NotTruncate) {
						dwCreateDisposition = OPEN_ALWAYS;
					} else {
						dwCreateDisposition = CREATE_ALWAYS;
					}
				}
			} else {
				dwDesiredAccess = GENERIC_READ;
				dwCreateDisposition = OPEN_EXISTING;
			}
			if (mode & FileMode::HintRandomAccess) {
				dwFlags |= FILE_FLAG_RANDOM_ACCESS;
			}

			HANDLE handle = ::CreateFileW(
				(LPCWSTR)(filePath.getData())
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

	Ref<AsyncStream> AsyncFile::openIOCP(const String& path, FileMode mode, const Ref<AsyncIoLoop>& loop)
	{
		Ref<_priv_Win32AsyncFileStreamInstance> ret = _priv_Win32AsyncFileStreamInstance::open(path, mode);
		return AsyncStream::create(ret.get(), AsyncIoMode::InOut, loop);
	}

	Ref<AsyncStream> AsyncFile::openIOCP(const String& path, FileMode mode)
	{
		return AsyncFile::openIOCP(path, mode, AsyncIoLoop::getDefault());
	}

}

#endif
