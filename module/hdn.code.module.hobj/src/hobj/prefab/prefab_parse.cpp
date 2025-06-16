#include "prefab_parse.h"

#include "flecs/flecs.h"

#include "prefab_component_parse.h"
#include "prefab_component_uuid.h"

#include "utils.h"

namespace hdn
{
	bool prefab_parse(Prefab& prefab, const fspath& prefabPath)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(prefabPath.c_str()); // "prefabs/example_prefab.xml"
		if (!result)
		{
			return false;
		}

		for (pugi::xml_node entityNode : doc.child("Prefab").children("Entity"))
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

			u64 id = str_uuid_to_u64(uuid);
			flecs::entity ent = prefab.world.entity();
			
			PrefabUUIDComponent uuidC{};
			uuidC.uuid = id;
			ent.set(uuidC);

			for (pugi::xml_node childNode : entityNode.children())
			{
				if (entity_parse_component(childNode, ent))
				{
					HWARN("Parsed '{0}' component", childNode.name());
				}
				else
				{
					HWARN("Failed to parse '{0}' component", childNode.name());
				}
			}
		}

		return true;
	}
}