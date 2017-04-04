#ifndef CONFIG_H
#define CONFIG_H

/* Compile as fixed-point (for machines without a fast enough FPU) */
#define FIXED_POINT 1

/* This is a build of OPUS */
#define OPUS_BUILD 1

// Windows
#if (defined(WINAPI_FAMILY) || defined(_WIN64) || defined(_WIN32))

#	if (defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP))
		/* Use C99 variable-size arrays */
#		define VAR_ARRAYS 1
#	else
		/* Make use of alloca */
#		define USE_ALLOCA 1
#	endif

#	if (defined(_M_IX86) || defined(_M_X64))

		/* Compiler supports X86 SSE Intrinsics */
#		define OPUS_X86_MAY_HAVE_SSE

		/* Compiler supports X86 SSE2 Intrinsics */
#		define OPUS_X86_MAY_HAVE_SSE2

		/* Compiler supports X86 SSE4.1 Intrinsics */
#		define OPUS_X86_MAY_HAVE_SSE4_1

		/* Presume SSE functions, if compiled to use SSE/SSE2/AVX (note that AMD64 implies SSE2, and AVX implies SSE4.1) */
#		if (defined(_M_X64) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1)) || defined(__AVX__))
			/* Define if binary requires SSE intrinsics support */
#			define OPUS_X86_PRESUME_SSE 1
#		endif
#		if (defined(_M_X64) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 2)) || defined(__AVX__))
			/* Define if binary requires SSE2 intrinsics support */
#			define OPUS_X86_PRESUME_SSE2 1
#		endif
#		if (defined(__AVX__))
			/* Define if binary requires SSE4.1 intrinsics support */
#			define OPUS_X86_PRESUME_SSE4_1 1
#		endif
#		if (!defined(OPUS_X86_PRESUME_SSE4_1) || !defined(OPUS_X86_PRESUME_SSE2) || !defined(OPUS_X86_PRESUME_SSE))
			/* Use run-time CPU capabilities detection */
#			define OPUS_HAVE_RTCD 1
#		endif
#	endif

// OSX, iOS
#elif (defined(__APPLE__))

	/* Use C99 variable-size arrays */
#	define VAR_ARRAYS 1

	/* Define to 1 if you have the `lrint' function. */
#	define HAVE_LRINT 1

	/* Define to 1 if you have the `lrintf' function. */
#	define HAVE_LRINTF 1

#	if (defined(__i386__) || defined(__x86_64__))

#	elif (defined(__arm64__))

		/* Make use of ARM asm optimization */
#		define OPUS_ARM_ASM 1

#	elif (defined(__arm__))

		/* Make use of ARM asm optimization */
#		define OPUS_ARM_ASM 1

		/* Use generic ARMv4 inline asm optimizations */
#		define OPUS_ARM_INLINE_ASM 1

		/* Use ARMv5E inline asm optimizations */
#		define OPUS_ARM_INLINE_EDSP 1

		/* Use ARMv6 inline asm optimizations */
#		define OPUS_ARM_INLINE_MEDIA 1

		/* Use ARM NEON inline asm optimizations */
#		define OPUS_ARM_INLINE_NEON 1

#		define OPUS_ARM_MAY_HAVE_EDSP 1
//#		define OPUS_ARM_PRESUME_EDSP 1
#		define OPUS_ARM_MAY_HAVE_MEDIA 1
//#		define OPUS_ARM_PRESUME_MEDIA 1

		/* Define if compiler supports NEON instructions */
#		if (defined(__ARM_NEON__) || defined(__ARM_NEON))
#			define OPUS_ARM_MAY_HAVE_NEON 1
#			define OPUS_ARM_MAY_HAVE_NEON_INTR 1
//#			define OPUS_ARM_PRESUME_NEON 1
#			define OPUS_ARM_PRESUME_NEON_INTR 1
#		endif

#	endif

// Android
#elif (defined(__ANDROID__))

/* Use C99 variable-size arrays */
#	define VAR_ARRAYS 1

/* Define to 1 if you have the `lrint' function. */
#	define HAVE_LRINT 1

/* Define to 1 if you have the `lrintf' function. */
#	define HAVE_LRINTF 1

#	if (defined(__i386__) || defined(__x86_64__))

#	elif (defined(__aarch64__))

		/* Make use of ARM asm optimization */
#		define OPUS_ARM_ASM 1

#	elif (defined(__arm__))

		/* Make use of ARM asm optimization */
#		define OPUS_ARM_ASM 1

		/* Use generic ARMv4 inline asm optimizations */
#		define OPUS_ARM_INLINE_ASM 1

		/* Use ARMv5E inline asm optimizations */
#		define OPUS_ARM_INLINE_EDSP 1

		/* Use ARMv6 inline asm optimizations */
#		define OPUS_ARM_INLINE_MEDIA 1

		/* Use ARM NEON inline asm optimizations */
#		define OPUS_ARM_INLINE_NEON 1

#		define OPUS_ARM_MAY_HAVE_EDSP 1
//#		define OPUS_ARM_PRESUME_EDSP 1
#		define OPUS_ARM_MAY_HAVE_MEDIA 1
//#		define OPUS_ARM_PRESUME_MEDIA 1

#		define OPUS_ARM_MAY_HAVE_NEON 1
#		define OPUS_ARM_MAY_HAVE_NEON_INTR 1
//#		define OPUS_ARM_PRESUME_NEON 1
#		define OPUS_ARM_PRESUME_NEON_INTR 1

#	endif

#endif

#endif /* CONFIG_H */