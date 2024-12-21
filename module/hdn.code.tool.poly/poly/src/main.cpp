#include <iostream>


#include "Core/Core.h"

int main()
{
	using namespace hdn;
#if USING(ENABLE_LOG)
	Log_Init();
#endif

	HINFO("Hello Poly");
	return 0;
}