#include "cache_example_compute.h"

#include "cache.h"
#include "hobj/hobj_registry.h"

#define EXAMPLE_COMPUTE_V1 "EXAMPLE_COMPUTE_V1"

namespace hdn
{
	ExampleCompute example_compute(int number, int count)
	{
		HashBuilder builder;
		builder.add(EXAMPLE_COMPUTE_V1);
		builder.add(number);
		builder.add(count);

		HDataCache* cache = HObjectRegistry::get().get<HDataCache>("cache");
		HASSERT(cache, "Cache was not found");

		char* data = nullptr;
		if (cache->cache_entry_exist(builder.hash))
		{
			u64 entrySize = cache->cache_entry_size(builder.hash);
			data = new char[entrySize];
			cache->cache_fetch(builder.hash, data);
			ExampleCompute example;
			example = *reinterpret_cast<ExampleCompute*>(data);
			delete[] data;
			return example;
		}

		ExampleCompute example;
		example.number = number;
		example.count = count;
		cache->cache_create_entry(builder.hash, &example, sizeof(ExampleCompute));
		return example;
	}
}
