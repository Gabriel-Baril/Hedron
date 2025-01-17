#pragma once

#include "core/core.h"
#include "async_task.h"

#include <queue>

namespace hdn
{
	class QueueGroup;

	class Queue
	{
	public:
		Queue();
		void AddTask(Ref<ITask> task);
		void AddBarrier();
		bool Done();
		void Pop();
	private:
		std::queue<Ref<QueueGroup>> m_QueueGroups;
	};
}