#pragma once

#include "core/core.h"

namespace hdn
{
	class QueueGroup;

	class ITask
	{
	public:
		virtual void Run() = 0;
		bool Completed();
		void Complete();
		void SetGroup(QueueGroup* group);
	private:
		bool m_Done = false;
		QueueGroup* m_Group = nullptr;
	};
}