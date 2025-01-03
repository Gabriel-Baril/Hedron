#include "core/core.h"
#include <iostream>

#include "key_mapper.h"
#include "hdef_packed_data_key.h"
#include "hdef_named_data_key.h"
#include "path_data_key.h"

namespace hdn
{
	template<typename T>
	void GetData(const T& key, DataResult& result)
	{
		key_mapper<T>::get(key, result);
	}

	void GetDefinition()
	{

	}
}

int main()
{
	using namespace hdn;
	Log_Init();

	{
		DataResult result;
		GetData(path_data_key{ "content/SnippetArticulationRC.PNG", path_data_key::Image }, result);
		Image* image = static_cast<Image*>(result.payload);
		HINFO("Image Dimension ({0}, {1})", image->GetWidth(), image->GetHeight());
		HINFO("Image Ptr: {0}", (void*)image);
	}
}