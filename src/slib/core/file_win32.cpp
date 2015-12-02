#include "../../../inc/slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_WIN32

#include "../../../inc/slib/core/file.h"

#include <windows.h>

SLIB_NAMESPACE_BEGIN
sl_file File::_open(const String& _filePath, Mode mode)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return SLIB_FILE_INVALID_HANDLE;
	}

	DWORD dwDesiredAccess = 0;
	DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	DWORD dwCreateDisposition = 0;
	DWORD dwFlags = FILE_ATTRIBUTE_NORMAL;

	switch (mode) {
	case modeRead:
		dwDesiredAccess = GENERIC_READ;
		dwCreateDisposition = OPEN_EXISTING;
		break;
	case modeWrite:
		dwDesiredAccess = GENERIC_WRITE;
		dwCreateDisposition = CREATE_ALWAYS;
		break;
	case modeReadWrite:
		dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
		dwCreateDisposition = CREATE_ALWAYS;
		break;
	case modeAppend:
		dwDesiredAccess = GENERIC_WRITE;
		dwCreateDisposition = OPEN_ALWAYS;
		break;
	case modeRandomAccess:
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

sl_bool File::_close(sl_file file)
{
	HANDLE handle = (HANDLE)file;
	if (handle != (HANDLE)SLIB_FILE_INVALID_HANDLE) {
		::CloseHandle(handle);
		return sl_true;
	}
	return sl_false;
}

sl_bool File::lock()
{
	HANDLE handle = (HANDLE)m_file;
	if (handle != (HANDLE)SLIB_FILE_INVALID_HANDLE) {
		OVERLAPPED o;
		ZeroMemory(&o, sizeof(o));
		if (::LockFileEx(handle, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 1, 0, &o)) {
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool File::unlock()
{
	HANDLE handle = (HANDLE)m_file;
	if (handle != (HANDLE)SLIB_FILE_INVALID_HANDLE) {
		OVERLAPPED o;
		ZeroMemory(&o, sizeof(o));
		if (::UnlockFileEx(handle, 0, 0, 0, &o)) {
			return sl_true;
		}
	}
	return sl_false;
}

sl_uint64 File::getPosition()
{
	if (isOpened()) {
		HANDLE handle = (HANDLE)m_file;
		sl_int64 pos = 0;
		DWORD ret = ::SetFilePointer(handle, 0, ((LONG*)&pos) + 1, FILE_CURRENT);
		DWORD err = NOERROR;
		if (ret == -1) {
			err = GetLastError();
		}
		if (err == NOERROR) {
			*((DWORD*)&pos) = ret;
			return pos;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

sl_bool File::seek(sl_int64 location, Position from)
{
	if (isOpened()) {
		HANDLE handle = (HANDLE)m_file;
		DWORD dwFrom;
		if (from == positionCurrent) {
			dwFrom = FILE_CURRENT;
		} else if (from == positionBegin) {
			dwFrom = FILE_BEGIN;
		} else if (from == positionEnd) {
			dwFrom = FILE_END;
		} else {
			return sl_false;
		}
		DWORD ret = ::SetFilePointer(handle, *((LONG*)&location), ((LONG*)&location) + 1, dwFrom);
		DWORD err = NOERROR;
		if (ret == -1) {
			err = GetLastError();
		}
		if (err == NOERROR) {
			return sl_true;
		}
	}
	return sl_false;
}


sl_bool File::setSize(sl_uint64 size)
{
	if (isOpened()) {
		sl_int64 pos_orig = getPosition();
		if (seek(size, positionBegin)) {
			HANDLE handle = (HANDLE)m_file;
			BOOL bRet = ::SetEndOfFile(handle);
			seek(pos_orig, positionBegin);
			return bRet != 0;
		}
	}
	return sl_false;
}

sl_int32 File::read32(void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		sl_uint32 ret = 0;
		HANDLE handle = (HANDLE)m_file;
		if (::ReadFile(handle, buf, size, (DWORD*)&ret, NULL)) {
			if (ret > 0) {
				return ret;
			}
		}
	}
	return -1;
}

sl_int32 File::write32(const void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		sl_uint32 ret = 0;
		HANDLE handle = (HANDLE)m_file;
		if (::WriteFile(handle, (LPVOID)buf, size, (DWORD*)&ret, NULL)) {
			if (ret > 0) {
				return ret;
			}
		}
	}
	return -1;
}

sl_uint64 File::getSize(sl_file fd)
{
	HANDLE handle = (HANDLE)fd;
	if (handle != INVALID_HANDLE_VALUE) {
		sl_uint64 ret = 0;
		*((DWORD*)&ret) = ::GetFileSize(handle, ((DWORD*)&ret) + 1);
		return ret;
	} else {
		return 0;
	}
}

sl_uint64 File::getSize(const String& _filePath)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return 0;
	}
	HANDLE handle = ::CreateFileW((LPCWSTR)(filePath.getBuf()), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		return getSize((sl_file)handle);
	} else {
		return 0;
	}
}

static Time _File_getModifiedTime(HANDLE handle)
{
	FILETIME ft;
	BOOL bRet = ::GetFileTime(handle, NULL, NULL, &ft);
	if (bRet) {
		FILETIME lft;
		::FileTimeToLocalFileTime(&ft, &lft);
		Time time(*((sl_int64*)&lft) / 10);
		return time;
	} else {
		return Time::zero();
	}
}

static Time _File_getAccessedTime(HANDLE handle)
{
	FILETIME ft;
	BOOL bRet = ::GetFileTime(handle, NULL, &ft, NULL);
	if (bRet) {
		FILETIME lft;
		::FileTimeToLocalFileTime(&ft, &lft);
		Time time(*((sl_int64*)&lft) / 10);
		return time;
	} else {
		return Time::zero();
	}
}

static Time _File_getCreatedTime(HANDLE handle)
{
	FILETIME ft;
	BOOL bRet = ::GetFileTime(handle, &ft, NULL, NULL);
	if (bRet) {
		FILETIME lft;
		::FileTimeToLocalFileTime(&ft, &lft);
		Time time(*((sl_int64*)&lft) / 10);
		return time;
	} else {
		return Time::zero();
	}
}

Time File::getModifiedTime()
{
	if (isOpened()) {
		return _File_getModifiedTime((HANDLE)m_file);
	} else {
		return Time::zero();
	}
}

Time File::getModifiedTime(const String& _filePath)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return Time::zero();
	}
	HANDLE handle = ::CreateFileW((LPCWSTR)(filePath.getBuf()), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		Time ret = _File_getModifiedTime(handle);
		::CloseHandle(handle);
		return ret;
	} else {
		return Time::zero();
	}
}

Time File::getAccessedTime()
{
	if (isOpened()) {
		return _File_getAccessedTime((HANDLE)m_file);
	} else {
		return Time::zero();
	}
}

Time File::getAccessedTime(const String& _filePath)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return Time::zero();
	}
	HANDLE handle = ::CreateFileW((LPCWSTR)(filePath.getBuf()), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		Time ret = _File_getAccessedTime(handle);
		::CloseHandle(handle);
		return ret;
	} else {
		return Time::zero();
	}
}

Time File::getCreatedTime()
{
	if (isOpened()) {
		return _File_getCreatedTime((HANDLE)m_file);
	} else {
		return Time::zero();
	}
}

Time File::getCreatedTime(const String& _filePath)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return Time::zero();
	}
	HANDLE handle = ::CreateFileW((LPCWSTR)(filePath.getBuf()), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		Time ret = _File_getCreatedTime(handle);
		::CloseHandle(handle);
		return ret;
	} else {
		return Time::zero();
	}
}

sl_bool File::setModifiedTime(const String& _filePath, Time time)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return sl_false;
	}
	HANDLE handle = ::CreateFileW((LPCWSTR)(filePath.getBuf()), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		FILETIME lft;
		FILETIME ft;
		*((sl_int64*)&lft) = time.toInt() * 10;
		::LocalFileTimeToFileTime(&lft, &ft);
		BOOL bRet = ::SetFileTime(handle, NULL, NULL, &ft);
		::CloseHandle(handle);
		return bRet != 0;
	} else {
		return sl_false;
	}
}

sl_bool File::setAccessedTime(const String& _filePath, Time time)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return sl_false;
	}
	HANDLE handle = ::CreateFileW((LPCWSTR)(filePath.getBuf()), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		FILETIME lft;
		FILETIME ft;
		*((sl_int64*)&lft) = time.toInt() * 10;
		::LocalFileTimeToFileTime(&lft, &ft);
		BOOL bRet = ::SetFileTime(handle, NULL, &ft, NULL);
		::CloseHandle(handle);
		return bRet != 0;
	} else {
		return sl_false;
	}
}

sl_bool File::setCreatedTime(const String& _filePath, Time time)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return sl_false;
	}
	HANDLE handle = ::CreateFileW((LPCWSTR)(filePath.getBuf()), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) {
		FILETIME lft;
		FILETIME ft;
		*((sl_int64*)&lft) = time.toInt() * 10;
		::LocalFileTimeToFileTime(&lft, &ft);
		BOOL bRet = ::SetFileTime(handle, &ft, NULL, NULL);
		::CloseHandle(handle);
		return bRet != 0;
	} else {
		return sl_false;
	}
}

int File::getAttributes(const String& _filePath)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return -1;
	}
	DWORD attr = ::GetFileAttributesW((LPCWSTR)(filePath.getBuf()));
	if (attr == -1) {
		return -1;
	} else {
		int ret = 0;
		if (attr & FILE_ATTRIBUTE_DIRECTORY) {
			ret |= attributeDirectory;
		}
		if (attr & FILE_ATTRIBUTE_HIDDEN) {
			ret |= attributeHidden;
		}
		return ret;
	}
}

sl_bool File::setHidden(const String& _filePath, sl_bool flagHidden)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return sl_false;
	}
	DWORD attr = ::GetFileAttributesW((LPCWSTR)(filePath.getBuf()));
	if (attr == -1) {
		return sl_false;
	}
	if (flagHidden) {
		attr |= FILE_ATTRIBUTE_HIDDEN;
	} else {
		attr &= (~FILE_ATTRIBUTE_HIDDEN);
	}
	return ::SetFileAttributesW((LPCWSTR)(filePath.getBuf()), attr) != 0;
}

List<String> File::getFiles(const String& _filePath)
{
	String filePath = _filePath;
	if (filePath.isEmpty()) {
		return List<String>::null();
	}
	if (File::isDirectory(filePath)) {
		filePath = normalizeDirectoryPath(filePath);
	} else {
		return List<String>::null();
	}

	SLIB_STATIC_STRING(t, "/*");
	String16 query = filePath + t;
	WIN32_FIND_DATAW fd;
	HANDLE handle = ::FindFirstFileW((LPCWSTR)(query.getBuf()), &fd);
	if (handle != INVALID_HANDLE_VALUE) {
		List<String> ret;
		BOOL c = TRUE;
		SLIB_STATIC_STRING(p1, ".");
		SLIB_STATIC_STRING(p2, "..");
		while (c) {
			String str((sl_char16*)(fd.cFileName));
			if (str != p1 && str != p2) {
				ret.add(str);
			}
			c = ::FindNextFileW(handle, &fd);
		}
		::FindClose(handle);
		return ret;
	} else {
		return List<String>::null();
	}
}

sl_bool File::createDirectory(const String& _filePath)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return sl_false;
	}
	BOOL ret = ::CreateDirectoryW((LPCWSTR)(filePath.getBuf()), NULL);
	return ret != 0;
}

sl_bool File::deleteFile(const String& _filePath)
{
	String16 filePath = _filePath;
	if (filePath.isEmpty()) {
		return sl_false;
	}
	BOOL ret = ::DeleteFileW((LPCWSTR)(filePath.getBuf()));
	return ret != 0;
}

sl_bool File::deleteDirectoryOnly(const String& _filePath)
{
	String filePath = _filePath;
	if (filePath.isEmpty()) {
		return sl_false;
	}
	String16 dirPath = normalizeDirectoryPath(filePath);
	BOOL ret = ::RemoveDirectoryW((LPCWSTR)(dirPath.getBuf()));
	return ret != 0;
}

sl_bool File::rename(const String& _oldPath, const String& _newPath)
{
	String16 oldPath = _oldPath;
	if (oldPath.isEmpty()) {
		return sl_false;
	}
	String16 newPath = _newPath;
	if (newPath.isEmpty()) {
		return sl_false;
	}
	BOOL ret = ::MoveFileExW((LPCWSTR)(oldPath.getBuf()), (LPCWSTR)(newPath.getBuf()), 0);
	return ret != 0;
}
SLIB_NAMESPACE_END
#endif
