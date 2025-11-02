#include "strdb.h"

#include "core/memory/allocator_heap.h"
#include "core/memory/core_memory.h"

namespace hdn
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

		u8* strMemory = new u8[STRDB_MEMORY_POOL_SIZE];
		core_memset(strMemory, 0, STRDB_MEMORY_POOL_SIZE);
		heap_allocator_init(s_StrDBGlob.strAllocator, strMemory, STRDB_MEMORY_POOL_SIZE);
		s_StrDBGlob.initialized = true;
	}

	void strdb_shutdown()
	{
		if (!s_StrDBGlob.initialized)
		{
			s_StrDBGlob.initialized = false;
		}

		heap_allocator_shutdown(s_StrDBGlob.strAllocator);
		delete[] s_StrDBGlob.strAllocator.memory;
		s_StrDBGlob.initialized = false;
	}

	char* strdb_allocate(const char* source, u64 len)
	{	
		char* ptr = (char*)heap_allocator_allocate(s_StrDBGlob.strAllocator, sizeof(char) * len, alignof(char));
		core_strcpy(ptr, source);
		return ptr;
	}

	char* strdb_allocate(const char* source)
	{
		return strdb_allocate(source, strlen(source));
	}

	void strdb_deallocate(char* source)
	{
		heap_allocator_deallocate(s_StrDBGlob.strAllocator, source);
	}
}
