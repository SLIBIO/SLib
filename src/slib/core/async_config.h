#ifndef CHECKHEADER_SLIB_CORE_ASYNC_CONFIG
#define CHECKHEADER_SLIB_CORE_ASYNC_CONFIG

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)
#define ASYNC_USE_IOCP
#elif defined(SLIB_PLATFORM_IS_APPLE)
#define ASYNC_USE_KQUEUE
#elif defined(SLIB_PLATFORM_IS_LINUX)
#define ASYNC_USE_EPOLL
#elif defined(SLIB_PLATFORM_IS_FREEBSD)
#define ASYNC_USE_KEVENT
#endif

#define ASYNC_MAX_WAIT_EVENT 256

#endif
