#include "mem_virtual_debug.h"
#include <windows.h>
#include <dbghelp.h>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>

#pragma comment(lib, "dbghelp.lib")

namespace dm
{
	void mem_virtual_print_memory_map(HANDLE hProcess)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		unsigned char *address = 0;
		SymInitialize(hProcess, NULL, TRUE);

		MEMORY_BASIC_INFORMATION mbi;
		IMAGEHLP_MODULE64 moduleInfo;
		moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

		while (address < si.lpMaximumApplicationAddress)
		{
			if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0)
				break;

			const char *stateStr =
					mbi.State == MEM_COMMIT ? "Committed" : mbi.State == MEM_RESERVE ? "Reserved"
																							: mbi.State == MEM_FREE			 ? "Free"
																																					 : "Unknown";

			const char *typeStr =
					mbi.Type == MEM_IMAGE ? "Image" : mbi.Type == MEM_MAPPED ? "Mapped"
																				: mbi.Type == MEM_PRIVATE	 ? "Private"
																																	 : "";

			// Only try to resolve module name if the region is committed
			std::string moduleName = "";
			if (mbi.State == MEM_COMMIT)
			{
				memset(&moduleInfo, 0, sizeof(moduleInfo));
				moduleInfo.SizeOfStruct = sizeof(moduleInfo);

				if (SymGetModuleInfo64(hProcess, (DWORD64)mbi.BaseAddress, &moduleInfo))
				{
					moduleName = moduleInfo.ImageName;
				}
				else
				{
					moduleName = "Anonymous";
				}
			}

			printf("Base: %p, Size: %-10zu, State: %-9s, Type: %-8s, Module: %s\n",
						 mbi.BaseAddress,
						 mbi.RegionSize,
						 stateStr,
						 typeStr,
						 moduleName.c_str());

			address += mbi.RegionSize;
		}

		SymCleanup(hProcess);
	}
}
