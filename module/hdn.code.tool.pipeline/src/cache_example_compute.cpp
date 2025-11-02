#include "cache_example_compute.h"

#include "srcdb/cache.h"
#include "hobj/hobj_registry.h"

#define EXAMPLE_COMPUTE_V1 "EXAMPLE_COMPUTE_V1"

namespace hdn
{
	FExampleCompute example_compute(int number, int count)
	{
		HashBuilder builder;
		builder.add(EXAMPLE_COMPUTE_V1);
		builder.add(number);
		builder.add(count);

		char *data = nullptr;
		if (cache_obj_exist(builder.hash))
		{
			u64 entrySize = cache_obj_size(builder.hash);
			data = new char[entrySize];
			cache_obj_load(builder.hash, data);
			FExampleCompute example;
			example = *reinterpret_cast<FExampleCompute *>(data);
			delete[] data;
			return example;
		}

		FExampleCompute example;
		example.number = number;
		example.count = count;
		cache_obj_store(builder.hash, &example, sizeof(FExampleCompute));
		return example;
	}
}
