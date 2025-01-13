#include "core/core.h"
#include <iostream>

#include "hobj_def_lightcfg.h"

namespace hdn
{
	void Print(const HLightConfig& config)
	{
		HINFO("Key: {0}", config.GetKey());
		HINFO("Path: {0}", config.GetPath().c_str());
		HINFO("MaxPrimaryLightCount: {0}", config.GetMaxPrimaryLightCount());
		HINFO("MaxSecondaryLightCount: {0}", config.GetMaxSecondaryLightCount());
	}

	void CreateObjectExample()
	{
		HLightConfig* object = HObject::Create<HLightConfig>();
		object->SetMaxPrimaryLightCount(10);
		object->SetMaxSecondaryLightCount(2);
		HObject::Save(object, "object/light_config.ho");
		Print(*object);
	}

	void LoadObjectExample()
	{
		HLightConfig* object = HObject::LoadFromPath<HLightConfig>("object/light_config.ho");
		Print(*object);
	}
}

int main()
{
	using namespace hdn;
	Log_Init();
	CreateObjectExample();
	std::cout << "----------------\n";
	LoadObjectExample();
}