/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_ASYNC_CONFIG
#define CHECKHEADER_SLIB_CORE_ASYNC_CONFIG

#include "slib/core/definition.h"

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
