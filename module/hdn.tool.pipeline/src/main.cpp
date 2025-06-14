#include "core/core.h"
#include "core/stl/unordered_map.h"

#include "hobj/scene/scene_hobj.h"
#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"

#include "config/config.h"

#include "utils.h"
#include "entity_component_parse.h"
#include "cache.h"
#include "args.h"

namespace hdn
{
	void hobj_registry_init()
	{
		HObjectRegistry& registry = HObjectRegistry::get();
		std::string objectPath = Configuration::get().get_root_config_variable(CONFIG_SECTION_PIPELINE, CONFIG_KEY_OBJECTS_PATH, "");
		registry.add_source<FilesystemObjectSource>("local", objectPath);
		registry.populate();
	}
}

int main(int argc, char* argv[])
{
	using namespace hdn;
	log_init();
	if (!args_init(argc, argv))
	{
		return 1;
	}
	cache_init();
	hobj_registry_init();

	HINFO("Pipeline started");

	const PipelineCmdArgs& args = args_get();

	std::string pipelineModulePath = Configuration::get().get_root_config_variable(CONFIG_SECTION_PIPELINE, CONFIG_KEY_PIPELINE_MODULE_PATH, "");
	std::string scenePath = fmt::format("{0}\\{1}", pipelineModulePath, args.scenePath);

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(scenePath.c_str()); // "prefabs/example_prefab.xml"
	if (!result)
	{
		return -1;
	}

	flecs::world ecsWorld;
	unordered_map<u64, EntityDef> m_Entities;
	for (pugi::xml_node entityNode: doc.child("Prefab").children("Entity"))
	{
		const pugi::char_t* uuid = entityNode.attribute("uuid").as_string();
		HASSERT(uuid, "Entity without uuid found");

		const pugi::char_t* name = entityNode.attribute("name").as_string();
		if (name)
		{
			HWARN("Entity: '{0}' ({1})", name, uuid);
		}
		else
		{
			HWARN("Entity: '<unnamed>' ({0})", uuid);
		}

		u64 uuidTou64 = str_uuid_to_u64(uuid);
		m_Entities[uuidTou64].entity = ecsWorld.entity();
		m_Entities[uuidTou64].uuid = uuidTou64;

		for (pugi::xml_node childNode : entityNode.children())
		{
			if (entity_parse_component(childNode, m_Entities[uuidTou64]))
			{
				HWARN("Parsed '{0}' component", childNode.name());
			}
			else
			{
				HWARN("Failed to parse '{0}' component", childNode.name());
			}
		}
	}

	HRef<HScene> scene = HObjectRegistry::get().new_object<HScene>("scene_01");
	scene_set_world(scene->get_scene(), ecsWorld);
	HObjectFilesystemData data;
	data.path = "scene_01.hobj";
	HObjectRegistry::get().object_save(scene.get(), "local", &data, sizeof(data));

	return 0;
}