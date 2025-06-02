#include "core/core.h"
#include "core/stl/unordered_map.h"

#include "utils.h"

#include "entity_component_parse.h"


int main()
{
	using namespace hdn;
	log_init();
	
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("prefabs/example_prefab.xml");
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
		m_Entities[uuidTou64].entity = ecsWorld.entity("ent");
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

	return 0;
}