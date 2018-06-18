#ifndef CONFIG_H
#define CONFIG_H

#define PACKAGE_VERSION "1.2.1"
#define OPUS_BUILD 1

/* Compile as fixed-point (for machines without a fast enough FPU) */
#define FIXED_POINT 1

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
/* Use C99 variable-size arrays */
#	define VAR_ARRAYS 1
#elif defined(_WIN64) || defined(_WIN32)
#   define USE_ALLOCA 1
#else
#   define VAR_ARRAYS 1
#	define HAVE_LRINT 1
#	define HAVE_LRINTF 1
#   define HAVE_STDINT_H 1
#   define restrict __restrict
#endif

#if defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(i386) || defined(__i386) || defined(__i386__) || defined(_X86_) || defined(_M_IX86) || defined(EMSCRIPTEN)
/* Can always compile SSE intrinsics (no special compiler flags necessary) */
#   define OPUS_X86_MAY_HAVE_AVX
#   define OPUS_X86_MAY_HAVE_SSE
#   define OPUS_X86_MAY_HAVE_SSE2
#   define OPUS_X86_MAY_HAVE_SSE4_1
/* Presume SSE functions, if compiled to use SSE/SSE2/AVX (note that AMD64 implies SSE2, and AVX
   implies SSE4.1) */
#   if defined(__SSE__) || defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1)) || defined(__AVX__)
#       define OPUS_X86_PRESUME_SSE 1
#   endif
#   if defined(__SSE2__) || defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && (_M_IX86_FP >= 2)) || defined(__AVX__)
#       define OPUS_X86_PRESUME_SSE2 1
#   endif
#   if defined(__SSE4_1__) || defined(__AVX__)
#       define OPUS_X86_PRESUME_SSE4_1 1
#   endif
#   if !defined(OPUS_X86_PRESUME_SSE4_1) || !defined(OPUS_X86_PRESUME_SSE2) || !defined(OPUS_X86_PRESUME_SSE)
#       define OPUS_HAVE_RTCD 1
#   endif
#elif defined(__arm64__) || defined(__arm64) || defined(__aarch64__)
#   define OPUS_ARM_ASM 1
#   define OPUS_ARM_MAY_HAVE_NEON_INTR 1
#   define OPUS_ARM_PRESUME_AARCH64_NEON_INTR 1
#   define OPUS_ARM_PRESUME_NEON_INTR 1
#elif defined(__arm__) || defined(__arm) || defined(ARM) || defined(_ARM_) || defined(__ARM__) || defined(_M_ARM)
#   define OPUS_ARM_ASM 1
#   define OPUS_ARM_INLINE_ASM 1
#   define OPUS_ARM_INLINE_EDSP 1
#   define OPUS_ARM_INLINE_MEDIA 1
#   define OPUS_ARM_MAY_HAVE_EDSP 1
#   define OPUS_ARM_MAY_HAVE_MEDIA 1
#   define OPUS_ARM_PRESUME_EDSP 1
#   define OPUS_ARM_PRESUME_MEDIA 1
#	if (defined(__ARM_NEON__) || defined(__ARM_NEON))
#		define OPUS_ARM_INLINE_NEON 1
#       define OPUS_ARM_MAY_HAVE_NEON 1
#       define OPUS_ARM_MAY_HAVE_NEON_INTR 1
#       define OPUS_ARM_PRESUME_NEON 1
#       define OPUS_ARM_PRESUME_NEON_INTR 1
#   endif
#endif

#endif /* CONFIG_H */
