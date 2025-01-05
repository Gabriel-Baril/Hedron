#include "hdef_loader.h"

#include "config/config.h"
#include "core/core_filesystem.h"
#include "fmt/format.h"
#include "hdef/LightConfig_generated.h"
#include "hdef/SceneConfig_generated.h"
#include "hdef/Definition_generated.h"

#include <fstream>

namespace hdn
{
	void PrintDef(const Hedron::Definition::Definition& definition)
	{
		const auto* sig = definition.signature();
		HINFO("Definition Name: '{0}'", sig->c_str());

		const Hedron::Definition::DefinitionMetadata* metadata = definition.metadata();
		HINFO("Definition Type: '{0}'", metadata->arch_type()->c_str());
		HINFO("Definition Type Chain: '{0}'", metadata->type_chain()->c_str());
		HINFO("Definition Version: '{0}'", metadata->version());

		const auto* dependencies = definition.dependencies();
		for (auto offset : *dependencies)
		{
			const auto* dependency = offset;
			if (dependency)
			{
				HINFO("Definition Dependency: '{0}'", dependency->c_str());
			}
		}
		HINFO("Definition Payload Size: '{0}'", definition.data()->Length());
	}

	void PrintLightConfig(const Hedron::Definition::LightConfig& lightConfig)
	{
		HINFO("Max Probes: {0}", lightConfig.max_probes());
		HINFO("Max Primary Light: {0}", lightConfig.max_primary_light());
	}

	void PrintSceneConfig(const Hedron::Definition::SceneConfig& sceneConfig)
	{
		HINFO("Generate Proxy Geo: {0}", sceneConfig.generate_proxy_geo());
		HINFO("Light Config: {0}", sceneConfig.light_config()->c_str());
		// PrintLightConfig(*sceneConfig.light_config());
	}

	void HdefCompile(const char* defName)
	{
		string defFolder = Configuration::Get().GetRootConfigVariable("data", "DefinitionFolder", "");
		string defFilename = fmt::format("{}.hdef", defName);

		fspath defFolderPath = defFolder;
		fspath defPath = defFolderPath / defFilename;

		if (FileSystem::DirectoryHasFile(defFolder, defPath))
		{
			// Load hdef file
			// const Hedron::Definition::LightConfig* lightConfig = Hedron::Definition::GetLightConfig()
			std::ifstream defFile(defPath, std::ios::binary);
			if (!defFile.is_open())
			{
				HWARN("Cannot open file {0}", defPath.string().c_str());
			}
			std::vector<char> defBuffer((std::istreambuf_iterator<char>(defFile)), std::istreambuf_iterator<char>());
			HTRACE("Definition Size '{0}'", defBuffer.size());
			const Hedron::Definition::Definition* definition = Hedron::Definition::GetDefinition(defBuffer.data());
			HASSERT(definition, "Definition cannot be null");
			const Hedron::Definition::SceneConfig* sceneConfig = Hedron::Definition::GetSceneConfig(definition->data()->Data());
			// const Hedron::Definition::LightConfig* lightConfig = Hedron::Definition::GetLightConfig(definition->data()->Data());
			// HASSERT(lightConfig, "LightConfig cannot be null");

			PrintDef(*definition);
			PrintSceneConfig(*sceneConfig);
			// PrintLightConfig(*lightConfig);

			defFile.close();
		}
		else
		{
			// Invoke hdef compiler
		}
	}
}
