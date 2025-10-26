#include "async_orchestrator.h"

namespace hdn
{
	AsyncOrchestrator& AsyncOrchestrator::Get()
	{
		static AsyncOrchestrator s_Instance;
		return s_Instance;
	}

	AsyncOrchestrator::AsyncOrchestrator()
		: m_WorkerSystem{ WorkerSystem::OptimalWorkerCount(true) }
	{
	}

	void AsyncOrchestrator::AddPendingTask(ITask* task)
	{
		m_WorkerSystem.AddPendingTask(task);
	}

	void AsyncOrchestrator::Shutdown()
	{
		m_WorkerSystem.Shutdown();
	}
}
