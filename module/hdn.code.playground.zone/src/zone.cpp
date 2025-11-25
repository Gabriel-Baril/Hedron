#include "zone.h"
#include "mem_virtual.h"

namespace hdn
{
	struct ZoneGlob
	{
		void* zoneVirtualMemoryBase = nullptr;
		bool initialized = false;
	};

	static ZoneGlob s_ZoneGlob;

	void zone_init()
	{
		if (zone_initialized())
		{
			return;
		}
		HDN_CORE_ASSERT(mem_virtual_initialized(), "Trying to use zone before mem_virtual initialization");

		s_ZoneGlob.initialized = true;
	}

	bool zone_initialized()
	{
		return s_ZoneGlob.initialized;
	}

	void zone_reserve_virtual_range(u64 totalZoneMemory)
	{
		HDN_CORE_ASSERT(zone_initialized(), "Trying to use zone system before initialization");
		HDN_CORE_ASSERT(is_size_aligned(totalZoneMemory, ZONE_BLOCK_SIZE), "totalZoneMemory should be a multiple of ZONE_BLOCK_SIZE");
		s_ZoneGlob.zoneVirtualMemoryBase = mem_virtual_reserve(totalZoneMemory);
	}

	void zone_load(const char* zone)
	{
	}
}
