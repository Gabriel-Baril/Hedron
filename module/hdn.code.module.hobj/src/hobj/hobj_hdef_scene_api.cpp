#include "hobj_hdef_scene_api.h"

namespace hdn
{
	void hobj_hdef_scene_set_max_light(hobj_hdef_scene& object, int max_light)
	{
		object.max_light = max_light;
	}

	void hobj_hdef_scene_set_max_primary_light(hobj_hdef_scene& object, int max_primary_light)
	{
		object.max_primary_light = max_primary_light;
	}

	void hobj_hdef_scene_emplace_entity(hobj_hdef_scene& object, const hobjsub_map_entity& entity)
	{
		object.entities.emplace_back(entity);
	}

	void hobj_hdef_scene_serialize(hostream& stream, const hobj_hdef_scene& object)
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

	void hobj_hdef_scene_deserialize(histream& stream, hobj_hdef_scene& object)
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