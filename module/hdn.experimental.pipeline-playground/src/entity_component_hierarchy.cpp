#include "entity_component_hierarchy.h"

namespace hdn
{
	bool component_parse_hierarchy(pugi::xml_node componentNode, EntityDef& entDef)
	{
		HierarchyComponent hierarchyC{};
		entDef.add_component(hierarchyC);

		return true;
	}
}