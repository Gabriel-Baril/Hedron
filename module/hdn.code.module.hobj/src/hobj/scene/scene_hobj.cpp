#include "scene_hobj.h"

namespace hdn
{
	void HSceneDef::serialize(hostream& stream)
	{
		HDefinition::serialize(stream);
		scene_def_serialize(stream, m_MapDefinition);
	}

	void HSceneDef::deserialize(histream& stream)
	{
		HDefinition::deserialize(stream);
		scene_def_deserialize(stream, m_MapDefinition);
	}
}