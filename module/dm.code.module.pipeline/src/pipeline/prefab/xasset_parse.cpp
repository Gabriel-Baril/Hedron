#include "xasset_parse.h"

#include "flecs/flecs.h"

#include "core/hash.h"

#include "pipeline/xsrc.h"
#include "prefab_component.h"
#include "prefab_component_parse.h"

namespace dm
{
	bool prefab_parse(SPrefabDef &def, const pugi::xml_node &node)
	{
		const pugi::char_t *nameStr = node.attribute("name").as_string();
		DM_CORE_ASSERT(nameStr, "xsymbol require a name");
		def.name = get_symbol_from_name(nameStr);

		for (pugi::xml_node entityNode : node.children("Entity"))
		{
			const pugi::char_t *uuid = entityNode.attribute("uuid").as_string();
			DM_CORE_ASSERT(uuid, "Entity without uuid found");

			const pugi::char_t *name = entityNode.attribute("name").as_string();
			if (name)
			{
				DM_WARNING_LOG("Entity: '{0}' ({1})", name, uuid);
			}
			else
			{
				DM_WARNING_LOG("Entity: '<unnamed>' ({0})", uuid);
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
					DM_WARNING_LOG("Parsed '{0}' component", childNode.name());
				}
				else
				{
					DM_WARNING_LOG("Failed to parse '{0}' component", childNode.name());
				}
			}
		}

		return true;
	}

	bool xasset_parse_prefab(const pugi::xml_node &symbolNode, const SourceContext &ctx)
	{
		SPrefabDef def;
		bool success = prefab_parse(def, symbolNode);

		return success;
	}
}
