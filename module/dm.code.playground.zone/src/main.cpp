#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"
#include "core/stl/map.h"

#include "mem_virtual.h"
#include "mem_virtual_debug.h"
#include "core/memory/allocator_linear.h"

#include "zone.h"

#define DB_ALLOC()

namespace dm
{

	using xhash = u64;

	struct Component
	{
		float a;
		int b;
	};

	struct Entity
	{
		xhash name;
		int componentCount;
		Component *components;
	};

	struct Scene
	{
		xhash name;
		Entity *entities;
		int entityCount;
	};

	enum class DbAllocType
	{
		OBJECT,
		SUBOBJECT,
		STREAMED
	};

	struct DbAllocEntry
	{
		void *ptr;
		u64 size;
	};

	struct DbGlob
	{
		LinearAllocator dbStreamAllocator;
		map<void *, DbAllocEntry> entries;
	} s_dbGlob;

	template <typename T>
	T *db_alloc(int count)
	{
		const u64 memSize = sizeof(T) * count;
		void *mem = linear_allocator_allocate(s_dbGlob.dbStreamAllocator, memSize, alignof(T));
		DM_CORE_ASSERT(!s_dbGlob.entries.contains(mem), "Trying to add a pointer already there in the db stream");
		auto &allocEntry = s_dbGlob.entries[mem];
		allocEntry.ptr = mem;
		allocEntry.size = memSize;
		return static_cast<T *>(mem);
	}

	void scene_assemble(xhash name)
	{
		Scene *scene = db_alloc<Scene>(1);
		scene->name = name;

		// In reality the values filled here would be based on the provided xhash name. Given the xhash, we would usually fetch the homologous symbol and fetch its information to populate/drive the values of the struct
		// DUMMY_BEGIN
		scene->entityCount = 4;
		scene->entities = db_alloc<Entity>(scene->entityCount);
		for (int i = 0; i < scene->entityCount; i++)
		{
			Entity &ent = scene->entities[i];
			ent.name = 0;

			ent.componentCount = i;
			ent.components = db_alloc<Component>(ent.componentCount);
			for (int j = 0; j < ent.componentCount; j++)
			{
				ent.components[j].a = i;
				ent.components[j].b = i + j;
			}
		}
		// DUMMY_END
	}
}

void example_reserve_zone_mem()
{
	using namespace dm;
	constexpr u64 TOTAL_ZONE_MEMORY = 2048 * ZONE_BLOCK_SIZE; // 4MB, simulate complex determination of total zone memory size
	zone_reserve_virtual_range(TOTAL_ZONE_MEMORY);
	mem_virtual_print_memory_map(GetCurrentProcess());
}

void example_db_alloc()
{
	using namespace dm;

	// Init linear db stream
	constexpr u64 DB_STREAM_BUDGET = 20 * MB;
	void *dbStreamMem = mem_virtual_reserve(DB_STREAM_BUDGET);
	linear_allocator_init(s_dbGlob.dbStreamAllocator, dbStreamMem, DB_STREAM_BUDGET);

	xhash sceneName = 0; // For now assume the hash will be provided by another system
	scene_assemble(0);
}

int main()
{
	using namespace dm;
	log_init();
	mem_virtual_init();
	zone_init();

	return 0;
}
