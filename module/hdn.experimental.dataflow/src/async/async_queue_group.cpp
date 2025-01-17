#include "async_queue_group.h"
#include "async_queue.h"

namespace hdn
{
	QueueGroup::QueueGroup(Queue* queue)
		: m_Queue{ queue }
	{
	}

	void QueueGroup::AddTask(Ref<ITask> task)
	{
		m_Tasks.push_back(task);
		task->SetGroup(this);
	}

	bool QueueGroup::Done()
	{
		for (Ref<ITask> task : m_Tasks)
		{
			if (!task->Completed())
			{
				return false;
			}
		}
		return true;
	}

	void QueueGroup::NotifyTaskCompletion()
	{
		if (Done())
		{
			m_Queue->Pop();
		}
	}

}
