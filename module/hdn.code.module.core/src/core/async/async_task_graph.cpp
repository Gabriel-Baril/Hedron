#include "async_task_graph.h"

namespace hdn
{
	ITaskGraph::ITaskGraph()
	{
	}

	void ITaskGraph::AddEdge(ITask* from, ITask* to)
	{
		AddInternalDependency(from);
		AddInternalDependency(to);
		from->SetParent(this);
		to->SetParent(this);

		from->AddOutDependency(to);
		to->AddInDependency(from);
	}

	bool ITaskGraph::HasCycle() const
	{
		unordered_set<ITask*> visited;
		unordered_set<ITask*> recStack;

		auto dfs = [&](ITask* task, auto& dfs_ref) -> bool {
			if (recStack.count(task)) return true;
			if (visited.count(task)) return false;

			visited.insert(task);
			recStack.insert(task);

			for (auto& dependent : task->GetOutDependencies()) {
				if (dfs_ref(dependent, dfs_ref)) {
					return true;
				}
			}
			recStack.erase(task);
			return false;
		};

		for (const auto& task : GetInternalDependencies()) {
			if (dfs(task, dfs)) {
				return true;
			}
		}
		return false;
	}

	bool ITaskGraph::Completed() const
	{
		return AreInternalDepResolved();
	}

	void ITaskGraph::PreExecute()
	{
		HASSERT(!HasCycle(), "The provided task graph has a circular dependency!");
		ITask::PreExecute();

		// Find the source nodes in set of internal dependencies
		for (const auto& task : GetInternalDependencies())
		{
			if (task->Independent())
			{
				m_SourceTasks.insert(task);
			}
		}
	}

	void ITaskGraph::Execute()
	{
		for (const auto& task : m_SourceTasks)
		{
			task->Enqueue();
		}
	}

	void ITaskGraph::DependencyCompletionNotification(ITask* task)
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

	const char* ITaskGraph::GetName() const
	{
		return "ITaskGraph";
	}
}
