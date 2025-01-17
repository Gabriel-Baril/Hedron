#pragma once

#include "async_queue.h"
#include "async_worker.h"
#include "async_thread.h"

namespace hdn
{
	class ExecuteQueueThread : public IThread
	{
	public:
		void AddQueue(Ref<Queue> queue)
		{

		}

		void Run() override
		{
			while (true)
			{
			}
		}
	private:
		TVector<Ref<Queue>> m_Queues;
	};

	class AsyncOrchestrator
	{
	public:
		static AsyncOrchestrator& Get();
	private:
		AsyncOrchestrator()
			: m_WorkerSystem{ WorkerSystem::OptimalWorkerCount() }
		{
		}

		void ExecuteQueue(Queue& queue)
		{
			QueueGroup& group = queue.GetCurrentGroup();
			for (ITask* task : group)
			{
				m_WorkerSystem.EnqueueTask(task);
			}

			while (!queue.Done())
			{

			}

		}
	private:
		WorkerSystem m_WorkerSystem;
	};
}