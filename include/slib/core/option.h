/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

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

#if defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
#	define SLIB_ARCH_IS_X64
#	define SLIB_ARCH_IS_64BIT
#	define SLIB_ARCH_IS_LITTLE_ENDIAN
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_X86_) || defined(_M_IX86) || defined(EMSCRIPTEN)
#	define SLIB_ARCH_IS_X86
#	define SLIB_ARCH_IS_LITTLE_ENDIAN
#elif defined(__arm64__) || defined(__arm64) || defined(__aarch64__)
#	define SLIB_ARCH_IS_ARM64
#	define SLIB_ARCH_IS_64BIT
#	if defined(__AARCH64EL__)
#		define SLIB_ARCH_IS_LITTLE_ENDIAN
#	elif defined(__AARCH64EB__) || defined(__ARMEB__)
#		define SLIB_ARCH_IS_BIG_ENDIAN
#	endif
#elif defined(__arm__) || defined(__arm) || defined(ARM) || defined(_ARM_) || defined(__ARM__) || defined(_M_ARM)
#	define SLIB_ARCH_IS_ARM
#	if defined(__ARMEB__)
#		define SLIB_ARCH_IS_BIG_ENDIAN
#	endif
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__) || defined(_M_MRX000)
#	define SLIB_ARCH_IS_MIPS
#	if defined(__MIPSEB__) || defined(_MIPSEB) || defined(__MIPSEB)
#		define SLIB_ARCH_IS_BIG_ENDIAN
#	elif defined(__MIPSEL__) || defined(_MIPSEL) || defined(__MIPSEL)
#		define SLIB_ARCH_IS_LITTLE_ENDIAN
#	endif
#elif defined(_IA64) || defined(__IA64__) || defined(__ia64__) || defined(__ia64) || defined(_M_IA64)
#	define SLIB_ARCH_IS_IA64
#	define SLIB_ARCH_IS_64BIT
#	if defined(hpux) || defined(_hpux)
#		define SLIB_ARCH_IS_BIG_ENDIAN
#	else
#		define SLIB_ARCH_IS_LITTLE_ENDIAN
#	endif
#elif defined(__ALPHA) || defined(__alpha) || defined(__alpha__) || defined(_M_ALPHA)
#	define SLIB_ARCH_IS_ALPHA
#	define SLIB_ARCH_IS_LITTLE_ENDIAN
#elif defined(__hppa) || defined(__hppa__)
#	define SLIB_ARCH_IS_HPPA
#	define SLIB_ARCH_IS_BIG_ENDIAN
#elif defined(__PPC) || defined(__POWERPC__) || defined(__powerpc) || defined(__PPC__) || defined(__powerpc__) || defined(__ppc__) || defined(__ppc) || defined(_ARCH_PPC) || defined(_M_PPC)
#	define SLIB_ARCH_IS_POWERPC
#elif defined(_POWER) || defined(_ARCH_PWR) || defined(_ARCH_PWR2) || defined(_ARCH_PWR3) || defined(_ARCH_PWR4) || defined(__THW_RS6000)
#	define SLIB_ARCH_IS_POWER
#	define SLIB_ARCH_IS_BIG_ENDIAN
#elif defined(__sparc__) || defined(__sparc) || defined(sparc)
#	define SLIB_ARCH_IS_SPARC
#	define SLIB_ARCH_IS_BIG_ENDIAN
#elif defined(__m68k__)
#	define SLIB_ARCH_IS_M68K
#	define SLIB_ARCH_IS_BIG_ENDIAN
#elif defined(__s390__)
#	define SLIB_ARCH_IS_S390
#	define SLIB_ARCH_IS_BIG_ENDIAN
#elif defined(__sh__) || defined(__sh) || defined(SHx) || defined(_SHX_)
#	define SLIB_ARCH_IS_SH
#	if defined(__LITTLE_ENDIAN__)
#		define SLIB_ARCH_IS_LITTLE_ENDIAN
#	else
#		define SLIB_ARCH_IS_BIG_ENDIAN
#	endif
#elif defined (nios2) || defined(__nios2) || defined(__nios2__)
#	define SLIB_ARCH_IS_NIOS2
#	if defined(__nios2_little_endian) || defined(nios2_little_endian) || defined(__nios2_little_endian__)
#		define SLIB_ARCH_IS_LITTLE_ENDIAN
#	else
#		define SLIB_ARCH_IS_BIG_ENDIAN
#	endif
#endif

#ifndef SLIB_ARCH_IS_64BIT
#	if defined(__LP64) || defined(__LP64__) || defined(_WIN64)
#		define SLIB_ARCH_IS_64BIT
#	endif
#endif
#ifndef SLIB_ARCH_IS_64BIT
#	define SLIB_ARCH_IS_32BIT
#endif

#if !defined(SLIB_ARCH_IS_BIG_ENDIAN) && !defined(SLIB_ARCH_IS_LITTLE_ENDIAN)
#	if defined(__BYTE_ORDER__)
#		if defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#			define SLIB_ARCH_IS_BIG_ENDIAN
#		elif defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#			define SLIB_ARCH_IS_LITTLE_ENDIAN
#		endif
#	endif
#endif
#if defined(SLIB_ARCH_IS_ARM) || defined(SLIB_ARCH_IS_ARM64)
#	ifndef SLIB_ARCH_IS_BIG_ENDIAN
#		define SLIB_ARCH_IS_LITTLE_ENDIAN
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

#if defined(SLIB_ARCH_IS_ARM64)
#	define SLIB_IF_ARCH_IS_ARM64(Y, N) Y
#else
#	define SLIB_IF_ARCH_IS_ARM64(Y, N) N
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
