#include "core/core.h"

int main()
{
	using namespace hdn;
	Log_Init();

	HTRACE("Test");
	HDEBUG("Test");
	HINFO("Test");
	HWARN("Test");
	HERR("Test");
	HCRIT("Test");
}