#include <iostream>

#include "core/Core.h"

int main()
{
	using namespace hdn;
#if USING(ENABLE_LOG)
	Log_Init();
#endif

	HINFO("Hello Ideon");
	return 0;
}