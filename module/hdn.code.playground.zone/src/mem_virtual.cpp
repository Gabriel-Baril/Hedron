#include "mem_virtual.h"

#if USING(HDN_PLATFORM_WINDOWS)
#include <windows.h>
#include <stdio.h>
#endif

namespace hdn
{
	struct MemVirtualGlob
	{
		u64 osPageMemorySizeByte = 0;
		u64 osAllocationGranularity = 0;
		bool initialized = false;
	};

	static MemVirtualGlob s_MemVirtualGlob;

	static bool is_power_2(u64 n)
	{
		return n != 0 && (n & (n - 1)) == 0;
	}

	static bool is_ptr_aligned(void* address, u64 alignment)
	{
		HDN_CORE_ASSERT(is_power_2(alignment), "Alignment is not a power of 2");
		return (reinterpret_cast<uintptr_t>(address) & (alignment - 1)) == 0;
	}

	bool is_size_aligned(u64 size, u64 alignment)
	{
		HDN_CORE_ASSERT(is_power_2(alignment), "Alignment is not a power of 2");
		return (size & (alignment - 1)) == 0;
	}

	static void* get_page_address(void* addr, u64 pageSize)
	{
		HDN_CORE_ASSERT(is_power_2(pageSize), "pageSize is not a power of 2");
		return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(addr) & ~(pageSize - 1));
	}

	static void init_os_page_size()
	{
#if USING(HDN_PLATFORM_WINDOWS)
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		s_MemVirtualGlob.osPageMemorySizeByte = si.dwPageSize;
		s_MemVirtualGlob.osAllocationGranularity = si.dwAllocationGranularity;
		HDN_CORE_ASSERT(is_power_2(s_MemVirtualGlob.osPageMemorySizeByte), "os page size is not a power of 2");
		HDN_CORE_ASSERT(is_power_2(s_MemVirtualGlob.osAllocationGranularity), "os allocation granularity is not a power of 2");
#else
#error "No implementation for zone_init_os_page_size for the currently selected platform!"
#endif
	}

	void mem_virtual_init()
	{
		if (s_MemVirtualGlob.initialized)
		{
			return;
		}
		init_os_page_size();
		s_MemVirtualGlob.initialized = true;
	}

	bool mem_virtual_initialized()
	{
		return s_MemVirtualGlob.initialized;
	}

	void* mem_virtual_reserve(u64 reservationSizeByte)
	{
		const SIZE_T bytesToReserve = reservationSizeByte;
		HDN_CORE_ASSERT(is_size_aligned(bytesToReserve, s_MemVirtualGlob.osPageMemorySizeByte), "Alignment error");
		
#if USING(HDN_PLATFORM_WINDOWS)
		LPVOID reservedMemory = VirtualAlloc(NULL, bytesToReserve, MEM_RESERVE, PAGE_NOACCESS);
		HDN_CORE_ASSERT(reservedMemory, "Failed to allocate virtual memory");
		HDN_CORE_ASSERT(is_ptr_aligned(reservedMemory, s_MemVirtualGlob.osAllocationGranularity), "Base reserve address is not a multiple of os allocation granularity");
		return reservedMemory;
#else
#error "No implementation for mem_virtual_reserve for the currently selected platform!"
#endif
		return nullptr;
	}

	void* mem_virtual_commit(void* pageAddress, u64 commitSize)
	{
		HDN_CORE_ASSERT(is_ptr_aligned(pageAddress, s_MemVirtualGlob.osPageMemorySizeByte), "Alignment error");
		HDN_CORE_ASSERT(is_size_aligned(commitSize, s_MemVirtualGlob.osPageMemorySizeByte), "Alignment error");
		LPVOID commitedMemory = VirtualAlloc(pageAddress, commitSize, MEM_COMMIT, PAGE_READWRITE);
		return commitedMemory;
	}
}