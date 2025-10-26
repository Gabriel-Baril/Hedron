#include "async_task_queue.h"

namespace hdn
{
	ITaskQueue::ITaskQueue()
	{
	}

	void ITaskQueue::AddTask(ITask* task)
	{
		AddInternalDependency(task);
		task->SetParent(this);
		if (!m_TaskQueue.empty())
		{
			ITask* previousTask = m_TaskQueue.back();
			previousTask->AddOutDependency(task);
			task->AddInDependency(previousTask);
		}
		m_TaskQueue.push_back(task);
	}

	bool ITaskQueue::Completed() const
	{
		return AreInternalDepResolved();
	}

	void ITaskQueue::Execute()
	{
		ITask* firstTask = m_TaskQueue.front();
		// We only need to queue the first task, the subsequent tasks will automatically register themselve once their in-dependencies are Completed()
		firstTask->Enqueue();
	}

	void ITaskQueue::DependencyCompletionNotification(ITask* task)
	{
		HASSERT_TASK(task);
		HASSERT(task->Completed(), "The task notified was not completed");
		if (Completed())
		{
			ITask::Complete();
		}

		if (!IsEnqueued() && AreInDepResolved())
		{
			Enqueue();
		}
	}

	const char* ITaskQueue::GetName() const
	{
		return "ITaskQueue";
	}
}
