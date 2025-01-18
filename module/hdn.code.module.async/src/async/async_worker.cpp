#include "async_worker.h"

#include <algorithm> // For std::lower_bound

namespace hdn
{
	struct SortTaskByImportance
	{
		bool operator()(ITask* lhs, ITask* rhs) const {
			return lhs->Importance() < rhs->Importance();
		}
	};

	WorkerSystem::WorkerSystem(u64 numWorkers)
		: m_StopFlag{ false }
	{
		HINFO("Worker Count: {0}", numWorkers);

		for (size_t i = 0; i < numWorkers; ++i)
		{
			m_Workers.emplace_back([this]() { this->WorkerThread(); });
		}
	}

	WorkerSystem::~WorkerSystem()
	{
		Shutdown();
	}

	void WorkerSystem::AddPendingTask(ITask* task)
	{
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			InsertTaskSorted(task);
			HDEBUG("Task Enqueued: {0}", task->GetName());
		}
		m_Condition.notify_one();
	}

	void WorkerSystem::Shutdown()
	{
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_StopFlag = true;
		}
		m_Condition.notify_all();

		for (std::thread& worker : m_Workers)
		{
			if (worker.joinable())
			{
				worker.join();
			}
		}
	}

	size_t WorkerSystem::OptimalWorkerCount(bool ioBound)
	{
		size_t hardwareThreads = std::thread::hardware_concurrency();
		if (hardwareThreads == 0)
		{
			hardwareThreads = 4;
		}
		return ioBound ? 2 * hardwareThreads : hardwareThreads;
	}

	void WorkerSystem::InsertTaskSorted(ITask* task)
	{
		auto pos = std::lower_bound(m_PendingTasks.begin(), m_PendingTasks.end(), task, SortTaskByImportance{});
		m_PendingTasks.insert(pos, task);
	}

	void WorkerSystem::WorkerThread()
	{
		while (true)
		{
			ITask* task = nullptr;
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);
				m_Condition.wait(lock, [this]() {
					return !m_PendingTasks.empty() || m_StopFlag;
					});

				if (m_StopFlag && m_PendingTasks.empty())
				{
					return;
				}

				task = std::move(m_PendingTasks.back());
				m_PendingTasks.pop_back();
			}
			task->PreExecute();
			task->Execute();
		}
	}


}