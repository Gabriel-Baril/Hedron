#include "core/core.h"
#include <iostream>

#include "key_mapper.h"
#include "hdef/hdef_packed_data_key.h"
#include "hdef/hdef_named_data_key.h"
#include "path_data_key.h"

#include "homogeneous_memory_pool.h"

namespace hdn
{
	template<typename T>
	void GetData(const T& key, DataResult& result)
	{
		key_mapper<T>::get(key, result);
	}
}

struct Point
{
	float x;
	float y;
};

int main()
{
	using namespace hdn;
	Log_Init();

	void* mem = new byte[sizeof(Point) * 10];
	pool_allocator pool(sizeof(Point), 10, mem);
	Point* point = static_cast<Point*>(pool.Allocate());


	// {
	// 	DataResult result;
	// 	GetData(path_data_key{ "content/SnippetArticulationRC.PNG", path_data_key::Image }, result);
	// 	Image* image = static_cast<Image*>(result.payload);
	// 	HINFO("Image Dimension ({0}, {1})", image->GetWidth(), image->GetHeight());
	// 	HINFO("Image Ptr: {0}", (void*)image);
	// 	GetData(hdef_named_data_key{ "scene_config_asset_1" }, result);
	// }
}