#pragma once

#include "core/core.h"

#include "async_task.h"
#include "async_queue.h"

namespace hdn
{
	class IScheduler
	{
	public:
		// The Resolve function returns a queue object based on the topology of the underlying scheduler.
		// 1. The linear scheduler is trivial since all task will be able to be run in parallel
		// 2. The DAG scheduler is more complex and we need to add sync barrier within the queue
		virtual Ref<Queue> Resolve() = 0;
	};
}