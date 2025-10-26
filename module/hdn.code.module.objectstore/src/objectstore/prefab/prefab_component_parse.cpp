#include "prefab_component_parse.h"
#include "prefab_component_transform.h"
#include "prefab_component_model.h"
#include "prefab_component_hierarchy.h"

namespace hdn
{
	using ParseComponentFunc = bool(*)(pugi::xml_node, flecs::entity);
	static ParseComponentFunc s_ComponentFuncs[underlying(PrefabComponentType::COUNT)] = {
		nullptr,
		component_parse_transform,
		component_parse_model,
		component_parse_hierarchy
	};

	static constexpr const char* s_ComponentNames[underlying(PrefabComponentType::COUNT)] = {
		"UUIDComponent",
		"TransformComponent",
		"ModelComponent",
		"HierarchyComponent"
	};

	PrefabComponentType component_name_to_enum(const char* name)
	{
		for (int i = 0; i < underlying(PrefabComponentType::COUNT); i++)
		{
			if (str_equals(name, s_ComponentNames[i]))
			{
				return static_cast<PrefabComponentType>(i);
			}
		}
		return PrefabComponentType::UNKNOWN;
	}

	bool entity_parse_component(pugi::xml_node componentNode, flecs::entity ent)
	{
		PrefabComponentType componentType = component_name_to_enum(componentNode.name());
		if (componentType != PrefabComponentType::UNKNOWN)
		{
			auto func = s_ComponentFuncs[underlying(componentType)];
			if (func)
			{
				return func(componentNode, ent);
			}
		}
		return false;
	}
}