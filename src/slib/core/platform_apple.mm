#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/core/platform_apple.h"

#include "../../../inc/slib/core/file.h"

namespace slib
{

	NSString* Apple::getNSStringFromString(const String& str, NSString* def)
	{
		String s = str;
		if (s.isNull()) {
			return def;
		}
		NSString* ret = [[NSString alloc] initWithUTF8String:s.getData()];
		if (ret != nil) {
			return ret;
		}
		return def;
	}

	NSString* Apple::getNSStringFromString16(const String16& str, NSString* def)
	{
		String16 s = str;
		if (s.isNull()) {
			return def;
		}
		NSString* ret = [[NSString alloc] initWithCharacters:(unichar*)s.getData() length:s.getLength()];
		if (ret != nil) {
			return ret;
		}
		return def;
	}

	String Apple::getStringFromNSString(NSString* str)
	{
		if (str == nil) {
			return sl_null;
		}
		const char* buf = [str UTF8String];
		return String::fromUtf8(buf);
	}

	String16 Apple::getString16FromNSString(NSString* str)
	{
		if (str == nil) {
			return sl_null;
		}
		sl_size len = (sl_size)([str length]);
		String16 ret = String16::allocate(len);
		if (ret.isNull()) {
			return ret;
		}
		NSRange range;
		range.length = len;
		range.location = 0;
		[str getCharacters:ret.getData() range:range];
		return ret;
	}

	String Apple::getFilePathFromNSURL(NSURL* url)
	{
		if (url != nil) {
			SLIB_STATIC_STRING(prefix, "file://");
			String path = Apple::getStringFromNSString([url path]);
			if (path.startsWith(prefix)) {
				path = path.substring(7);
			}
			return path;
		}
		return sl_null;
	}

	Memory Apple::getMemoryFromNSData(NSData* data)
	{
		if (data != nil) {
			sl_size n = (sl_size)([data length]);
			if (n > 0) {
				Memory mem = Memory::create(n);
				if (mem.isNotNull()) {
					char* p = (char*)(mem.getData());
					[data enumerateByteRangesUsingBlock:^(const void* bytes, NSRange byteRange, BOOL* stop) {
						Base::copyMemory(p + byteRange.location, bytes, byteRange.length);
					}];
					return mem;
				}
			}
		}
		return sl_null;
	}

	NSData* Apple::getNSDataFromMemory(const Memory& memory)
	{
		if(memory.isNotNull()) {
			return [NSData dataWithBytes:memory.getData() length:memory.getSize()];
		}
		
		return nil;
	}

	String Apple::getAssetFilePath(const String &path)
	{
		String fileExt = File::getFileExtension(path);
		String fileName = File::getFileNameOnly(path);
		String dirPath = File::getParentDirectoryPath(path);
		
		NSString* strFileName = getNSStringFromString(fileName);
		NSString* strFolderPath = getNSStringFromString(dirPath);
		NSString* strFileExtension = getNSStringFromString(fileExt);
		
		NSString *filePath = [[NSBundle mainBundle] pathForResource:strFileName ofType:strFileExtension inDirectory:strFolderPath];
		return Apple::getStringFromNSString(filePath);
	}

	SLIB_STATIC_STRING_NULL(_g_system_version);
	sl_uint32 _g_system_version_major = 0;
	sl_uint32 _g_system_version_minor = 0;
	sl_bool _g_system_flagInitVersion = sl_true;

	void _Apple_initSystemVersion()
	{
		if (_g_system_flagInitVersion) {
#if defined(SLIB_PLATFORM_IS_OSX)
			double v = NSAppKitVersionNumber;
			if (v >= NSAppKitVersionNumber10_10) {
				_g_system_version = "10.10";
				_g_system_version_major = 10;
				_g_system_version_minor = 10;
			} else if (v >= NSAppKitVersionNumber10_9) {
				_g_system_version = "10.9";
				_g_system_version_major = 10;
				_g_system_version_minor = 9;
			} else if (v >= NSAppKitVersionNumber10_8) {
				_g_system_version = "10.8";
				_g_system_version_major = 10;
				_g_system_version_minor = 8;
			} else if (v >= NSAppKitVersionNumber10_7) {
				_g_system_version = "10.7";
				_g_system_version_major = 10;
				_g_system_version_minor = 7;
			} else if (v >= NSAppKitVersionNumber10_6) {
				_g_system_version = "10.6";
				_g_system_version_major = 10;
				_g_system_version_minor = 6;
			}
#elif defined(SLIB_PLATFORM_IS_IOS)
			NSString* _version = [[UIDevice currentDevice] systemVersion];
			String version = Apple::getStringFromNSString(_version);
			if (version.isNotEmpty()) {
				ListLocker<String> list(version.split("."));
				if (list.count > 0) {
					_g_system_version_major = list[0].parseUint32();
					if (list.count > 1) {
						_g_system_version_minor = list[1].parseUint32();
					}
				}
			}
			_g_system_version = version;
#endif
			_g_system_flagInitVersion = sl_false;
		}
	}

	String Apple::getSystemVersion()
	{
		_Apple_initSystemVersion();
		return _g_system_version;
	}

	sl_uint32 Apple::getSystemMajorVersion()
	{
		_Apple_initSystemVersion();
		return _g_system_version_major;
	}

	sl_uint32 Apple::getSystemMinorVersion()
	{
		_Apple_initSystemVersion();
		return _g_system_version_minor;
	}

	String Apple::getMainBundlePath()
	{
		NSString* path = [[NSBundle mainBundle] bundlePath];
		return getStringFromNSString(path);
	}

}

#endif
