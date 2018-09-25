#include <stdlib.h>

#if defined(__linux__) && !defined(__ANDROID__)

extern "C"
{
	void *_old_memcpy(void*, const void*, size_t);

#if defined(__x86_64__)
	asm(".symver _old_memcpy, memcpy@GLIBC_2.2.5");
#else
	asm(".symver _old_memcpy, memcpy@GLIBC_2.0");
#endif

	void* __wrap_memcpy(void* dst, const void* src, size_t size)
	{
  	return _old_memcpy(dst, src, size);
	}
}

#endif
