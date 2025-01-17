#include "async_queue.h"

#include "async_queue_group.h"

namespace hdn
{
	Queue::Queue()
	{
		Ref<QueueGroup> group = CreateRef<QueueGroup>(this);
		m_QueueGroups.push(group);
	}

	void Queue::AddTask(Ref<ITask> task)
	{
		m_QueueGroups.front()->AddTask(task);
	}

	void Queue::AddBarrier()
	{
		Ref<QueueGroup> group = CreateRef<QueueGroup>(this);
		m_QueueGroups.push(group);
	}

	bool Queue::Done()
	{
		return m_QueueGroups.empty();
	}

	void Queue::Pop()
	{
		m_QueueGroups.pop();
	}
}
