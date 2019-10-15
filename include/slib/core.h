/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_CORE_HEADER
#define CHECKHEADER_SLIB_CORE_HEADER

#ifndef SLIB_NOT_SUPPORT_STD_TYPES
#	ifndef SLIB_SUPPORT_STD_TYPES
#		define SLIB_SUPPORT_STD_TYPES
#	endif
#endif

#include "core/definition.h"
#include "core/base.h"
#include "core/endian.h"
#include "core/mio.h"
#include "core/asm.h"

#include "core/cpp.h"
#include "core/atomic.h"
#include "core/tuple.h"
#include "core/ref.h"
#include "core/object.h"
#include "core/ptr.h"
#include "core/function.h"
#include "core/promise.h"
#include "core/new_helper.h"

#include "core/macro.h"
#include "core/scoped.h"
#include "core/safe_static.h"
#include "core/singleton.h"

#include "core/spin_lock.h"
#include "core/mutex.h"
#include "core/string.h"
#include "core/string_buffer.h"
#include "core/string_param.h"
#include "core/memory.h"
#include "core/time.h"
#include "core/variant.h"

#include "core/cast.h"
#include "core/nullable.h"
#include "core/null_value.h"
#include "core/compare.h"
#include "core/hash.h"
#include "core/search.h"
#include "core/sort.h"

#include "core/array.h"
#include "core/list.h"
#include "core/map.h"
#include "core/hash_map.h"
#include "core/hash_table.h"
#include "core/linked_list.h"
#include "core/queue.h"
#include "core/queue_channel.h"
#include "core/linked_object.h"
#include "core/loop_queue.h"
#include "core/expire.h"
#include "core/btree.h"

#include "core/math.h"
#include "core/interpolation.h"
#include "core/animation.h"

#include "core/system.h"
#include "core/console.h"
#include "core/event.h"
#include "core/process.h"
#include "core/thread.h"
#include "core/thread_pool.h"
#include "core/rw_lock.h"
#include "core/log.h"
#include "core/asset.h"

#include "core/io.h"
#include "core/file.h"
#include "core/pipe.h"
#include "core/async.h"
#include "core/dispatch.h"
#include "core/dispatch_loop.h"
#include "core/timer.h"
#include "core/global_unique_instance.h"

#include "core/app.h"
#include "core/service.h"
#include "core/content_type.h"
#include "core/locale.h"
#include "core/charset.h"
#include "core/parse.h"
#include "core/resource.h"
#include "core/preference.h"
#include "core/setting.h"

#include "core/regex.h"
#include "core/json.h"
#include "core/xml.h"

#endif

