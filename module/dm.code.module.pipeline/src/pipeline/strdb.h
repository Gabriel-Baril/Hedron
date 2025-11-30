#pragma once

#include "core/core.h"

// General purpose system dedicated to manage a heap of immutable strings to prevent the memory, performance and fragmentation cost of heap-allocated string

namespace dm
{
	using hstring = std::string_view;

	void strdb_init();
	void strdb_shutdown();
	char *strdb_allocate(const char *source, u64 len);
	char *strdb_allocate(const char *source);
	void strdb_deallocate(char *source);
}
