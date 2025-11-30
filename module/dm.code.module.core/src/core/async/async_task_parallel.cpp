#include "async_task_parallel.h"

#include "async_orchestrator.h"

namespace hdn
{
	ITaskParallel::ITaskParallel()
	{
	}

	void ITaskParallel::AddTask(ITask* task)
	{
		AddInternalDependency(task);
		task->SetParent(this);
		m_Tasks.insert(task);
	}

	bool ITaskParallel::Completed() const
	{
		return AreInternalDepResolved();
	}

	void ITaskParallel::Execute()
	{
		for (const auto& task : m_Tasks)
		{
			task->Enqueue();
		}
	}

	void ITaskParallel::DependencyCompletionNotification(ITask* task)
	{
		HDN_TASK_ASSERT(task);
		HDN_CORE_ASSERT(task->Completed(), "The task notified was not completed");
		if (Completed())
		{
			ITask::Complete();
		}

		if (!IsEnqueued() && AreInDepResolved())
		{
			Enqueue();
		}
	}

	const char* ITaskParallel::GetName() const
	{
		return "ITaskParallel";
	}
}
