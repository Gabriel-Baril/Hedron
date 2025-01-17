#include "async_orchestrator.h"

namespace hdn
{
	AsyncOrchestrator& AsyncOrchestrator::Get()
	{
		static AsyncOrchestrator s_Instance;
		return s_Instance;
	}
}
