#pragma once

#include <EASTL/internal/config.h>

#define DM_DEFAULT_ALLOCATOR EASTLAllocatorType
#define DM_DUMMY_ALLOCATOR EASTLDummyAllocatorType

struct MemStat
{
	size_t allocated = 0;
	size_t allocationCount = 0;
	size_t deallocated = 0;
	size_t deallocationCount = 0;
};

// TODO: Take a look at this file 'module\dm.code.external.eastl\include\EASTL\allocator.h' for further details about why we need to define these
void *operator new[](size_t size, const char *name, int flags, unsigned debugFlags, const char *file, int line);
void operator delete[](void *ptr, const char *name, int flags, unsigned debugFlags, const char *file, int line) noexcept;

void *operator new[](size_t size, size_t alignment, size_t offset, const char *name, int flags, unsigned debugFlags, const char *file, int line);
void operator delete[](void *ptr, size_t alignment, size_t offset, const char *name, int flags, unsigned debugFlags, const char *file, int line) noexcept;

void *operator new(size_t size);
void operator delete(void *ptr) noexcept;

MemStat GetMemStat();
