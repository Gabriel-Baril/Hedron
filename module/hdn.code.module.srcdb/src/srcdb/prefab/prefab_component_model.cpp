#include "prefab_component_model.h"

namespace hdn
{
	bool component_parse_model(pugi::xml_node componentNode, flecs::entity ent)
	{
		PrefabModelComponent modelC{};
		ent.set(modelC);
		return true;
	}
}