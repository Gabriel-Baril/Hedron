#include "strdb.h"

#include "core/memory/allocator_heap_string.h"
#include "core/memory/core_memory.h"

namespace dm
{
	static constexpr u64 STRDB_MEMORY_POOL_SIZE = 50 * MB;

	struct StrDBGlob
	{
		bool initialized = false;
		HeapAllocator strAllocator;
	} s_StrDBGlob;

	void strdb_init()
	{
		if (s_StrDBGlob.initialized)
		{
			return;
		}

		u8 *strMemory = new u8[STRDB_MEMORY_POOL_SIZE];
		core_memset(strMemory, 0, STRDB_MEMORY_POOL_SIZE);
		str_heap_allocator_init(s_StrDBGlob.strAllocator, strMemory, STRDB_MEMORY_POOL_SIZE);
		s_StrDBGlob.initialized = true;
	}

	void strdb_shutdown()
	{
		if (!s_StrDBGlob.initialized)
		{
			return;
		}

		str_heap_allocator_shutdown(s_StrDBGlob.strAllocator);
		delete[] s_StrDBGlob.strAllocator.memory;
		s_StrDBGlob.initialized = false;
	}

	char *strdb_allocate(const char *source, u64 len)
	{
		return str_heap_allocator_allocate(s_StrDBGlob.strAllocator, source, len); // str_heap_allocator_allocate already core_memcpy the data into the dst string
	}

	char *strdb_allocate(const char *source)
	{
		const size_t len = strlen(source) + 1;
		return strdb_allocate(source, len);
	}

	void strdb_deallocate(char *source)
	{
		str_heap_allocator_deallocate(s_StrDBGlob.strAllocator, source);
	}
}
