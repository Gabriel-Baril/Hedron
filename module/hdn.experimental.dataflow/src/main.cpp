#include "core/core.h"
#include <iostream>

#include "hobj_def_lightcfg.h"
#include "hobj_def_scene.h"

#include "hobj_util.h"
#include "hobj_registry.h"

#include "core/core_filesystem.h"

namespace hdn
{

	void Print(const HLightConfig& config)
	{
		HINFO("Key: {0}", config.GetKey());
		HINFO("Path: {0}", config.GetPath().c_str());
		HINFO("MaxPrimaryLightCount: {0}", config.GetMaxPrimaryLightCount());
		HINFO("MaxSecondaryLightCount: {0}", config.GetMaxSecondaryLightCount());
	}

	void Print(const HScene& scene)
	{
		HINFO("Key: {0}", scene.GetKey());
		HINFO("Path: {0}", scene.GetPath().c_str());
		HINFO("-- Light Config --");
		Print(*scene.GetLightConfig());
	}

	void CreateObjectExample()
	{
		HObjPtr<HLightConfig> lightConfig = HObjectUtil::Create<HLightConfig>();
		lightConfig->SetMaxPrimaryLightCount(10);
		lightConfig->SetMaxSecondaryLightCount(2);
		
		HObjPtr<HScene> scene = HObjectUtil::Create<HScene>();
		scene->SetLightConfig(lightConfig);
		
		HObjectUtil::Save(lightConfig, "object/light_config.ho");
		HObjectUtil::Save(scene, "object/scene.ho");
	}

	void LoadObjectExample()
	{
		HObjPtr<HScene> scene = HObjectUtil::GetObjectFromPath<HScene>("object/scene.ho");
		Print(*scene);
		scene = HObjectUtil::GetObjectFromPath<HScene>("object/scene.ho");
		Print(*scene);
	}

	bool IsHObjectFile(const fspath& path)
	{
		return FileSystem::Extension(path) == HOBJ_FILE_EXT;
	}

	void IterateHObject()
	{
		TVector<fspath> files = FileSystem::Walk("object/", IsHObjectFile);
		for (const auto& file : files)
		{
			std::ifstream inFile(file, std::ios::binary);
			if (!inFile) {
				std::cerr << "Error: Could not open file " << file << " for reading.\n";
				continue;
			}

			const size_t bufferSize = sizeof(u64) + sizeof(HObjectTypeHash) + sizeof(HObjectKey);
			char fileData[bufferSize];
			inFile.read(fileData, bufferSize);
			if (!inFile) {
				std::cerr << "Error: Failed to read from the file.\n";
				continue;
			}

			inFile.close();

			HObjectKey key = *reinterpret_cast<HObjectKey*>(fileData + sizeof(u64) + sizeof(HObjectTypeHash));
			HObjectRegistry::Get().RegisterObjectPath(key, file);
		}
	}
}

int main()
{
	using namespace hdn;
	Log_Init();
	// CreateObjectExample();
	IterateHObject();
	LoadObjectExample();
}