#pragma once

#include "core/core.h"
#include "core/stl/unordered_set.h"

#include "async_task.h"

namespace hdn
{
	class ITaskParallel : public ITask
	{
	public:
		ITaskParallel();
		virtual ~ITaskParallel() = default;
		void AddTask(ITask* task);
		virtual bool Completed() const override;
		virtual void Execute() override;
		virtual void DependencyCompletionNotification(ITask* task) override;

		virtual const char* GetName() const override;
	private:
		unordered_set<ITask*> m_Tasks;
	};
}