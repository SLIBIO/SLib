#include "../../../inc/slib/core/linked_list.h"
#include "../../../inc/slib/core/loop_queue.h"
#include "../../../inc/slib/core/queue.h"
#include "../../../inc/slib/core/queue_channel.h"
#include "../../../inc/slib/core/linked_object.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(CLinkedListBase, Object)

	CLinkedListBase::CLinkedListBase()
	{
	}

	CLinkedListBase::~CLinkedListBase()
	{
	}


	SLIB_DEFINE_OBJECT(LoopQueueBase, Object)

	LoopQueueBase::LoopQueueBase()
	{
	}

	LoopQueueBase::~LoopQueueBase()
	{
	}


	SLIB_DEFINE_OBJECT(LinkedObjectListBase, Object)

	LinkedObjectListBase::LinkedObjectListBase()
	{
	}

	LinkedObjectListBase::~LinkedObjectListBase()
	{
	}

}
