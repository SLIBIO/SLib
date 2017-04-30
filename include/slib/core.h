/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_HEADER
#define CHECKHEADER_SLIB_CORE_HEADER

#ifndef SLIB_NOT_SUPPORT_STD_TYPES
#	ifndef SLIB_SUPPORT_STD_TYPES
#		define SLIB_SUPPORT_STD_TYPES
#	endif
#endif

#include "core/definition.h"
#include "core/constants.h"
#include "core/base.h"
#include "core/endian.h"
#include "core/mio.h"

#include "core/cpp.h"
#include "core/atomic.h"
#include "core/tuple.h"
#include "core/ref.h"
#include "core/object.h"
#include "core/ptr.h"
#include "core/function.h"
#include "core/new_helper.h"

#include "core/macro.h"
#include "core/scoped.h"
#include "core/safe_static.h"
#include "core/singleton.h"

#include "core/spin_lock.h"
#include "core/mutex.h"
#include "core/string.h"
#include "core/string_buffer.h"
#include "core/memory.h"
#include "core/time.h"
#include "core/variant.h"

#include "core/cast.h"
#include "core/compare.h"
#include "core/hash.h"
#include "core/search.h"
#include "core/sort.h"

#include "core/hashtable.h"
#include "core/tree.h"
#include "core/array.h"
#include "core/array2d.h"
#include "core/iterator.h"
#include "core/list.h"
#include "core/map.h"
#include "core/linked_list.h"
#include "core/queue.h"
#include "core/queue_channel.h"
#include "core/linked_object.h"
#include "core/loop_queue.h"
#include "core/expire.h"

#include "core/math.h"
#include "core/interpolation.h"
#include "core/animation.h"

#include "core/system.h"
#include "core/event.h"
#include "core/thread.h"
#include "core/thread_pool.h"
#include "core/log.h"
#include "core/asset.h"

#include "core/io.h"
#include "core/file.h"
#include "core/pipe.h"
#include "core/async.h"
#include "core/dispatch.h"
#include "core/dispatch_loop.h"
#include "core/timer.h"

#include "core/app.h"
#include "core/service.h"
#include "core/content_type.h"
#include "core/locale.h"
#include "core/charset.h"
#include "core/parse.h"
#include "core/resource.h"
#include "core/preference.h"
#include "core/setting.h"

#include "core/json.h"
#include "core/xml.h"
#include "core/base64.h"

#endif

