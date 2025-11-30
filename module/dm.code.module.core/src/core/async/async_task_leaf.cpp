#include "async_task_leaf.h"

namespace hdn
{
	ITaskLeaf::ITaskLeaf()
	{
	}

	bool ITaskLeaf::Completed() const
	{
		return m_Completed;
	}

	void ITaskLeaf::Complete()
	{
		m_Completed = true;
		ITask::Complete();
	}

	void ITaskLeaf::DependencyCompletionNotification(ITask* task)
	{
		HDN_MAYBE_UNUSED(task);
		if (!IsEnqueued() && AreInDepResolved())
		{
			Enqueue();
		}
	}

	const char* ITaskLeaf::GetName() const
	{
		return "ITaskLeaf";
	}
}
