#include "prefab_parse.h"

#include "flecs/flecs.h"

#include "prefab_component_parse.h"
#include "prefab_component_uuid.h"

#include "hobj/utils.h"

namespace hdn
{
	bool prefab_parse(SPrefabDef& def, const pugi::xml_node& node)
	{
		const pugi::char_t* nameStr = node.attribute("name").as_string();
		HASSERT(nameStr, "xsymbol require a name");
		def.name = get_symbol_from_name(nameStr);

		for (pugi::xml_node entityNode : node.children("Entity"))
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
			flecs::entity ent = def.world.entity();
			
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

	bool prefab_parse_callback(const pugi::xml_node& symbolNode)
	{
		SPrefabDef def;
		bool success = prefab_parse(def, symbolNode);

		HINFO("xsymbol (prefab) '{0}' registered", def.name);
		// TODO: Register prefab to relevant pipeline systems
		return success;
	}
}