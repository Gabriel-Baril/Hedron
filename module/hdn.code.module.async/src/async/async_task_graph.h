#pragma once

#include "core/core.h"
#include "async_task.h"

#include <queue>

namespace hdn
{
	class ITaskGraph : public ITask
	{
	public:
		ITaskGraph();
		virtual ~ITaskGraph() = default;
		
		void AddEdge(ITask* from, ITask* to);
		bool HasCycle() const;

		virtual bool Completed() const override;
		virtual void PreExecute() override;
		virtual void Execute() override;
		virtual void DependencyCompletionNotification(ITask* task) override;

		virtual const char* GetName() const override;
	private:
		unordered_set<ITask*> m_SourceTasks; // Tasks with no in dependencies after graph construction
	};
}