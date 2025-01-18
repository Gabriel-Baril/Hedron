#pragma once

#include "core/core.h"
#include "async_task.h"

#include <queue>

namespace hdn
{
	class ITaskQueue : public ITask
	{
	public:
		ITaskQueue();
		virtual ~ITaskQueue() = default;
		void AddTask(ITask* task);
		virtual bool Completed() const override;
		virtual void Execute() override;
		virtual void DependencyCompletionNotification(ITask* task) override;

		virtual const char* GetName() const override;
	private:
		std::deque<ITask*> m_TaskQueue;
	};
}