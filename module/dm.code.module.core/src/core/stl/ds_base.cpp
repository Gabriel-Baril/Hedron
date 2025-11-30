#include "ds_base.h"

#include "core/core_define.h"
#include "core/core_filesystem.h"

#include <cstdlib>
#include <new>

static MemStat s_MemStat;

void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	HDN_MAYBE_UNUSED(name);
	HDN_MAYBE_UNUSED(flags);
	HDN_MAYBE_UNUSED(debugFlags);
	HDN_MAYBE_UNUSED(file);
	HDN_MAYBE_UNUSED(line);
	// HDEBUG("Allocating {0} bytes at {1} : {2} (name: {3}, flags: {4}, debugFlags: {5})", size, file, line, name, flags, debugFlags);

	// Use the standard new operator for allocation
	return ::operator new[](size);
}

void operator delete[](void* ptr, const char* name, int flags, unsigned debugFlags, const char* file, int line) noexcept
{
	HDN_MAYBE_UNUSED(name);
	HDN_MAYBE_UNUSED(flags);
	HDN_MAYBE_UNUSED(debugFlags);
	HDN_MAYBE_UNUSED(file);
	HDN_MAYBE_UNUSED(line);
	// HDEBUG("Deallocating Memory");
	::operator delete[](ptr);
}

void* operator new[](size_t size, size_t alignment, size_t offset, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	HDN_MAYBE_UNUSED(alignment);
	HDN_MAYBE_UNUSED(offset);
	HDN_MAYBE_UNUSED(name);
	HDN_MAYBE_UNUSED(flags);
	HDN_MAYBE_UNUSED(debugFlags);
	HDN_MAYBE_UNUSED(file);
	HDN_MAYBE_UNUSED(line);
	// HDEBUG("Allocating {0} bytes with alignment {1} and offset {2} at {3} : {4} (name: {5}, flags: {6} debugFlags: {7})", size, alignment, offset, file, line, name, flags, debugFlags);
	
	// Ensure alignment and offset are respected
	// 	void* ptr = nullptr;
	// 
	// #if USING(HDN_PLATFORM_WINDOWS)
	// 	ptr = _aligned_offset_malloc(size, alignment, offset);
	// 	if (!ptr) throw std::bad_alloc();
	// #else
	// 	if (posix_memalign(&ptr, alignment, size + offset) != 0 || !ptr) throw std::bad_alloc();
	// #endif

	return new uint8_t[size];
}

void operator delete[](void* ptr, size_t alignment, size_t offset, const char* name, int flags, unsigned debugFlags, const char* file, int line) noexcept
{
	HDN_MAYBE_UNUSED(alignment);
	HDN_MAYBE_UNUSED(offset);
	HDN_MAYBE_UNUSED(name);
	HDN_MAYBE_UNUSED(flags);
	HDN_MAYBE_UNUSED(debugFlags);
	HDN_MAYBE_UNUSED(file);
	HDN_MAYBE_UNUSED(line);

	// HDEBUG("Deallocating Aligned Memory");
	
	// #if USING(HDN_PLATFORM_WINDOWS)
	// 	_aligned_free(ptr);
	// #else
	// 	std::free(ptr);
	// #endif
	delete[] ptr;
}

void* operator new(size_t size)
{
	s_MemStat.allocated += size;
	s_MemStat.allocationCount++;
	return std::malloc(size);
}

void operator delete(void* ptr) noexcept
{
	s_MemStat.deallocationCount++;
	std::free(ptr);
}

MemStat GetMemStat()
{
	return s_MemStat;
}
