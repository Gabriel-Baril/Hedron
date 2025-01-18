#pragma once

#include "core/core.h"
#include "core/stl/unordered_set.h"

#include <chrono>
#include <set>

#define HASSERT_TASK(task) HASSERT(task, "Task cannot be null!")

namespace hdn
{
	class ITask
	{
	public:

		virtual ~ITask() = default;

		virtual void PreExecute();
		virtual void Execute() = 0;
		virtual void Complete();
		virtual bool Completed() const = 0;

		// An independent task is a task without in dependencies
		bool Independent() const;

		// Both Priority and Complexity are additive property, P(n) + P(m) = P(n+m)
		virtual u32 Priority() const { return 1; }

		// Complexity cannot be 0
		virtual u32 Complexity() const { return 1; }
		float Importance() const { return static_cast<float>(Priority()) / static_cast<float>(Complexity()); }
		
		// TODO: Remove symbols from release build
		virtual const char* GetName() const { return "ITask"; }

		template<typename T>
		long long GetExecutionTime() const
		{
			std::chrono::steady_clock::time_point endTime;
			if (Completed())
			{
				endTime = m_EndTime;
			}
			else
			{
				endTime = std::chrono::steady_clock::now();
			}

			const T duration = std::chrono::duration_cast<T>(endTime - m_StartTime);
			return duration.count();
		}

		void PrintTimeHierarchy(int tabCount = 0)
		{
			std::string tabs(tabCount, ' ');
			HINFO("{0}{1}: {2}ms", tabs, GetName(), GetExecutionTime<std::chrono::milliseconds>());
			for (const auto& internalDep : m_InternalDep)
			{
				internalDep->PrintTimeHierarchy(tabCount + 1);
			}
		}

		void Enqueue();
		bool IsEnqueued();

		void AddInDependency(ITask* task);
		void AddOutDependency(ITask* task);
		void AddInternalDependency(ITask* task);
		void SetParent(ITask* parent);

		// This function is called each time a task is completed to let the out dependencies that the task is completed
		virtual void DependencyCompletionNotification(ITask* task) = 0;

		const unordered_set<ITask*>& GetInternalDependencies() const;
		const unordered_set<ITask*>& GetInDependencies() const;
		const unordered_set<ITask*>& GetOutDependencies() const;
	protected:
		bool AreInDepResolved() const;
		bool AreInternalDepResolved() const;
	private:
		unordered_set<ITask*> m_InDep;
		unordered_set<ITask*> m_OutDep;
		unordered_set<ITask*> m_InternalDep;
		ITask* m_Parent = nullptr;
		bool m_Enqueued = false;

		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_EndTime;
	};
}