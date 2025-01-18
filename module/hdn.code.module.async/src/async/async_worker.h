#pragma once

#include "core/core.h"
#include "core/stl/vector.h"

#include "async_task.h"
#include "async_thread.h"

#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>

namespace hdn
{
	class WorkerSystem
	{
	public:
		WorkerSystem(u64 numWorkers);
		~WorkerSystem();
		void AddPendingTask(ITask* task);
		void Shutdown();
		static size_t OptimalWorkerCount(bool ioBound = false);
	private:
		void InsertTaskSorted(ITask* task);
		void WorkerThread();
	private:
		vector<std::thread> m_Workers;
		vector<ITask*> m_PendingTasks;
		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
		std::atomic<bool> m_StopFlag;
	};
}