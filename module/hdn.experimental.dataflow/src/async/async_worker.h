#pragma once

#include "core/core.h"
#include "async_task.h"
#include "async_thread.h"

#include <thread>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <vector>

namespace hdn
{
	class WorkerThread : public IThread
	{
	public:
		WorkerThread()
		{
		}

		void SetTaskQueue(std::queue<ITask*>* taskQueue) { m_TaskQueue = taskQueue; }
		void SetQueueMutex(std::mutex* queueMutex) { m_QueueMutex = queueMutex; }
		void SetCondition(std::condition_variable* condition) { m_Condition = condition; }
		void SetStopFlag(std::atomic<bool>* stopFlag) { m_StopFlag = stopFlag; }

	protected:
		virtual void Run() override
		{
			while (true)
			{
				ITask* task = nullptr;
				{
					std::unique_lock<std::mutex> lock(*m_QueueMutex);
					m_Condition->wait(lock, [this]() {
						return !m_TaskQueue->empty() || *m_StopFlag;
						});

					if (*m_StopFlag && m_TaskQueue->empty())
					{
						return;
					}

					task = std::move(m_TaskQueue->front());
					m_TaskQueue->pop();
				}
				task->Run();
				task->Complete();
			}
		}
	private:
		std::queue<ITask*>* m_TaskQueue = nullptr;
		std::mutex* m_QueueMutex = nullptr;
		std::condition_variable* m_Condition = nullptr;
		std::atomic<bool>* m_StopFlag = nullptr;
	};

	class WorkerSystem
	{
	public:
		WorkerSystem(u64 numWorkers)
			: m_StopFlag{ false }
		{
			for (u64 i = 0; i < numWorkers; i++)
			{
				WorkerThread thread;
				thread.SetTaskQueue(&m_TaskQueue);
				thread.SetQueueMutex(&m_QueueMutex);
				thread.SetCondition(&m_Condition);
				thread.SetStopFlag(&m_StopFlag);
				thread.Start();
				m_Workers.emplace_back(thread);
			}
		}

		~WorkerSystem()
		{
			Stop();
		}

		void EnqueueTask(ITask* task)
		{
			{
				std::lock_guard<std::mutex> lock(m_QueueMutex);
				m_TaskQueue.push(std::move(task));
			}
			m_Condition.notify_one();
		}

		void EnqueueGroup(QueueGroup* group)
		{
			{
				std::lock_guard<std::mutex> lock(m_QueueMutex);
				for (ITask* task : group->GetTasks())
				{
					m_TaskQueue.push(std::move(task));
				}
			}
			m_Condition.notify_one();
		}

		void Stop()
		{
			{
				std::lock_guard<std::mutex> lock(m_QueueMutex);
				m_StopFlag = true;
			}
			m_Condition.notify_all();

			for (WorkerThread& worker : m_Workers)
			{
				if (worker.Joinable())
				{
					worker.Join();
				}
			}
		}

		static size_t OptimalWorkerCount(bool ioBound = false)
		{
			size_t hardwareThreads = std::thread::hardware_concurrency();
			if (hardwareThreads == 0)
			{
				hardwareThreads = 4;
			}
			return ioBound ? 2 * hardwareThreads : hardwareThreads;
		}

	private:
		TVector<WorkerThread> m_Workers;
		std::queue<ITask*> m_TaskQueue;
		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
		std::atomic<bool> m_StopFlag;
	};
}