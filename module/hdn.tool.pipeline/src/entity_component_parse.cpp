#include "entity_component_parse.h"

#include "entity_component_transform.h"
#include "entity_component_model.h"
#include "entity_component_hierarchy.h"

namespace hdn
{
	using ParseComponentFunc = bool(*)(pugi::xml_node, EntityDef&);
	static ParseComponentFunc s_ComponentFuncs[underlying(EntityComponentType::COUNT)] = {
		component_parse_transform,
		component_parse_model,
		component_parse_hierarchy
	};

	static constexpr const char* s_ComponentNames[underlying(EntityComponentType::COUNT)] = {
		"TransformComponent",
		"ModelComponent",
		"HierarchyComponent"
	};

	EntityComponentType component_name_to_enum(const char* name)
	{
		for (int i = 0; i < underlying(EntityComponentType::COUNT); i++)
		{
			if (str_equals(name, s_ComponentNames[i]))
			{
				return static_cast<EntityComponentType>(i);
			}
		}
		return EntityComponentType::UNKNOWN;
	}

	bool entity_parse_component(pugi::xml_node componentNode, EntityDef& entDef)
	{
		EntityComponentType componentType = component_name_to_enum(componentNode.name());
		if (componentType != EntityComponentType::UNKNOWN)
		{
			return s_ComponentFuncs[underlying(componentType)](componentNode, entDef);
		}
		return false;
	}
}