#include "../../../inc/slib/core/file.h"

SLIB_NAMESPACE_BEGIN
File::File()
{
	m_file = SLIB_FILE_INVALID_HANDLE;
}

File::~File()
{
	close();
}

Ref<File> File::open(const String& filePath, Mode mode)
{
	sl_file file = _open(filePath, mode);
	if (file != SLIB_FILE_INVALID_HANDLE) {
		Ref<File> ret = new File();
		if (ret.isNotNull()) {
			ret->m_file = file;
			ret->m_path = filePath;
			if (mode == modeAppend) {
				ret->seekToEnd();
			}
			return ret;
		}
		_close(file);
	}
	return Ref<File>::null();
}

void File::close()
{
	if (isOpened()) {
		if (_close(m_file)) {
			m_file = SLIB_FILE_INVALID_HANDLE;
		}
	}
}

sl_uint64 File::getSize()
{
	return getSize(m_file);
}

String File::getParentDirectoryPath(const String& pathName)
{
	if (pathName.isEmpty()) {
		return String::null();
	}
	sl_int32 indexSlash = pathName.lastIndexOf('/');
	sl_int32 indexBackSlash = pathName.lastIndexOf('\\');
	sl_int32 index = -1;
	if (indexSlash != -1) {
		if (indexBackSlash != -1) {
			if (indexSlash < indexBackSlash) {
				index = indexBackSlash;
			} else {
				index = indexSlash;
			}
		} else {
			index = indexSlash;
		}
	} else {
		if (indexBackSlash != -1) {
			index = indexBackSlash;
		}
	}
	if (index == -1) {
		return String::null();
	} else {
		return pathName.substring(0, index);
	}
}

String File::getFileName(const String& pathName)
{
	if (pathName.isEmpty()) {
		return String::null();
	}
	sl_int32 indexSlash = pathName.lastIndexOf('/');
	sl_int32 indexBackSlash = pathName.lastIndexOf('\\');
	sl_int32 index = -1;
	if (indexSlash != -1) {
		if (indexBackSlash != -1) {
			if (indexSlash < indexBackSlash) {
				index = indexBackSlash;
			} else {
				index = indexSlash;
			}
		} else {
			index = indexSlash;
		}
	} else {
		if (indexBackSlash != -1) {
			index = indexBackSlash;
		}
	}
	return pathName.substring(index + 1);
}


String File::getFileExtension(const String& pathName)
{
	String fileName = getFileName(pathName);
	if (fileName.isEmpty()) {
		return String::null();
	}
	sl_int32 index = fileName.lastIndexOf('.');
	if (index > 0) {
		return fileName.substring(index + 1);
	} else {
		return String::null();
	}
}

String File::getFileNameOnly(const String& pathName)
{
	String fileName = getFileName(pathName);
	if (fileName.isEmpty()) {
		return String::null();
	}
	sl_int32 index = fileName.lastIndexOf('.');
	if (index > 0) {
		return fileName.substring(0, index);
	} else {
		return fileName;
	}
}

String File::normalizeDirectoryPath(const String& _str)
{
	String str = _str;
	if (str.endsWith('\\') || str.endsWith('/')) {
		str = str.substring(0, str.getLength() - 1);
	}
	return str;
}
Memory File::readAllBytes(const String& path)
{
	Ref<File> file = File::openForRead(path);
	if (file.isNotNull()) {
#if defined(SLIB_ARCH_IS_64BIT)
		sl_uint64 size = file->getSize();
#else
		sl_uint64 _size = file->getSize();
		if (_size > 0x7fffffff) {
			return Memory::null();
		}
		sl_size size = (sl_size)_size;
#endif
		Memory ret = Memory::create(size);
		if (ret.isNotNull()) {
			char* buf = (char*)(ret.getBuf());
			sl_reg n = file->read(buf, size);
			if (n != size) {
				return Memory::null();
			}
		}
		return ret;
	} else {
		return Memory::null();
	}
}

String File::readUtf8Text(const String& path)
{
	Ref<File> file = File::openForRead(path);
	if (file.isNotNull()) {
		sl_uint64 _size = file->getSize();
		if (_size > SLIB_STR_MAX_LEN) {
			_size = SLIB_STR_MAX_LEN;
		}
		sl_uint32 size = (sl_uint32)_size;
		String ret = String8::allocate(size);
		if (ret.isNotEmpty()) {
			sl_char8* buf = ret.getBuf();
			sl_int32 n;
			if (size >= 3) {
				n = file->read32(buf, 3);
				if (n != 3) {
					return String::null();
				}
				size -= 3;
				if ((sl_uint8)(buf[0]) == 0xEF && (sl_uint8)(buf[1]) == 0xBB && (sl_uint8)(buf[2]) == 0xBF) {
					ret.setLength(size);
					buf[size] = 0;
				} else {
					buf += 3;
				}
			}
			n = file->read32(buf, size);
			if (n != size) {
				return String::null();
			}
		}
		return ret;
	} else {
		return String::null();
	}
}

sl_size File::writeAllBytes(const String& path, const void* buf, sl_size size)
{
	Ref<File> file = File::openForWrite(path);
	if (file.isNotNull()) {
		sl_reg ret = file->write(buf, size);
		if (ret > 0) {
			return ret;
		}
	}
	return 0;
}

sl_size File::writeAllBytes(const String& path, const Memory& mem)
{
	return File::writeAllBytes(path, mem.getBuf(), mem.getSize());
}

sl_bool File::writeUtf8Text(const String& path, const String& text)
{
	sl_size n = text.getLength();
	sl_size ret = File::writeAllBytes(path, text.getBuf(), n);
	return ret == n;
}

sl_size File::appendAllBytes(const String& path, const void* buf, sl_size size)
{
	Ref<File> file = File::openForAppend(path);
	if (file.isNotNull()) {
		sl_reg ret = file->write(buf, size);
		if (ret > 0) {
			return ret;
		}
	}
	return 0;
}

sl_size File::appendAllBytes(const String& path, const Memory& mem)
{
	return File::appendAllBytes(path, mem.getBuf(), mem.getSize());
}

sl_bool File::appendUtf8Text(const String& path, const String& text)
{
	sl_size n = text.getLength();
	sl_size ret = File::appendAllBytes(path, text.getBuf(), n);
	return ret == n;
}

List<String> File::getAllDescendantFiles(const String& dirPath)
{
	if (!isDirectory(dirPath)) {
		return List<String>::null();
	}
	List<String> _listCurrent = getFiles(dirPath);
	_listCurrent.sort(sl_true);
	ListLocker<String> listCurrent(_listCurrent);
	List<String> list;
	for (sl_size i = 0; i < listCurrent.getCount(); i++) {
		String item = listCurrent[i];
		list.add(item);
		String dir = dirPath + "/" + item;
		if (File::isDirectory(dir)) {
			ListLocker<String> sub(File::getAllDescendantFiles(dir));
			for (sl_size j = 0; j < sub.getCount(); j++) {
				list.add(item + "/" + sub[j]);
			}
		}
	}
	return list;
}

sl_bool File::createDirectories(const String& dirPath)
{
	if (File::createDirectory(dirPath)) {
		return sl_true;
	} else {
		String parent = File::getParentDirectoryPath(dirPath);
		if (File::exists(parent)) {
			return sl_false;
		} else {
			if (File::createDirectories(parent)) {
				return File::createDirectory(dirPath);
			} else {
				return sl_false;
			}
		}
	}
}

sl_bool File::deleteDirectoryRecursively(const String& dirPath)
{
	if (File::isDirectory(dirPath)) {
		String path = dirPath + "/";
		ListLocker<String> list(File::getFiles(dirPath));
		sl_bool ret = sl_true;
		for (sl_size i = 0; i < list.getCount(); i++) {
			String sub = path + list[i];
			if (File::exists(sub)) {
				if (File::isDirectory(sub)) {
					ret = ret && File::deleteDirectoryRecursively(sub);
				} else {
					ret = ret && File::deleteFile(sub);
				}
			}
		}
		ret = ret && File::deleteDirectoryOnly(path);
		return ret;
	} else {
		return sl_false;
	}
}

String File::makeSafeFileName(const String& fileName)
{
	String8 ret = fileName.duplicate();
	if (ret.isEmpty()) {
		return ret;
	}
	sl_char8* buf = ret.getBuf();
	sl_uint32 len = ret.getLength();
	for (sl_uint32 i = 0; i < len; i++) {
		sl_uint32 ch = (sl_uint8)(buf[i]);
		if (ch < 0x20) {
			ch = '_';
		} else if (ch >= 0x7f && ch < 0xA0) {
			ch = '_';
		} else {
			switch (ch) {
			case '\\':
			case '/':
			case ':':
			case '*':
			case '?':
			case '"':
			case '<':
			case '>':
			case '|':
				ch = '_';
				break;
			}
		}
	}
	return ret;
}

String File::makeSafeFilePath(const String& filePath)
{
	String8 ret = filePath.duplicate();
	if (ret.isEmpty()) {
		return ret;
	}
	sl_char8* buf = ret.getBuf();
	sl_uint32 len = ret.getLength();
	for (sl_uint32 i = 0; i < len; i++) {
		sl_uint32 ch = (sl_uint8)(buf[i]);
		if (ch < 0x20) {
			ch = '_';
		} else if (ch >= 0x7f && ch < 0xA0) {
			ch = '_';
		} else {
			switch (ch) {
			case ':':
			case '*':
			case '?':
			case '"':
			case '<':
			case '>':
			case '|':
				ch = '_';
				break;
			}
		}
	}
	return ret;
}

String File::findParentPathContainingFile(const String& basePath, const String& filePath, sl_uint32 nDeep)
{
	FilePathSegments segments;
	segments.parsePath(basePath);
	if (nDeep > segments.segments.getCount()) {
		nDeep = (sl_uint32)(segments.segments.getCount());
	}
	for (sl_uint32 i = 0; i <= nDeep; i++) {
		String path = segments.buildPath();
		if (File::exists(path + "/" + filePath)) {
			return path;
		}
		segments.segments.popBack();
	}
	return String::null();
}


FilePathSegments::FilePathSegments()
{
	parentLevel = 0;
}

void FilePathSegments::parsePath(const String& path)
{
	parentLevel = 0;
	segments.removeAll();

	sl_char8* buf = path.getBuf();
	sl_uint32 len = path.getLength();
	sl_uint32 pos = 0;
	sl_uint32 start = 0;
	for (; pos <= len; pos++) {
		sl_char8 ch;
		if (pos == len) {
			ch = '/';
		} else {
			ch = buf[pos];
		}
		if (ch == '/' || ch == '\\') {
			if (pos == 0) {
				segments.add(String::null());
			} else {
				sl_uint32 n = pos - start;
				if (n > 0) {
					if (n == 1 && buf[start] == '.') {
					} else if (n == 2 && buf[start] == '.' && buf[start + 1] == '.') {
						if (segments.count() > 0) {
							segments.popBack();
						} else {
							parentLevel++;
						}
					} else {
						String s(buf + start, n);
						segments.add(s);
					}
				}
			}
			start = pos + 1;
		}
	}
}

String FilePathSegments::buildPath()
{
	StringBuffer ret;
	SLIB_STATIC_STRING(sep, "/");
	sl_bool flagFirst = sl_true;
	{
		SLIB_STATIC_STRING(p, "..");
		for (sl_uint32 i = 0; i < parentLevel; i++) {
			if (!flagFirst) {
				ret.add(sep);
			}
			ret.add(p);
			flagFirst = sl_false;
		}
	}
	{
		ListLocker<String> list(segments);
		for (sl_size i = 0; i < list.count(); i++) {
			if (!flagFirst) {
				ret.add(sep);
			}
			ret.add(list[i]);
			flagFirst = sl_false;
		}
	}
	return ret.merge();
}

SLIB_NAMESPACE_END
