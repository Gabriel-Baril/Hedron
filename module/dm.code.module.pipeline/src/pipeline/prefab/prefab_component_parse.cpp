#include "prefab_component_parse.h"

#include "core/hash.h"

#include "prefab_component.h"

namespace dm
{
	bool component_parse_uuid(pugi::xml_node componentNode, flecs::entity ent);
	bool component_parse_transform(pugi::xml_node componentNode, flecs::entity ent);
	bool component_parse_model(pugi::xml_node componentNode, flecs::entity ent);
	bool component_parse_hierarchy(pugi::xml_node componentNode, flecs::entity ent);

	using ParseComponentFunc = bool (*)(pugi::xml_node, flecs::entity);
	static ParseComponentFunc s_ComponentFuncs[underlying(PrefabComponentType::COUNT)] = {
			component_parse_uuid,
			component_parse_transform,
			component_parse_model,
			component_parse_hierarchy};

	static constexpr const char *s_ComponentNames[underlying(PrefabComponentType::COUNT)] = {
			"UUIDComponent",
			"TransformComponent",
			"ModelComponent",
			"HierarchyComponent"};

	PrefabComponentType component_name_to_enum(const char *name)
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

	bool component_parse_uuid(pugi::xml_node componentNode, flecs::entity ent)
	{
		PrefabUUIDComponent component{};
		component.uuid = hash_generate(componentNode.attribute("uuid").as_string());
		ent.set(component);
		return true;
	}

	bool component_parse_transform(pugi::xml_node componentNode, flecs::entity ent)
	{
		PrefabTransformComponent component;
		component.position.x = componentNode.child("Translation").attribute("x").as_float();
		component.position.y = componentNode.child("Translation").attribute("y").as_float();
		component.position.z = componentNode.child("Translation").attribute("z").as_float();

		component.rotation.x = componentNode.child("Rotation").attribute("x").as_float();
		component.rotation.y = componentNode.child("Rotation").attribute("y").as_float();
		component.rotation.z = componentNode.child("Rotation").attribute("z").as_float();

		component.scale.x = componentNode.child("Scale").attribute("x").as_float();
		component.scale.y = componentNode.child("Scale").attribute("y").as_float();
		component.scale.z = componentNode.child("Scale").attribute("z").as_float();
		ent.set(component);

		return true;
	}

	bool component_parse_model(pugi::xml_node componentNode, flecs::entity ent)
	{
		PrefabModelComponent component{};
		ent.set(component);
		return true;
	}

	bool component_parse_hierarchy(pugi::xml_node componentNode, flecs::entity ent)
	{
		PrefabHierarchyComponent component{};
		ent.set(component);

		return true;
	}
}
