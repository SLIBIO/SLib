/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_UNIX

#include "slib/core/file.h"

#define _FILE_OFFSET_BITS 64
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#if defined(SLIB_PLATFORM_IS_DESKTOP)
#	include <sys/ioctl.h>
#	if defined(SLIB_PLATFORM_IS_MACOS)
#		include <sys/disk.h>
#	elif defined(SLIB_PLATFORM_IS_LINUX)
#		include <linux/fs.h>
#	endif
#endif
#include <dirent.h>
#include <stdio.h>
#include <errno.h>

namespace slib
{

	sl_file File::_open(const String& filePath, const FileMode& mode, const FilePermissions& permissions)
	{
		if (filePath.isEmpty()) {
			return (sl_file)-1;
		}
		
		int flags = 0;
		if (mode & FileMode::Write) {
			if (mode & FileMode::Read) {
				flags = O_RDWR;
			} else {
				flags = O_WRONLY;
			}
			if (!(mode & FileMode::NotTruncate)) {
				flags |= O_TRUNC;
			}
			if (!(mode & FileMode::NotCreate)) {
				flags |= O_CREAT;
			}
		} else {
			flags = O_RDONLY;
		}
		
		int perm = 0;
		if (flags & O_CREAT) {
			if (permissions & FilePermissions::ReadByOthers) {
				perm |= S_IROTH;
			}
			if (permissions & FilePermissions::WriteByOthers) {
				perm |= S_IWOTH;
			}
			if (permissions & FilePermissions::ExecuteByOthers) {
				perm |= S_IXOTH;
			}
			if (permissions & FilePermissions::ReadByGroup) {
				perm |= S_IRGRP;
			}
			if (permissions & FilePermissions::WriteByGroup) {
				perm |= S_IWGRP;
			}
			if (permissions & FilePermissions::ExecuteByGroup) {
				perm |= S_IXGRP;
			}
			if (permissions & FilePermissions::ReadByUser) {
				perm |= S_IRUSR;
			}
			if (permissions & FilePermissions::WriteByUser) {
				perm |= S_IWUSR;
			}
			if (permissions & FilePermissions::ExecuteByUser) {
				perm |= S_IXUSR;
			}
		}
		
		int fd = ::open(filePath.getData(), flags, perm);
		return (sl_file)fd;
	}

	sl_bool File::_close(sl_file file)
	{
		int fd = (int)file;
		if (fd != -1) {
			::close(fd);
			return sl_true;
		}
		return sl_false;
	}

	sl_uint64 File::getPosition()
	{
		if (isOpened()) {
			int fd = (int)m_file;
#if defined(SLIB_PLATFORM_IS_LINUX)
			off64_t ret = ::lseek64(fd, 0, SEEK_CUR);
#else
			off_t ret = ::lseek(fd, 0, SEEK_CUR);
#endif
			if (ret != -1) {
				return ret;
			}
		}
		return 0;
	}

	sl_bool File::seek(sl_int64 pos, SeekPosition from)
	{
		if (isOpened()) {
			int fd = (int)m_file;
			int origin = SEEK_SET;
			if (from == SeekPosition::Begin) {
				origin = SEEK_SET;
			} else if (from == SeekPosition::Current) {
				origin = SEEK_CUR;
			} else if (from == SeekPosition::End) {
				origin = SEEK_END;
			} else {
				return sl_false;
			}
#if defined(SLIB_PLATFORM_IS_LINUX)
			off64_t ret = ::lseek64(fd, pos, origin);
#else
			off_t ret = ::lseek(fd, pos, origin);
#endif
			if (ret != -1) {
				return sl_true;
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
			int fd = (int)m_file;
			ssize_t n = ::read(fd, buf, size);
			if (n >= 0) {
				if (n > 0) {
					return (sl_int32)n;
				}
			} else {
				int err = errno;
				if (err == EAGAIN || err == EWOULDBLOCK) {
					return 0;
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
			int fd = (int)m_file;
			ssize_t n = ::write(fd, buf, size);
			if (n >= 0) {
				if (n > 0) {
					return (sl_int32)n;
				}
			} else {
				int err = errno;
				if (err == EAGAIN || err == EWOULDBLOCK) {
					return 0;
				}
			}
		}
		return -1;
	}

	sl_bool File::setSize(sl_uint64 newSize)
	{
		if (isOpened()) {
			int fd = (int)m_file;
			return 0 == ::ftruncate(fd, newSize);
		}
		return sl_false;
	}

	sl_uint64 File::getSize(sl_file _fd)
	{
		int fd = (int)_fd;
		if (fd != -1) {
			struct stat st;
			if (0 == ::fstat(fd, &st)) {
				return st.st_size;
			} else {
				return 0;
			}
		} else {
			return 0;
		}
	}
	
	sl_uint64 File::getSize(const String& filePath)
	{
		if (filePath.isEmpty()) {
			return 0;
		}
		struct stat st;
		if (0 == ::stat(filePath.getData(), &st)) {
			return st.st_size;
		} else {
			return 0;
		}
	}
	
	sl_uint64 File::getDiskSize(sl_file _fd)
	{
#if defined(SLIB_PLATFORM_IS_DESKTOP)
#	if defined(SLIB_PLATFORM_IS_MACOS)
		int fd = (int)_fd;
		if (fd != -1) {
			sl_uint64 nSectors = 0;
			::ioctl(fd, DKIOCGETBLOCKCOUNT, &nSectors);
			sl_uint32 nSectorSize = 0;
			::ioctl(fd, DKIOCGETBLOCKSIZE, &nSectorSize);
			return nSectorSize * nSectors;
		}
#	elif defined(SLIB_PLATFORM_IS_LINUX)
		int fd = (int)_fd;
		if (fd != -1) {
			sl_uint64 size = 0;
			::ioctl(fd, BLKGETSIZE64, &size);
			return size;
		}
#	endif
#endif
		return 0;
	}
	
	sl_bool File::lock()
	{
		if (isOpened()) {
			int fd = (int)m_file;
			struct flock fl;
			Base::resetMemory(&fl, 0, sizeof(fl));
			fl.l_start = 0;
			fl.l_len = 0;
			fl.l_type = F_WRLCK;
			fl.l_whence = SEEK_SET;
			if (::fcntl(fd, F_SETLK, &fl) >= 0) {
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool File::unlock()
	{
		if (isOpened()) {
			int fd = (int)m_file;
			struct flock fl;
			Base::resetMemory(&fl, 0, sizeof(fl));
			fl.l_start = 0;
			fl.l_len = 0;
			fl.l_type = F_UNLCK;
			fl.l_whence = SEEK_SET;
			if (::fcntl(fd, F_SETLK, &fl) >= 0) {
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_int64 _priv_File_getModifiedTime(struct stat& st)
	{
#if defined(SLIB_PLATFORM_IS_APPLE)
		sl_int64 t = st.st_mtimespec.tv_sec;
		t *= 1000000;
		t += st.st_mtimespec.tv_nsec / 1000;
#elif defined(SLIB_PLATFORM_IS_ANDROID)
		sl_int64 t = st.st_mtime;
		t *= 1000000;
		t += st.st_mtime_nsec / 1000;
#else
		sl_int64 t = st.st_mtim.tv_sec;
		t *= 1000000;
		t += st.st_mtim.tv_nsec / 1000;
#endif
		return t;
	}

	sl_int64 _priv_File_getAccessedTime(struct stat& st)
	{
#if defined(SLIB_PLATFORM_IS_APPLE)
		sl_int64 t = st.st_atimespec.tv_sec;
		t *= 1000000;
		t += st.st_atimespec.tv_nsec / 1000;
#elif defined(SLIB_PLATFORM_IS_ANDROID)
		sl_int64 t = st.st_atime;
		t *= 1000000;
		t += st.st_atime_nsec / 1000;
#else
		sl_int64 t = st.st_atim.tv_sec;
		t *= 1000000;
		t += st.st_atim.tv_nsec / 1000;
#endif
		return t;
	}

	sl_int64 _priv_File_getCreatedTime(struct stat& st)
	{
#if defined(SLIB_PLATFORM_IS_APPLE)
		sl_int64 t = st.st_ctimespec.tv_sec;
		t *= 1000000;
		t += st.st_ctimespec.tv_nsec / 1000;
#elif defined(SLIB_PLATFORM_IS_ANDROID)
		sl_int64 t = st.st_ctime;
		t *= 1000000;
		t += st.st_ctime_nsec / 1000;
#else
		sl_int64 t = st.st_ctim.tv_sec;
		t *= 1000000;
		t += st.st_ctim.tv_nsec / 1000;
#endif
		return t;
	}

	Time File::getModifiedTime()
	{
		if (isOpened()) {
			int fd = (int)m_file;
			struct stat st;
			if (0 == ::fstat(fd, &st)) {
				return _priv_File_getModifiedTime(st);
			}
		}
		return Time::zero();
	}

	Time File::getModifiedTime(const String& filePath)
	{
		if (filePath.isEmpty()) {
			return Time::zero();
		}
		struct stat st;
		if (0 == ::stat(filePath.getData(), &st)) {
			return _priv_File_getModifiedTime(st);
		} else {
			return Time::zero();
		}
	}

	Time File::getAccessedTime()
	{
		if (isOpened()) {
			int fd = (int)m_file;
			struct stat st;
			if (0 == ::fstat(fd, &st)) {
				return _priv_File_getAccessedTime(st);
			}
		}
		return Time::zero();
	}

	Time File::getAccessedTime(const String& filePath)
	{
		if (filePath.isEmpty()) {
			return Time::zero();
		}
		struct stat st;
		if (0 == ::stat(filePath.getData(), &st)) {
			return _priv_File_getAccessedTime(st);
		} else {
			return Time::zero();
		}
	}

	Time File::getCreatedTime()
	{
		if (isOpened()) {
			int fd = (int)m_file;
			struct stat st;
			if (0 == ::fstat(fd, &st)) {
				return _priv_File_getCreatedTime(st);
			}
		}
		return Time::zero();
	}

	Time File::getCreatedTime(const String& filePath)
	{
		if (filePath.isEmpty()) {
			return Time::zero();
		}
		struct stat st;
		if (0 == ::stat(filePath.getData(), &st)) {
			return _priv_File_getCreatedTime(st);
		} else {
			return Time::zero();
		}
	}

	static sl_bool _priv_File_setAccessedAndModifiedTime(const String& filePath, Time timeAccess, Time timeModify)
	{
		if (filePath.isEmpty()) {
			return sl_false;
		}
		timeval t[2];
		t[0].tv_sec = (int)(timeAccess.toInt() / 1000000);
		t[0].tv_usec = (int)(timeAccess.toInt() % 1000000);
		t[1].tv_sec = (int)(timeModify.toInt() / 1000000);
		t[1].tv_usec = (int)(timeModify.toInt() % 1000000);
		return ::utimes(filePath.getData(), t) == 0;
	}

	sl_bool File::setModifiedTime(const String& filePath, Time time)
	{
		Time timeAccess = getAccessedTime(filePath);
		return _priv_File_setAccessedAndModifiedTime(filePath, timeAccess, time);
	}

	sl_bool File::setAccessedTime(const String& filePath, Time time)
	{
		Time timeModify = getModifiedTime(filePath);
		return _priv_File_setAccessedAndModifiedTime(filePath, time, timeModify);
	}

	sl_bool File::setCreatedTime(const String& filePath, Time time)
	{
		// not supported
		return sl_false;
	}

	FileAttributes File::getAttributes(const String& filePath)
	{
		if (filePath.isEmpty()) {
			return FileAttributes::NotExist;
		}
		struct stat st;
		if (0 == ::stat(filePath.getData(), &st)) {
			int ret = 0;
			if (S_ISDIR(st.st_mode)) {
				ret |= FileAttributes::Directory;
			}
			if (filePath.startsWith('.')) {
				ret |= FileAttributes::Hidden;
			}
			return ret;
		} else {
			return FileAttributes::NotExist;
		}
	}

	sl_bool File::setHidden(const String& filePath, sl_bool flagHidden)
	{
		// not supported
		return sl_false;
	}

	List<String> File::getFiles(const String& _filePath)
	{
		String filePath = _filePath;
		if (filePath.isEmpty()) {
			return sl_null;
		}
		if (File::isDirectory(filePath)) {
			filePath = normalizeDirectoryPath(filePath);
		} else {
			return sl_null;
		}
		List<String> ret;
		String dirPath = filePath;
		DIR* dir = ::opendir(dirPath.getData());
		if (dir) {
			dirent* ent;
			while ((ent = readdir(dir))) {
				ret.add_NoLock(String::fromUtf8(ent->d_name));
			}
			::closedir(dir);
		}
		return ret;
	}

	sl_bool File::_createDirectory(const String& filePath)
	{
		if (filePath.isEmpty()) {
			return sl_false;
		}
		return 0 == ::mkdir(filePath.getData(), 0777);
	}


	sl_bool File::_deleteFile(const String& filePath)
	{
		if (filePath.isEmpty()) {
			return sl_false;
		}
		return 0 == ::remove(filePath.getData());
	}

	sl_bool File::_deleteDirectory(const String& filePath)
	{
		if (filePath.isEmpty()) {
			return sl_false;
		}
		String dirPath = normalizeDirectoryPath(filePath);
		return 0 == ::rmdir(dirPath.getData());
	}

	sl_bool File::rename(const String& oldPath, const String& newPath)
	{
		if (oldPath.isEmpty()) {
			return sl_false;
		}
		if (newPath.isEmpty()) {
			return sl_false;
		}
		return 0 == ::rename(oldPath.getData(), newPath.getData());
	}

	sl_bool File::setNonBlocking(int fd, sl_bool flagEnable)
	{
		int flag = ::fcntl(fd, F_GETFL, 0);
		if (flag != -1) {
			if (flagEnable) {
				flag |= O_NONBLOCK;
			} else {
				flag = flag & ~O_NONBLOCK;
			}
			int ret = ::fcntl(fd, F_SETFL, flag);
			return ret == 0;
		} else {
			return sl_false;
		}
	}

}

#endif
