#include "core/core.h"
#include "hobj/hobj.h"

#include "tlsf/tlsf.h"
#include "core/stl/vector.h"

namespace hdn
{

	enum class HObjMemoryFlags : u64
	{
		// Memory Usage
		MEMORY_USAGE_USED = BIT(1), // The amount of memory currently used
		MEMORY_USAGE_UNUSED = BIT(2), // The amount of memory currently unused
		MEMORY_USAGE_ANY = MEMORY_USAGE_USED | MEMORY_USAGE_UNUSED,

		// Memory Region
		MEMORY_REGION_CPU = BIT(3),
		MEMORY_REGION_GPU = BIT(4),
		MEMORY_REGION_ANY = MEMORY_REGION_CPU | MEMORY_REGION_GPU,

		// Memory Layer
		MEMORY_LAYER_ENGINE = BIT(5), // The memory is needed for the engine
		MEMORY_LAYER_APPLICATION = BIT(6), // The memory is needed by the application
		MEMORY_LAYER_ANY = MEMORY_LAYER_ENGINE | MEMORY_LAYER_APPLICATION,

		// Memory Source
		MEMORY_SOURCE_RESIDENT = BIT(7), // The resident data of the hobj itself
		MEMORY_SOURCE_MANAGED = BIT(8), // Memory that is managed by the object but not owned. Meaning it *reserved* memory from an allocator that own memory (or even one that reserved memory)
		MEMORY_SOURCE_OWNED = BIT(9), // Memory that is managed by the object and owned, meaning it is responsible to free the memory at the end. Mostly relevant for top level allocator. Owned memory always come from a dynamic memory allocation. 
		MEMORY_SOURCE_ANY = MEMORY_SOURCE_RESIDENT | MEMORY_SOURCE_MANAGED | MEMORY_SOURCE_OWNED,

		// Memory Lifetime Hint
		MEMORY_LIFETIME_TRANSIENT = BIT(10), // Short lived memory designed to be quickly discarded after the initial operation is done (for example, loading a file)
		MEMORY_LIFETIME_OBJECT_BOUND = BIT(11), // Bound to the object it belong 
		MEMORY_LIFETIME_FRAME = BIT(12),
		MEMORY_LIFETIME_ALWAYSLOADED = BIT(13),
		MEMORY_LIFETIME_ANY = MEMORY_LIFETIME_TRANSIENT | MEMORY_LIFETIME_OBJECT_BOUND | MEMORY_LIFETIME_FRAME | MEMORY_LIFETIME_ALWAYSLOADED,

		// Memory Access
		MEMORY_ACCESS_READ = BIT(14), // Memory can be read
		MEMORY_ACCESS_WRITE = BIT(15), // Memory can be written
		MEMORY_ACCESS_ANY = MEMORY_ACCESS_READ | MEMORY_ACCESS_WRITE,

		// Memory Context
		MEMORY_CONTEXT_DEV = BIT(16), // Memory only present in dev build
		MEMORY_CONTEXT_EXTERNAL = BIT(17), // Memory consumed by external libraries
		MEMORY_CONTEXT_ANY = MEMORY_CONTEXT_DEV | MEMORY_CONTEXT_EXTERNAL,

		// Broad Memory Type
		MEMORY_TYPE_STREAMER = BIT(18),
		MEMORY_TYPE_PHYSICS = BIT(19),
		MEMORY_TYPE_GFX = BIT(20),
		MEMORY_TYPE_ASSET = BIT(21),
		MEMORY_TYPE_EXECUTABLE = BIT(22),
		// ... Add new memory type has needed
		MEMORY_TYPE_ANY = MEMORY_TYPE_STREAMER | MEMORY_TYPE_PHYSICS | MEMORY_TYPE_GFX | MEMORY_TYPE_ASSET | MEMORY_TYPE_EXECUTABLE,


		ANY_USED = MEMORY_USAGE_USED | MEMORY_REGION_ANY | MEMORY_LAYER_ANY | MEMORY_SOURCE_ANY | MEMORY_LIFETIME_ANY | MEMORY_ACCESS_ANY | MEMORY_TYPE_ANY,
		ANY_UNUSED = MEMORY_USAGE_UNUSED | MEMORY_REGION_ANY | MEMORY_LAYER_ANY | MEMORY_SOURCE_ANY | MEMORY_LIFETIME_ANY | MEMORY_ACCESS_ANY | MEMORY_TYPE_ANY,
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjMemoryFlags);

	class IMemoryQueryable
	{
		virtual u64 get_memory(HObjMemoryFlags flags) = 0;
	};

	class HDummyObject : public HObject, public IMemoryQueryable
	{
	public:
		HDummyObject()
		{
			HINFO("Called HDummyObject constructor");
			ownedPtr = new int[maxCount];
			HRef<HMemoryOverview> hMemOverview = HObjectRegistry::get<HMemoryOverview>();
			hMemOverview->register_object(this);
		}
		HDummyObject(const HDummyObject&) = delete;
		HDummyObject& operator=(const HDummyObject&) = delete;

		u64 get_memory(HObjMemoryFlags flags) override
		{
			u64 memory = 0;
			const bool used = flag_on(HObjMemoryFlags::MEMORY_USAGE_USED, flags);
			const bool unused = flag_on(HObjMemoryFlags::MEMORY_USAGE_UNUSED, flags);
			const bool includeResident = flag_on(HObjMemoryFlags::MEMORY_SOURCE_RESIDENT, flags);
			const bool includeManaged = flag_on(HObjMemoryFlags::MEMORY_SOURCE_MANAGED, flags);
			const bool includeOwned = flag_on(HObjMemoryFlags::MEMORY_SOURCE_OWNED, flags);

			if (used)
			{
				if (includeResident) memory += sizeof(HDummyObject);
				if (includeManaged)  memory += 0;
				if (includeOwned)    memory += sizeof(int) * currentCount;
			}

			if (unused)
			{
				HASSERT(currentCount <= maxCount, "");
				if (includeOwned)    memory += sizeof(int) * (maxCount - currentCount);
			}

			return memory;
		}

		virtual ~HDummyObject()
		{
			hMemOverview->unregister_object(this);
			delete[] ownedPtr;
		}
	private:
		int a = 0;
		float b = 0;
		int* ownedPtr = nullptr;
		int maxCount = 5;
		int currentCount = 2;
	};

	class HMemoryOverview : public HObject
	{
	public:
		void register_object(IMemoryQueryable* mQueryable)
		{
			m_Queryable.push_back(mQueryable);
		}
	private:
		vector<IMemoryQueryable*> m_Queryable;
	};
}

void iterate_pool(void* ptr, size_t size, int used, void* user)
{
	HINFO("size: {0}, Used: {1}", size, used);
}

int main()
{
	using namespace hdn;
	log_init();

	const u64 allocatedSize = 1024 * 100;

	void* mem = new u8[allocatedSize];
	tlsf_t tlsf = tlsf_create_with_pool(mem, allocatedSize);
	void* block1 = tlsf_malloc(tlsf, sizeof(HDummyObject));
	HDummyObject* object = new (block1) HDummyObject();

	tlsf_walk_pool(tlsf_get_pool(tlsf), iterate_pool, nullptr);

	tlsf_free(tlsf, block1);
	tlsf_destroy(tlsf);
	delete[] mem;

	return 0;
}
