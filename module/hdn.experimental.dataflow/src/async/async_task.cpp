#include "async_task.h"

#include "async_queue.h"
#include "async_queue_group.h"

namespace hdn
{
	bool ITask::Completed()
	{
		return m_Done;
	}

	void ITask::Complete()
	{
		m_Done = true;
		if (m_Group)
		{
			m_Group->NotifyTaskCompletion();
		}
	}

	void ITask::SetGroup(QueueGroup* group)
	{
		m_Group = group;
	}
}