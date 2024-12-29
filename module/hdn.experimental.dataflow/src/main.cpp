#include "core/core.h"
#include <iostream>

#include "hdef_asset_alias.h"
#include "path_asset_alias.h"

bool on_get_callback(void* payload, int payloadByteSize)
{
	hdn::DummyData dummyData = *(hdn::DummyData*)payload;
	HINFO("a:{0} b:{1} c:{2}", dummyData.a, dummyData.b, dummyData.c);
	return true;
}

bool on_get_image_callback(void* payload, int payloadByteSize)
{
	HINFO("Image Pixel Count: {0}", payloadByteSize);
	return true;
}

template<typename T, typename Func>
void GetData(const T& alias, const Func& dataCallback)
{
	auto key = hdn::alias_key_mapper<T>::map(alias);
	hdn::key_resource_mapper<decltype(key)>::get(key, dataCallback);
}

class ComplexObject
{
public:
	void ComplexOnGetCallback(void* payload, int payloadByteSize)
	{
		HINFO("ComplexObject::ComplexOnGetCallback: {0}", payloadByteSize);
	}

	void Test()
	{
		GetData(hdn::hdef_asset_alias{ "maison2" }, [this](void* payload, int payloadByteSize) {
			ComplexOnGetCallback(payload, payloadByteSize);
		});
	}
};

int main()
{
	using namespace hdn;
	Log_Init();

	int externalData = 90;
	GetData(hdef_asset_alias{ "sd" }, on_get_callback);
	GetData(hdef_asset_alias{ "maison" }, [&externalData](void* payload, int payloadByteSize) {
		HWARN("Empty Callback Test: {0}, {1}", payloadByteSize, externalData);
	});
	ComplexObject object;
	object.Test();

	GetData(path_asset_alias{ "content/SnippetArticulationRC.PNG" }, on_get_image_callback);
}