#include "scene_def.h"

namespace hdn
{
	void scene_def_set_max_light(SceneDef& object, int maxLight)
	{
		object.maxLight = maxLight;
	}

	void scene_def_set_max_primary_light(SceneDef& object, int maxPrimaryLight)
	{
		object.maxPrimaryLight = maxPrimaryLight;
	}

	void scene_def_emplace_entity(SceneDef& object, const SceneEntityDef& entity)
	{
		object.entities.emplace_back(entity);
	}

	void scene_def_serialize(hostream& stream, const SceneDef& object)
	{
		// hson builder;
		// builder["max_light"] = object->max_light;
		// builder["max_primary_light"] = object->max_primary_light;
		// builder["entity_count"] = object.entities.size();
		// for (int i = 0; i < object.entities.size(); i++)
		// {
		// 	hsubobj_map_entity_serialize_hson(builder["entities"], object.entities.at(i));
		// }
		// stream << builder;
	}

	void scene_def_deserialize(histream& stream, SceneDef& object)
	{
		// hson h;
		// stream >> h;
		// object.max_light = h["max_light"].as<int>();
		// object.max_primary_light = h["max_primary_light"].as<int>();
		// object.entities.reserve(h["entity_count"].as<size_t>());
		// for (int i = 0; i < h["entity_count"].as<size_t>(); i++)
		// {
		// 	hsubobj_map_entity_deserialize_hson(h["entiites"], object.entities.at(i))
		// }
	}
}