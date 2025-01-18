#pragma once

#include "core/core.h"
#include "async_task.h"

namespace hdn
{
	class ITaskLeaf : public ITask
	{
	public:
		ITaskLeaf();
		virtual ~ITaskLeaf() = default;
		virtual bool Completed() const override;
		virtual void Complete() override;
		virtual void DependencyCompletionNotification(ITask* task) override;

		virtual const char* GetName() const override;
	private:
		bool m_Completed = false;
	};
}