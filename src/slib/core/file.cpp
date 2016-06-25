#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(File, IO)

File::File()
{
	m_file = SLIB_FILE_INVALID_HANDLE;
}

File::~File()
{
	close();
}

Ref<File> File::open(const String& filePath, FileMode mode)
{
	sl_file file = _open(filePath, mode);
	if (file != SLIB_FILE_INVALID_HANDLE) {
		Ref<File> ret = new File();
		if (ret.isNotNull()) {
			ret->m_file = file;
			ret->m_path = filePath;
			if (mode == FileMode::Append) {
				ret->seekToEnd();
			}
			return ret;
		}
		_close(file);
	}
	return Ref<File>::null();
}


Ref<File> File::openForRead(const String& filePath)
{
	return open(filePath, FileMode::Read);
}

Ref<File> File::openForWrite(const String& filePath)
{
	return open(filePath, FileMode::Write);
}

Ref<File> File::openForReadWrite(const String& filePath)
{
	return open(filePath, FileMode::ReadWrite);
}

Ref<File> File::openForAppend(const String& filePath)
{
	return open(filePath, FileMode::Append);
}

Ref<File> File::openForRandomAccess(const String& filePath)
{
	return open(filePath, FileMode::RandomAccess);
}

void File::close()
{
	if (isOpened()) {
		if (_close(m_file)) {
			m_file = SLIB_FILE_INVALID_HANDLE;
		}
	}
}

sl_bool File::isOpened() const
{
	return m_file != SLIB_FILE_INVALID_HANDLE;
}

String File::getPath() const
{
	return m_path;
}

sl_file File::getHandle() const
{
	return m_file;
}

sl_uint64 File::getSize()
{
	return getSize(m_file);
}

sl_bool File::exists(const String& filePath)
{
	return (getAttributes(filePath) & FileAttributes::NotExist) == 0;
}

sl_bool File::isDirectory(const String& filePath)
{
	return (getAttributes(filePath) & FileAttributes::Directory) != 0;
}

sl_bool File::isHidden(const String& filePath)
{
	return (getAttributes(filePath) & FileAttributes::Hidden) != 0;
}

String File::getParentDirectoryPath(const String& pathName)
{
	if (pathName.isEmpty()) {
		return String::null();
	}
	sl_reg indexSlash = pathName.lastIndexOf('/');
	sl_reg indexBackSlash = pathName.lastIndexOf('\\');
	sl_reg index = -1;
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
	sl_reg indexSlash = pathName.lastIndexOf('/');
	sl_reg indexBackSlash = pathName.lastIndexOf('\\');
	sl_reg index = -1;
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
	sl_reg index = fileName.lastIndexOf('.');
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
	sl_reg index = fileName.lastIndexOf('.');
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
		if (size == 0) {
			return Memory::null();
		}
		Memory ret = Memory::create(size);
		if (ret.isNotNull()) {
			char* buf = (char*)(ret.getData());
			if (file->read(buf, size) == size) {
				return ret;
			}
		}
	}
	return Memory::null();
}

String File::readAllTextUTF8(const String& path)
{
	Ref<File> file = File::openForRead(path);
	if (file.isNotNull()) {
#if defined(SLIB_ARCH_IS_64BIT)
		sl_uint64 size = file->getSize();
#else
		sl_uint64 _size = file->getSize();
		if (_size > 0x7fffffff) {
			return String::null();
		}
		sl_size size = (sl_size)_size;
#endif
		return file->readTextUTF8(size);
	}
	return String::null();
}

String16 File::readAllTextUTF16(const String& path, sl_bool flagBigEndian)
{
	Ref<File> file = File::openForRead(path);
	if (file.isNotNull()) {
#if defined(SLIB_ARCH_IS_64BIT)
		sl_uint64 size = file->getSize();
#else
		sl_uint64 _size = file->getSize();
		if (_size > 0x7fffffff) {
			return String16::null();
		}
		sl_size size = (sl_size)_size;
#endif
		return file->readTextUTF16(size, flagBigEndian);
	}
	return String16::null();
}

String File::readAllText(const String& path, Charset* outCharset)
{
	Ref<File> file = File::openForRead(path);
	if (file.isNotNull()) {
#if defined(SLIB_ARCH_IS_64BIT)
		sl_uint64 size = file->getSize();
#else
		sl_uint64 _size = file->getSize();
		if (_size > 0x7fffffff) {
			return String::null();
		}
		sl_size size = (sl_size)_size;
#endif
		return file->readText(size, outCharset);
	}
	return String::null();
}

String16 File::readAllText16(const String& path, Charset* outCharset)
{
	Ref<File> file = File::openForRead(path);
	if (file.isNotNull()) {
#if defined(SLIB_ARCH_IS_64BIT)
		sl_uint64 size = file->getSize();
#else
		sl_uint64 _size = file->getSize();
		if (_size > 0x7fffffff) {
			return String::null();
		}
		sl_size size = (sl_size)_size;
#endif
		return file->readText16(size, outCharset);
	}
	return String16::null();
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
	return File::writeAllBytes(path, mem.getData(), mem.getSize());
}

sl_bool File::writeAllTextUTF8(const String& path, const String& text, sl_bool flagWriteByteOrderMark)
{
	Ref<File> file = File::openForWrite(path);
	if (file.isNotNull()) {
		return file->writeTextUTF8(text, flagWriteByteOrderMark);
	}
	return sl_false;
}

sl_bool File::writeAllTextUTF16LE(const String& path, const String16& text, sl_bool flagWriteByteOrderMark)
{
	Ref<File> file = File::openForWrite(path);
	if (file.isNotNull()) {
		return file->writeTextUTF16LE(text, flagWriteByteOrderMark);
	}
	return sl_false;
}

sl_bool File::writeAllTextUTF16BE(const String& path, const String16& text, sl_bool flagWriteByteOrderMark)
{
	Ref<File> file = File::openForWrite(path);
	if (file.isNotNull()) {
		return file->writeTextUTF16BE(text, flagWriteByteOrderMark);
	}
	return sl_false;
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
	return File::appendAllBytes(path, mem.getData(), mem.getSize());
}

sl_bool File::appendAllTextUTF8(const String& path, const String& text)
{
	Ref<File> file = File::openForAppend(path);
	if (file.isNotNull()) {
		return file->writeTextUTF8(text, sl_false);
	}
	return sl_false;
}

sl_bool File::appendAllTextUTF16LE(const String& path, const String16& text)
{
	Ref<File> file = File::openForAppend(path);
	if (file.isNotNull()) {
		return file->writeTextUTF16LE(text, sl_false);
	}
	return sl_false;
}

sl_bool File::appendAllTextUTF16BE(const String& path, const String16& text)
{
	Ref<File> file = File::openForAppend(path);
	if (file.isNotNull()) {
		return file->writeTextUTF16BE(text, sl_false);
	}
	return sl_false;
}

List<String> File::getAllDescendantFiles(const String& dirPath)
{
	if (!isDirectory(dirPath)) {
		return List<String>::null();
	}
	List<String> ret;
	List<String> listCurrent = getFiles(dirPath);
	listCurrent.sort_NoLock(sl_true);
	String* p = listCurrent.getData();
	sl_size n = listCurrent.getCount();
	for (sl_size i = 0; i < n; i++) {
		String& item = p[i];
		ret.add_NoLock(item);
		String dir = dirPath + "/" + item;
		if (File::isDirectory(dir)) {
			ListItems<String> sub(File::getAllDescendantFiles(dir));
			for (sl_size j = 0; j < sub.count; j++) {
				ret.add(item + "/" + sub[j]);
			}
		}
	}
	return ret;
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
		ListItems<String> list(File::getFiles(dirPath));
		sl_bool ret = sl_true;
		for (sl_size i = 0; i < list.count; i++) {
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
	sl_char8* buf = ret.getData();
	sl_size len = ret.getLength();
	for (sl_size i = 0; i < len; i++) {
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
	sl_char8* buf = ret.getData();
	sl_size len = ret.getLength();
	for (sl_size i = 0; i < len; i++) {
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

	sl_char8* buf = path.getData();
	sl_size len = path.getLength();
	sl_size pos = 0;
	sl_size start = 0;
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
				sl_size n = pos - start;
				if (n > 0) {
					if (n == 1 && buf[start] == '.') {
					} else if (n == 2 && buf[start] == '.' && buf[start + 1] == '.') {
						if (segments.getCount() > 0) {
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
		for (sl_uint32 i = 0; i < parentLevel; i++) {
			if (!flagFirst) {
				ret.add(sep);
			}
			SLIB_STATIC_STRING(p, "..");
			ret.add(p);
			flagFirst = sl_false;
		}
	}
	{
		ListLocker<String> list(segments);
		for (sl_size i = 0; i < list.count; i++) {
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
