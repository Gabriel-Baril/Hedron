#include "async_task.h"

#include "async_orchestrator.h"

namespace hdn
{
	static std::mutex m_EnqueueFuncMutex;

	void ITask::PreExecute()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	void ITask::Complete()
	{
		m_EndTime = std::chrono::high_resolution_clock::now();
		// Notify the parent if there is one
		if (m_Parent)
		{
			m_Parent->DependencyCompletionNotification(this);
		}

		// Notify all the tasks that depend on this one, the let the dependencies enqueue themselve in the orchestrator
		for (ITask* task : m_OutDep)
		{
			HDN_TASK_ASSERT(task);
			task->DependencyCompletionNotification(this);
		}
	}

	bool ITask::Independent() const
	{
		return m_InDep.empty();
	}

	void ITask::Enqueue()
	{
		// HDN_CORE_ASSERT(!IsEnqueued(), "Cannot enqueue the same task two times!");
		if (IsEnqueued())
		{
			return;
		}

		std::lock_guard<std::mutex> lock(m_EnqueueFuncMutex);
		AsyncOrchestrator::Get().AddPendingTask(this);
		m_Enqueued = true;
	}

	bool ITask::IsEnqueued()
	{
		return m_Enqueued;
	}

	void ITask::AddInDependency(ITask* task)
	{
		m_InDep.insert(task);
	}

	void ITask::AddOutDependency(ITask* task)
	{
		m_OutDep.insert(task);
	}

	void ITask::AddInternalDependency(ITask* task)
	{
		m_InternalDep.insert(task);
	}

	void ITask::SetParent(ITask* parent)
	{
		m_Parent = parent;
	}

	bool ITask::AreInDepResolved() const
	{
		for (ITask* task : m_InDep)
		{
			HDN_TASK_ASSERT(task);
			if (!task->Completed())
			{
				return false;
			}
		}
		return true;
	}

	bool ITask::AreInternalDepResolved() const
	{
		for (ITask* task : m_InternalDep)
		{
			HDN_TASK_ASSERT(task);
			if (!task->Completed())
			{
				return false;
			}
		}
		return true;
	}

	const unordered_set<ITask*>& ITask::GetInternalDependencies() const
	{
		return m_InternalDep;
	}

	const unordered_set<ITask*>& ITask::GetInDependencies() const
	{
		return m_InDep;
	}

	const unordered_set<ITask*>& ITask::GetOutDependencies() const
	{
		return m_OutDep;
	}
}