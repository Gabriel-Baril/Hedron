#include "async_scheduler_linear.h"

namespace hdn
{
	void LinearScheduler::Add(Ref<ITask> task)
	{
		m_Tasks.push_back(task);
	}

	Ref<Queue> LinearScheduler::Resolve()
	{
		Ref<Queue> queue = CreateRef<Queue>();
		for (Ref<ITask> task : m_Tasks)
		{
			queue->AddTask(task);
		}
		return queue;
	}
}
