#pragma once

#include "core/core.h"
#include "async_task.h"

namespace hdn
{
	class Queue;

	class QueueGroup
	{
	public:
		QueueGroup(Queue* queue);
		void AddTask(Ref<ITask> task);
		bool Done();
		void NotifyTaskCompletion();
		const TVector<Ref<ITask>>& GetTasks() { return m_Tasks; }
	private:
		TVector<Ref<ITask>> m_Tasks{};
		Queue* m_Queue;
	};
}