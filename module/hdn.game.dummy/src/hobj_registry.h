#pragma once
#include "core/random.h"

namespace hdn
{
	struct HObjectEntry
	{
		u16 refCount = 0;
		void* heapPtr = nullptr;
	};

	class HObjectRegistry
	{ 
		template<typename T>
		static T* get(uuid64 id)
		{
			HASSERT(registry.contains(id), "Cannot find hobj with id {0}", id);
			return static_cast<T*>(registry.at(id)->heapPtr);
		}

		template<typename T, typename... Args>
		static HRef<T> construct(const char* name, Args... args)
		{
			uuid64 id;
			if (name == nullptr)
			{
				id = random_uuid();
				HASSERT(!registry.contains(id), "Hash collision when generating hobj id from name, buy a lottery ticket!");
			}
			else
			{
				id = hash_str_to_uuid(name);
				HASSERT(!registry.contains(id), "An object named {0} already exist in the registry!", name);
			}
			return HObjectRegistry::construct<T>(id, forward(args)...);
		}

		template<typename T, typename... Args>
		static HRef<T> construct(uuid64 id, Args... args)
		{
			void* heapPtr = nullptr;
			if (registry.contains(id))
			{
				heapPtr = registry.at(id)->heapPtr;
			}
			else
			{
				heapPtr = reserve<T>(id)->heapPtr;
			}
			new (heapPtr) T(forward(args)...);

			return HRef<T>(id);
		}

		template<typename T>
		static HObjectEntry* reserve(uuid64 id)
		{
			HObjectEntry* entry = entryPool.allocate();
			HASSERT(entry, "Failed to allocate entry in hobj pool");
			const u64 reserveSize = sizeof(T);
			void* heapPtr = heap.reserve(reserveSize);
			entry->heapPtr = heapPtr;
			registry[id] = entry;
			return entry;
		}

		template<typename T>
		static T* get_raw(uuid64 id)
		{
			HASSERT(registry.contains(id), "Cannot find hobj with id {0}", id);
			return static_cast<T*>(registry.at(id)->heapPtr);
		}

		template<typename T>
		static void increment_ref(uuid64 id)
		{
			HObjectEntry* entry = nullptr;
			if (registry.contains(id))
			{
				entry = registry.at(id);
			}
			else
			{
				entry = HObjectRegistry::reserve<T>(id); // In some contexts the specified hobj id might not be in the registry yet. To allow out of order initialization, we preemtively reserve it in the pool. Garbage collection should later clean up the unused hobj.  
			}
			entry->refCount++;
		}

		static void decrement_ref(uuid64 id)
		{
			if (registry[id]->refCount != 0)
			{
				registry[id]->refCount--;
			}
			else
			{
				// Call hobj destructor
				// Free entry
				// Free hobj heap allocation
			}
		}

		Pool<HObjectEntry> entryPool(HOBJ_MAX_ENTRY_COUNT);
		HObjectHeap heap(20 * MB);
		unordered_map<uuid64, HObjectEntry*> registry;
	};
}