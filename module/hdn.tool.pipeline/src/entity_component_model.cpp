#include "entity_component_model.h"

namespace hdn
{
	bool component_parse_model(pugi::xml_node componentNode, EntityDef& entDef)
	{
		ModelComponent modelC{};
		entDef.add_component(modelC);
		return true;
	}
}