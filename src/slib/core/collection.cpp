/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/core/array.h"

namespace slib
{
	
	SLIB_DEFINE_ROOT_OBJECT(CArrayBase)
	
	CArrayBase::CArrayBase() noexcept
	{
	}
	
	CArrayBase::~CArrayBase() noexcept
	{
	}
	
}


#include "slib/core/map.h"
#include "slib/core/hash_map.h"

namespace slib
{
	namespace priv
	{
		
		namespace map
		{
			const char g_classID[] = "map";
		}
		
		namespace hash_map
		{
			const char g_classID[] = "hash_map";
		}
		
	}
}


#include "slib/core/linked_list.h"
#include "slib/core/loop_queue.h"
#include "slib/core/queue.h"
#include "slib/core/queue_channel.h"
#include "slib/core/linked_object.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(CLinkedListBase, Object)

	CLinkedListBase::CLinkedListBase() noexcept
	{
	}

	CLinkedListBase::~CLinkedListBase() noexcept
	{
	}


	SLIB_DEFINE_OBJECT(LoopQueueBase, Object)

	LoopQueueBase::LoopQueueBase() noexcept
	{
	}

	LoopQueueBase::~LoopQueueBase() noexcept
	{
	}


	SLIB_DEFINE_OBJECT(LinkedObjectListBase, Object)

	LinkedObjectListBase::LinkedObjectListBase() noexcept
	{
	}

	LinkedObjectListBase::~LinkedObjectListBase() noexcept
	{
	}
	
}


#include "slib/core/ptr.h"

namespace slib
{
	namespace priv
	{
		namespace ptr
		{
			
			struct ConstStruct
			{
				void* _ptr;
				void* ref;
				sl_int32 lock;
			};
			
			const ConstStruct g_null = {0, 0, 0};
			
		}
	}
}


#include "slib/core/function.h"

namespace slib
{
	
	SLIB_DEFINE_ROOT_OBJECT(CallableBase)
	
	namespace priv
	{
		namespace function_list
		{
			const char g_classID[] = "FunctionList";
		}
	}
	
}


#include "slib/core/promise.h"

namespace slib
{
	namespace priv
	{
		namespace promise
		{
			const char g_classID[] = "promise";
		}
	}
}
