#ifndef CHECKHEADER_SLIB_CORE_LOOP_QUEUE
#define CHECKHEADER_SLIB_CORE_LOOP_QUEUE

#include "definition.h"

#include "object.h"
#include "new_helper.h"

namespace slib
{
	
	class SLIB_EXPORT LoopQueueBase : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		LoopQueueBase();

		~LoopQueueBase();

	};
	
	template <class T>
	class SLIB_EXPORT LoopQueue : public LoopQueueBase
	{
	protected:
		T* m_data;
		sl_size m_size;
		sl_size m_first;
		sl_size m_count;
		sl_size m_latency;
	
	public:
		LoopQueue(sl_size size = 10, sl_size latency = 0);

		~LoopQueue();
	
	public:
		sl_size getQueueSize();

		sl_bool setQueueSize(sl_size size);

		void removeAll();

		T* getBuffer();

		sl_size getCount();

		void setLatency(sl_size latency);

		sl_size getLatency();

		sl_bool add(const T& data, sl_bool flagPush = sl_true);

		sl_bool add(const T* buffer, sl_size count, sl_bool flagPush = sl_true);

		sl_bool get(T& output);

		sl_bool get(T* buffer, sl_size count);

		sl_size copy(T* buffer, sl_size count);

	};

}

#include "detail/loop_queue.h"

#endif
