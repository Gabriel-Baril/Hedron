#pragma once

#include "async_task.h"
#include "async_worker.h"

namespace hdn
{
	class AsyncOrchestrator
	{
	public:
		static AsyncOrchestrator& Get();
		void AddPendingTask(ITask* task);
		void Shutdown();
	private:
		AsyncOrchestrator();
	private:
		WorkerSystem m_WorkerSystem;
	};
}