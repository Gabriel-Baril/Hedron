#include "prefab_component_hierarchy.h"

namespace hdn
{
	bool component_parse_hierarchy(pugi::xml_node componentNode, flecs::entity ent)
	{
		PrefabHierarchyComponent hierarchyC{};
		ent.set(hierarchyC);

		return true;
	}
}