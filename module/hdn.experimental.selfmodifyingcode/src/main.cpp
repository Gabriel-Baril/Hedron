#include "core/core.h"
#include "core/core_filesystem.h"
#include <windows.h>

unsigned char code[] = {
	0xB8, 0x2A, 0x00, 0x00, 0x00, // mov eax, 42 (0x2A in hex)
	0xC3
};

void ExecSelfModifyingFunc()
{
	void* execMemory = VirtualAlloc(
		nullptr,
		sizeof(code),
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE // Allow reading, writing, and execution
	);

	if (!execMemory)
	{
		HERR("Virtual Alloc Failed");
		return;
	}

	memcpy(execMemory, code, sizeof(code));

	unsigned char* codePtr = reinterpret_cast<unsigned char*>(execMemory);
	// codePtr[1] = 0x63;

	auto func = reinterpret_cast<int(*)()>(execMemory);
	int result = func();
	HINFO("Result is '{0}'", result);

	VirtualFree(execMemory, 0, MEM_RELEASE);
}



int main()
{
	using namespace hdn;
	Log_Init();

	return 0;
}