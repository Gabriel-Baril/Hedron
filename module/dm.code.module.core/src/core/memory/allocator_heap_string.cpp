#include "allocator_heap_string.h"
#include "core_memory.h"

namespace hdn
{
	void str_heap_allocator_init(HeapAllocator& allocator, void* memory, u64 size)
	{
		heap_allocator_init(allocator, memory, size);
	}

	void str_heap_allocator_shutdown(HeapAllocator& allocator)
	{
		heap_allocator_shutdown(allocator);
	}

	char* str_heap_allocator_allocate(HeapAllocator& allocator, const char* source, u64 len)
	{
		char* ptr = (char*)heap_allocator_allocate(allocator, sizeof(char) * len, alignof(char));
		core_strcpy(ptr, source);
		return ptr;
	}

	char* str_heap_allocator_allocate(HeapAllocator& allocator, const char* source)
	{
		return str_heap_allocator_allocate(allocator, source, strlen(source));
	}

	void str_heap_allocator_deallocate(HeapAllocator& allocator, char* source)
	{
		heap_allocator_deallocate(allocator, source);
	}
}
