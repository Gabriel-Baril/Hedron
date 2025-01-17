#pragma once

#include "async_scheduler.h"

namespace hdn
{
	class LinearScheduler : public IScheduler
	{
	public:
		void Add(Ref<ITask> task);
		Ref<Queue> Resolve() override;
	private:
		TVector<Ref<ITask>> m_Tasks;
	};
}