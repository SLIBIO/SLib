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
