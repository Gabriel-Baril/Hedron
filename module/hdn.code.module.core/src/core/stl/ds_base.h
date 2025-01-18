#pragma once

#include <EASTL/internal/config.h>

#define HDN_DEFAULT_ALLOCATOR EASTLAllocatorType
#define HDN_DUMMY_ALLOCATOR EASTLDummyAllocatorType

struct MemStat
{
	int allocated = 0;
	int allocationCount = 0;
	int deallocated = 0;
	int deallocationCount = 0;
};

// TODO: Take a look at this file 'module\hdn.code.external.eastl\include\EASTL\allocator.h' for further details about why we need to define these
void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line);
void operator delete[](void* ptr, const char* name, int flags, unsigned debugFlags, const char* file, int line) noexcept;

void* operator new[](size_t size, size_t alignment, size_t offset, const char* name, int flags, unsigned debugFlags, const char* file, int line);
void operator delete[](void* ptr, size_t alignment, size_t offset, const char* name, int flags, unsigned debugFlags, const char* file, int line) noexcept;

void* operator new(size_t size);
void operator delete(void* ptr) noexcept;

MemStat GetMemStat();