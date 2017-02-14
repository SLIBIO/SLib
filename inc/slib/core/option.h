#ifndef CHECKHEADER_SLIB_CORE_OPTION
#define CHECKHEADER_SLIB_CORE_OPTION

#if !defined(_MSC_VER) && __cplusplus < 201103
#	error "Slib needs at least a C++11 compliant compiler. Please enable C++11 in Project Settings"
#endif

/* Platforms */
// Windows Platforms
#define SLIB_PLATFORM_WIN32			0x01010000
#define SLIB_PLATFORM_WIN64			0x01020000
#define SLIB_PLATFORM_CYGWIN_WIN32	0x01A10000
#define SLIB_PLATFORM_CYGWIN_WIN64	0x01A20000
#define SLIB_PLATFORM_WP8			0x01880000

// Unix Platforms
#define SLIB_PLATFORM_LINUX			0x02000000	/* RedHat, Fedora, CentOS, Debian, Ubuntu ... */
#define SLIB_PLATFORM_FREEBSD		0x03000000
#define SLIB_PLATFORM_SOLARIS		0x04000000
// macOS&iOS Platforms
#define SLIB_PLATFORM_OSX			0x08100000
#define SLIB_PLATFORM_MACOS			SLIB_PLATFORM_OSX
#define SLIB_PLATFORM_IOS			0x08210000
#define SLIB_PLATFORM_IOS_SIMULATOR	0x08220000
// Android Platforms
#define SLIB_PLATFORM_ANDROID		0x0A010000
// Tizen Platform
#define SLIB_PLATFORM_TIZEN		    0x0B010000

#define SLIB_PLATFORM_TYPE			(SLIB_PLATFORM & 0xFFFF0000)
#define SLIB_PLATFORM_VERSION		(SLIB_PLATFORM & 0x0000FFFF)

/* Compilers */
#define SLIB_COMPILER_VC		0x00010000
#define SLIB_COMPILER_GCC		0x00020000
#define SLIB_COMPILER_OBJC		0x00030000

#define SLIB_COMPILER_TYPE		(SLIB_COMPILER & 0xFFFF0000)
#define SLIB_COMPILER_VERSION	(SLIB_COMPILER & 0x0000FFFF)

#define SLIB_COMPILER_VC6		0x00010060
#define SLIB_COMPILER_VC7		0x00010070
#define SLIB_COMPILER_VC8		0x00010080
#define SLIB_COMPILER_VC9		0x00010090
#define SLIB_COMPILER_VC10		0x000100A0
#define SLIB_COMPILER_VC11		0x000100B0
#define SLIB_COMPILER_VC12		0x000100C0

/*************************************
	Platform Definition
**************************************/
#ifndef SLIB_PLATFORM
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#		define SLIB_PLATFORM			SLIB_PLATFORM_WP8
#	elif defined(_WIN64)
#		if defined(__CYGWIN__)
#			define SLIB_PLATFORM		SLIB_PLATFORM_CYGWIN_WIN64
#		else
#			define SLIB_PLATFORM		SLIB_PLATFORM_WIN64
#		endif
#	elif defined(_WIN32)
#		if defined(__CYGWIN__)
#			define SLIB_PLATFORM		SLIB_PLATFORM_CYGWIN_WIN32
#		else
#			define SLIB_PLATFORM		SLIB_PLATFORM_WIN32
#		endif
#	elif defined(__APPLE__) && defined(__MACH__)
#		include <TargetConditionals.h>
#		if TARGET_OS_IOS
#			if TARGET_OS_SIMULATOR
#				define SLIB_PLATFORM	SLIB_PLATFORM_IOS_SIMULATOR
#			else
#				define SLIB_PLATFORM	SLIB_PLATFORM_IOS
#			endif
#		elif TARGET_OS_MAC
#			define SLIB_PLATFORM		SLIB_PLATFORM_MACOS
#		endif
#	elif defined(__ANDROID__)
#		define SLIB_PLATFORM			SLIB_PLATFORM_ANDROID
#	elif defined(TIZEN_DEPRECATION)
#		define SLIB_PLATFORM			SLIB_PLATFORM_TIZEN
#	elif defined(__unix__) && defined(BSD)
#		define SLIB_PLATFORM			SLIB_PLATFORM_FREEBSD
#	elif defined(__unix__) && defined(__sun) && defined(__SVR4)
#		define SLIB_PLATFORM			SLIB_PLATFORM_SOLARIS
#	elif defined(__linux__)
#		define SLIB_PLATFORM			SLIB_PLATFORM_LINUX
#	endif
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_WIN32)
#	define SLIB_PLATFORM_IS_WIN32
#	define SLIB_PLATFORM_IS_WINDOWS
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_CYGWIN_WIN32)
#	define SLIB_PLATFORM_IS_WIN32
#	define SLIB_PLATFORM_IS_WINDOWS
#	define SLIB_PLATFORM_IS_CYGWIN
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_WIN64)
#	define SLIB_PLATFORM_IS_WIN32
#	define SLIB_PLATFORM_IS_WIN64
#	define SLIB_PLATFORM_IS_WINDOWS
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_CYGWIN_WIN64)
#	define SLIB_PLATFORM_IS_WIN32
#	define SLIB_PLATFORM_IS_WIN64
#	define SLIB_PLATFORM_IS_WINDOWS
#	define SLIB_PLATFORM_IS_CYGWIN
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_WP8)
#	define SLIB_PLATFORM_IS_WP8
#	define SLIB_PLATFORM_IS_WINDOWS
#	define SLIB_PLATFORM_IS_MOBILE
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_LINUX)
#	define SLIB_PLATFORM_IS_LINUX
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_FREEBSD)
#	define SLIB_PLATFORM_IS_FREEBSD
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_SOLARIS)
#	define SLIB_PLATFORM_IS_SOLARIS
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_MACOS)
#	define SLIB_PLATFORM_IS_OSX
#	define SLIB_PLATFORM_IS_MACOS
#	define SLIB_PLATFORM_IS_APPLE
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_IOS)
#	define SLIB_PLATFORM_IS_IOS
#	define SLIB_PLATFORM_IS_IOS_DEVICE
#	define SLIB_PLATFORM_IS_APPLE
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_MOBILE
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_IOS_SIMULATOR)
#	define SLIB_PLATFORM_IS_IOS
#	define SLIB_PLATFORM_IS_IOS_SIMULATOR
#	define SLIB_PLATFORM_IS_APPLE
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_MOBILE
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_ANDROID)
#	define SLIB_PLATFORM_IS_ANDROID
#	define SLIB_PLATFORM_IS_LINUX
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_MOBILE
#	define SLIB_PLATFORM_USE_JNI
#endif
#if (SLIB_PLATFORM_TYPE == SLIB_PLATFORM_TIZEN)
#	define SLIB_PLATFORM_IS_TIZEN
#	define SLIB_PLATFORM_IS_LINUX
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_MOBILE
#endif


#if defined(SLIB_PLATFORM_IS_DESKTOP)
#	define SLIB_IF_PLATFORM_IS_DESKTOP(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_DESKTOP(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_MOBILE)
#	define SLIB_IF_PLATFORM_IS_MOBILE(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_MOBILE(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#	define SLIB_IF_PLATFORM_IS_WINDOWS(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_WINDOWS(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_WIN32)
#	define SLIB_IF_PLATFORM_IS_WIN32(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_WIN32(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_UNIX)
#	define SLIB_IF_PLATFORM_IS_UNIX(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_UNIX(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_LINUX)
#	define SLIB_IF_PLATFORM_IS_LINUX(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_LINUX(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_ANDROID)
#	define SLIB_IF_PLATFORM_IS_ANDROID(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_ANDROID(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_APPLE)
#	define SLIB_IF_PLATFORM_IS_APPLE(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_APPLE(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_MACOS)
#	define SLIB_IF_PLATFORM_IS_MACOS(Y, N) Y
#	define SLIB_IF_PLATFORM_IS_OSX(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_MACOS(Y, N) N
#	define SLIB_IF_PLATFORM_IS_OSX(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_IOS)
#	define SLIB_IF_PLATFORM_IS_IOS(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_IOS(Y, N) N
#endif

#if defined(SLIB_PLATFORM_IS_TIZEN)
#	define SLIB_IF_PLATFORM_IS_TIZEN(Y, N) Y
#else
#	define SLIB_IF_PLATFORM_IS_TIZEN(Y, N) N
#endif

/*************************************
	Compiler Definition
**************************************/
#ifndef SLIB_COMPILER
#	if defined(_MSC_VER)
#		define SLIB_COMPILER		SLIB_COMPILER_VC
#	elif defined(__GNUC__)
#		if defined(__OBJC__)
#			define SLIB_COMPILER	SLIB_COMPILER_OBJC
#		else
#			define SLIB_COMPILER	SLIB_COMPILER_GCC
#		endif
#	endif
#endif
// Microsoft Visual Studio Sub-versions
#if (SLIB_COMPILER == SLIB_COMPILER_VC)
#undef SLIB_COMPILER
#	ifdef _MSC_VER
#		if _MSC_VER >= 1800
#			define SLIB_COMPILER				SLIB_COMPILER_VC12
#		elif _MSC_VER >= 1700
#			define SLIB_COMPILER				SLIB_COMPILER_VC11
#		elif _MSC_VER >= 1600
#			define SLIB_COMPILER				SLIB_COMPILER_VC10
#		elif _MSC_VER >= 1500
#			define SLIB_COMPILER				SLIB_COMPILER_VC9
#		elif _MSC_VER >= 1400
#			define SLIB_COMPILER				SLIB_COMPILER_VC8
#		elif _MSC_VER >= 1300
#			define SLIB_COMPILER				SLIB_COMPILER_VC7
#		elif _MSC_VER >= 1200
#			define SLIB_COMPILER				SLIB_COMPILER_VC6
#		endif
#	endif
#endif
#if (SLIB_COMPILER_TYPE == SLIB_COMPILER_VC)
#	define SLIB_COMPILER_IS_VC
#endif
#if (SLIB_COMPILER_TYPE == SLIB_COMPILER_GCC)
#	define SLIB_COMPILER_IS_GCC
#endif
#if (SLIB_COMPILER_TYPE == SLIB_COMPILER_OBJC)
#	define SLIB_COMPILER_IS_GCC
#	define SLIB_COMPILER_IS_OBJC
#endif

#if defined(SLIB_COMPILER_IS_VC)
#	define SLIB_IF_COMPILER_IS_VC(Y, N) Y
#else
#	define SLIB_IF_COMPILER_IS_VC(Y, N) N
#endif

#if defined(SLIB_COMPILER_IS_GCC)
#	define SLIB_IF_COMPILER_IS_GCC(Y, N) Y
#else
#	define SLIB_IF_COMPILER_IS_GCC(Y, N) N
#endif

#if defined(SLIB_COMPILER_IS_OBJC)
#	define SLIB_IF_COMPILER_IS_OBJC(Y, N) Y
#else
#	define SLIB_IF_COMPILER_IS_OBJC(Y, N) N
#endif

/*************************************
	Architecture Definition
**************************************/
#if defined(SLIB_COMPILER_IS_GCC)
#	if defined(__arm__) || defined(__aarch64__) || defined(__arm64__) // arm
#		define SLIB_ARCH_IS_ARM
#		if defined(__aarch64__) || defined(__arm64__)
#			define SLIB_ARCH_IS_ARM64
#		endif
#	elif defined(__ia64__) // itanium (IA64)
#		define SLIB_ARCH_IS_IA64
#	elif defined(__amd64__) || defined(__x86_64__) // AMD64(x86_64)
#		define SLIB_ARCH_IS_X64
#	elif defined(__i386__) || defined(_X86_) // x86
#		define SLIB_ARCH_IS_X86
#	elif defined(__mips__)
#		define SLIB_ARCH_IS_MIPS
#	endif
#	if defined(__LP64) || defined(__LP64__)
#		define SLIB_ARCH_IS_64BIT
#	else
#		define SLIB_ARCH_IS_32BIT
#	endif
#elif defined(SLIB_COMPILER_IS_VC)
#	if (defined(_M_ARM)) // arm
#		define SLIB_ARCH_IS_ARM
#	elif (defined(_M_IA64)) // itanium (IA64)
#		define SLIB_ARCH_IS_IA64
#	elif (defined(_M_IX86))  // x86
#		define SLIB_ARCH_IS_X86
#	elif (defined(_M_X64) || defined(_M_AMD64))  // AMD64(x86_64)
#		define SLIB_ARCH_IS_X64
#	endif
#	ifdef SLIB_PLATFORM_IS_WIN64
#		define SLIB_ARCH_IS_64BIT
#	else
#		ifdef SLIB_PLATFORM_IS_WIN32
#			define SLIB_ARCH_IS_32BIT
#		endif
#	endif
#endif

#if defined(SLIB_ARCH_IS_32BIT)
#	define SLIB_IF_ARCH_IS_32BIT(Y, N) Y
#else
#	define SLIB_IF_ARCH_IS_32BIT(Y, N) N
#endif

#if defined(SLIB_ARCH_IS_64BIT)
#	define SLIB_IF_ARCH_IS_64BIT(Y, N) Y
#else
#	define SLIB_IF_ARCH_IS_64BIT(Y, N) N
#endif

#if defined(SLIB_ARCH_IS_ARM)
#	define SLIB_IF_ARCH_IS_ARM(Y, N) Y
#else
#	define SLIB_IF_ARCH_IS_ARM(Y, N) N
#endif

#if defined(SLIB_ARCH_IS_X86)
#	define SLIB_IF_ARCH_IS_X86(Y, N) Y
#else
#	define SLIB_IF_ARCH_IS_X86(Y, N) N
#endif

#if defined(SLIB_ARCH_IS_X64)
#	define SLIB_IF_ARCH_IS_X64(Y, N) Y
#else
#	define SLIB_IF_ARCH_IS_X64(Y, N) N
#endif

#endif
