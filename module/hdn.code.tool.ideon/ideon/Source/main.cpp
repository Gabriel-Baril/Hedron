#include <iostream>

#include "Core/Core.h"

int main()
{
	using namespace hdn;
#if USING(ENABLE_LOG)
	Log_Init();
#endif

	HDN_CORE_INFO("Hello Ideon");
	return 0;
}