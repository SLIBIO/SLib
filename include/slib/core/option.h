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

/* Platform */
// Windows Platform
#define SLIB_PLATFORM_WIN32				0x01010000
#define SLIB_PLATFORM_WIN64				0x01010001
#define SLIB_PLATFORM_UWP				0x01020000	// Universal Windows Platform
// Unix Platform
#define SLIB_PLATFORM_LINUX				0x02010000	// RedHat, Fedora, CentOS, Debian, Ubuntu ...
#define SLIB_PLATFORM_ANDROID			0x02010A00
#define SLIB_PLATFORM_TIZEN				0x02010B00
#define SLIB_PLATFORM_MACOS				0x02020100
#define SLIB_PLATFORM_IOS				0x02020200
#define SLIB_PLATFORM_IOS_SIMULATOR		0x02020201
#define SLIB_PLATFORM_FREEBSD			0x02030000
#define SLIB_PLATFORM_SOLARIS			0x02040000

/* Graphics */
#define SLIB_GRAPHICS_GDI		0x0100
#define SLIB_GRAPHICS_QUARTZ	0x0200
#define SLIB_GRAPHICS_CAIRO		0x0300
#define SLIB_GRAPHICS_ANDROID	0x0400

/* UI Framework */
#define SLIB_UI_WIN32			0x0100
#define SLIB_UI_MACOS			0x0200 // Cocoa
#define SLIB_UI_IOS				0x0201 // Cocoa Touch
#define SLIB_UI_ANDROID			0x0300
#define SLIB_UI_GTK				0x0400
#define SLIB_UI_EFL				0x0500

/* Compiler */
#define SLIB_COMPILER_VC		0x0100
#define SLIB_COMPILER_GCC		0x0200
#define SLIB_COMPILER_OBJC		0x0300

/*************************************
 		Platform Definition
**************************************/
#ifndef SLIB_PLATFORM
#	if defined(WINAPI_FAMILY)
#		define SLIB_PLATFORM			SLIB_PLATFORM_UWP
#	elif defined(_WIN64)
#		define SLIB_PLATFORM			SLIB_PLATFORM_WIN64
#	elif defined(_WIN32)
#		define SLIB_PLATFORM			SLIB_PLATFORM_WIN32
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
#	elif defined(__linux__)
#		define SLIB_PLATFORM			SLIB_PLATFORM_LINUX
#	elif defined(__unix__) && defined(BSD)
#		define SLIB_PLATFORM			SLIB_PLATFORM_FREEBSD
#	elif defined(__unix__) && defined(__sun) && defined(__SVR4)
#		define SLIB_PLATFORM			SLIB_PLATFORM_SOLARIS
#	endif
#endif

#if (SLIB_PLATFORM == SLIB_PLATFORM_WIN32)
#	define SLIB_PLATFORM_IS_WIN32
#	define SLIB_PLATFORM_IS_WINDOWS
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_WIN64)
#	define SLIB_PLATFORM_IS_WIN32
#	define SLIB_PLATFORM_IS_WIN64
#	define SLIB_PLATFORM_IS_WINDOWS
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_MACOS)
#	define SLIB_PLATFORM_IS_MACOS
#	define SLIB_PLATFORM_IS_APPLE
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_IOS)
#	define SLIB_PLATFORM_IS_IOS
#	define SLIB_PLATFORM_IS_IOS_DEVICE
#	define SLIB_PLATFORM_IS_APPLE
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_MOBILE
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_IOS_SIMULATOR)
#	define SLIB_PLATFORM_IS_IOS
#	define SLIB_PLATFORM_IS_IOS_SIMULATOR
#	define SLIB_PLATFORM_IS_APPLE
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_MOBILE
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_ANDROID)
#	define SLIB_PLATFORM_IS_ANDROID
#	define SLIB_PLATFORM_IS_LINUX
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_MOBILE
#	define SLIB_PLATFORM_USE_JNI
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_TIZEN)
#	define SLIB_PLATFORM_IS_TIZEN
#	define SLIB_PLATFORM_IS_LINUX
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_MOBILE
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_UWP)
#	define SLIB_PLATFORM_IS_UWP
#	define SLIB_PLATFORM_IS_WINDOWS
#	define SLIB_PLATFORM_IS_MOBILE
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_LINUX)
#	define SLIB_PLATFORM_IS_LINUX
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_FREEBSD)
#	define SLIB_PLATFORM_IS_FREEBSD
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif
#if (SLIB_PLATFORM == SLIB_PLATFORM_SOLARIS)
#	define SLIB_PLATFORM_IS_SOLARIS
#	define SLIB_PLATFORM_IS_UNIX
#	define SLIB_PLATFORM_IS_DESKTOP
#endif

#ifdef SLIB_PLATFORM_IS_MACOS
#	define SLIB_IF_PLATFORM_IS_MACOS(Y,N) (Y)
#else
#	define SLIB_IF_PLATFORM_IS_MACOS(Y,N) (N)
#endif

/*************************************
 		Graphics Definition
**************************************/
#ifndef SLIB_GRAPHICS
#	if defined(SLIB_PLATFORM_IS_WIN32)
#		define SLIB_GRAPHICS		SLIB_GRAPHICS_GDI
#	elif defined(SLIB_PLATFORM_IS_APPLE)
#		define SLIB_GRAPHICS		SLIB_GRAPHICS_QUARTZ
#	elif defined(SLIB_PLATFORM_IS_ANDROID)
#		define SLIB_GRAPHICS		SLIB_GRAPHICS_ANDROID
#	elif defined(SLIB_PLATFORM_IS_LINUX)
#		define SLIB_GRAPHICS		SLIB_GRAPHICS_CAIRO
#	endif
#endif

#if (SLIB_GRAPHICS == SLIB_GRAPHICS_GDI)
#	define SLIB_GRAPHICS_IS_GDI
#endif
#if (SLIB_GRAPHICS == SLIB_GRAPHICS_QUARTZ)
#	define SLIB_GRAPHICS_IS_QUARTZ
#endif
#if (SLIB_GRAPHICS == SLIB_GRAPHICS_ANDROID)
#	define SLIB_GRAPHICS_IS_ANDROID
#endif
#if (SLIB_GRAPHICS == SLIB_GRAPHICS_CAIRO)
#	define SLIB_GRAPHICS_IS_CAIRO
#endif

/*************************************
 			UI Definition
**************************************/
#ifndef SLIB_UI
#	if defined(SLIB_PLATFORM_IS_WIN32)
#		define SLIB_UI			SLIB_UI_WIN32
#	elif defined(SLIB_PLATFORM_IS_MACOS)
#		define SLIB_UI			SLIB_UI_MACOS
#	elif defined(SLIB_PLATFORM_IS_IOS)
#		define SLIB_UI			SLIB_UI_IOS
#	elif defined(SLIB_PLATFORM_IS_ANDROID)
#		define SLIB_UI			SLIB_UI_ANDROID
#	elif defined(SLIB_PLATFORM_IS_TIZEN)
#		define SLIB_UI			SLIB_UI_EFL
#	elif defined(SLIB_PLATFORM_IS_LINUX)
#		define SLIB_UI			SLIB_UI_GTK
#	endif
#endif

#if (SLIB_UI == SLIB_UI_WIN32)
#	define SLIB_UI_IS_WIN32
#endif
#if (SLIB_UI == SLIB_UI_MACOS)
#	define SLIB_UI_IS_MACOS
#endif
#if (SLIB_UI == SLIB_UI_IOS)
#	define SLIB_UI_IS_IOS
#endif
#if (SLIB_UI == SLIB_UI_ANDROID)
#	define SLIB_UI_IS_ANDROID
#endif
#if (SLIB_UI == SLIB_UI_GTK)
#	define SLIB_UI_IS_GTK
#endif
#if (SLIB_UI == SLIB_UI_EFL)
#	define SLIB_UI_IS_EFL
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

#if (SLIB_COMPILER == SLIB_COMPILER_VC)
#	define SLIB_COMPILER_IS_VC
#endif
#if (SLIB_COMPILER == SLIB_COMPILER_GCC)
#	define SLIB_COMPILER_IS_GCC
#endif
#if (SLIB_COMPILER == SLIB_COMPILER_OBJC)
#	define SLIB_COMPILER_IS_GCC
#	define SLIB_COMPILER_IS_OBJC
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

#endif
