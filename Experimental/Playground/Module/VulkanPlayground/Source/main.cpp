#include <iostream>

#include "Core/Core.h"

int main()
{
	using namespace hdn;
#if USING( HDN_DEBUG )
	hdn::Log_Init();
#endif

	HDN_CORE_INFO("Hello VulkanPlayground");
	return 0;
}