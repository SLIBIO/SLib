/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
