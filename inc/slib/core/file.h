#ifndef CHECKHEADER_SLIB_CORE_FILE
#define CHECKHEADER_SLIB_CORE_FILE

#include "definition.h"

#include "list.h"
#include "io.h"

typedef sl_reg sl_file;
#define SLIB_FILE_INVALID_HANDLE (sl_file)(-1)

SLIB_NAMESPACE_BEGIN

enum FileMode
{
	fileMode_Read = 1,
	fileMode_Write = 2,
	fileMode_ReadWrite = 3,
	fileMode_Append = 4,
	fileMode_RandomAccess = 5
};

enum FileAttribute
{
	fileAttribute_Directory = 1,
	fileAttribute_Hidden = 2
};

class SLIB_EXPORT File : public IO
{
	SLIB_DECLARE_OBJECT(File, IO)
private:
	String m_path;
	sl_file m_file;

private:
	File();
	~File();

public:
	static Ref<File> open(const String& filePath, FileMode mode);
	
	SLIB_INLINE static Ref<File> openForRead(const String& filePath)
	{
		return open(filePath, fileMode_Read);
	}
	
	SLIB_INLINE static Ref<File> openForWrite(const String& filePath)
	{
		return open(filePath, fileMode_Write);
	}
	
	SLIB_INLINE static Ref<File> openForReadWrite(const String& filePath)
	{
		return open(filePath, fileMode_ReadWrite);
	}
	
	SLIB_INLINE static Ref<File> openForAppend(const String& filePath)
	{
		return open(filePath, fileMode_Append);
	}
	
	SLIB_INLINE static Ref<File> openForRandomAccess(const String& filePath)
	{
		return open(filePath, fileMode_RandomAccess);
	}

public:
	// override
	void close();
	
	SLIB_INLINE sl_bool isOpened() const
	{
		return m_file != SLIB_FILE_INVALID_HANDLE;
	}

	SLIB_INLINE String getPath() const
	{
		return m_path;
	}

	SLIB_INLINE sl_file getHandle() const
	{
		return m_file;
	}
	
	// override
	sl_uint64 getPosition();
	
	// override
	sl_uint64 getSize();
	
	// override
	sl_bool seek(sl_int64 offset, SeekPosition from);

	
	// override
	sl_int32 read32(void* buf, sl_uint32 size);
	
	// override
	sl_int32 write32(const void* buf, sl_uint32 size);

public:
	// works only if the file is already opened
	sl_bool setSize(sl_uint64 size);
	
	sl_bool lock();
	
	sl_bool unlock();

public:
	static sl_uint64 getSize(sl_file fd);
	
	static sl_uint64 getSize(const String& path);

	
	Time getModifiedTime();
	
	Time getAccessedTime();
	
	Time getCreatedTime();
	
	static Time getModifiedTime(const String& filePath);
	
	static Time getAccessedTime(const String& filePath);
	
	static Time getCreatedTime(const String& filePath);
	
	static sl_bool setModifiedTime(const String& filePath, Time time);
	
	static sl_bool setAccessedTime(const String& filePath, Time time);
	
	static sl_bool setCreatedTime(const String& filePath, Time time);

	
	static int getAttributes(const String& filePath);
	
	SLIB_INLINE static sl_bool exists(const String& filePath)
	{
		return getAttributes(filePath) >= 0;
	}
	
	SLIB_INLINE static sl_bool isDirectory(const String& filePath)
	{
		return (getAttributes(filePath) & fileAttribute_Directory) != 0;
	}
	
	SLIB_INLINE static sl_bool isHidden(const String& filePath)
	{
		return (getAttributes(filePath) & fileAttribute_Hidden) != 0;
	}

	static sl_bool setHidden(const String& filePath, sl_bool flagHidden = sl_true);

	
	static sl_bool createDirectory(const String& dirPath);
	
	static sl_bool createDirectories(const String& dirPath);
	
	static sl_bool deleteFile(const String& filePath);
	
	static sl_bool deleteDirectoryOnly(const String& dirPath);

	
	// Deletes the directory and its sub-directories and files
	static sl_bool deleteDirectoryRecursively(const String& dirPath);

	// Changes the path of file or directory. Don't replace the existing file.
	static sl_bool rename(const String& filePathOriginal, const String& filePathNew);

	
	static List<String> getFiles(const String& dirPath);

	static List<String> getAllDescendantFiles(const String& dirPath);

	
	static Memory readAllBytes(const String& path);
	
	static String readUtf8Text(const String& path);
	
	static sl_size writeAllBytes(const String& path, const void* buf, sl_size size);
	
	static sl_size writeAllBytes(const String& path, const Memory& mem);
	
	static sl_bool writeUtf8Text(const String& path, const String& text);
	
	static sl_size appendAllBytes(const String& path, const void* buf, sl_size size);
	
	static sl_size appendAllBytes(const String& path, const Memory& mem);
	
	static sl_bool appendUtf8Text(const String& path, const String& text);
	

	static String getParentDirectoryPath(const String& path);
	
	static String getFileName(const String& path);
	
	static String getFileExtension(const String& path);
	
	static String getFileNameOnly(const String& path);
	
	static String normalizeDirectoryPath(const String& path);

	
	// converts any invalid characters (0~0x1f, 0x7f~0x9f, :*?"<>|\/) into "_"
	static String makeSafeFileName(const String& fileName);

	// converts any invalid characters (0~0x1f, 0x7f~0x9f, :*?"<>|) into "_"
	static String makeSafeFilePath(const String& filePath);

	static String findParentPathContainingFile(const String& basePath, const String& filePath, sl_uint32 nDeep = SLIB_UINT32_MAX);

#if defined(SLIB_PLATFORM_IS_UNIX)
	static sl_bool setNonBlocking(int fd, sl_bool flag);
#endif

private:
	static sl_file _open(const String& filePath, FileMode mode);
	static sl_bool _close(sl_file file);
	
};

class SLIB_EXPORT FilePathSegments
{
public:
	sl_uint32 parentLevel;
	CList<String> segments;

public:
	FilePathSegments();

public:
	void parsePath(const String& path);
	
	String buildPath();

};

SLIB_NAMESPACE_END

#endif //CHECKHEADER_SLIB_CORE_FILE
