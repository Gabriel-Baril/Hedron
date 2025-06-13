#include "scene_def_hobj.h"

namespace hdn
{
	void HSceneDef::serialize(hostream& stream)
	{
		HDefinition::serialize(stream);
		scene_def_serialize(stream, m_SceneDefinition);
	}

	void HSceneDef::deserialize(histream& stream)
	{
		HDefinition::deserialize(stream);
		scene_def_deserialize(stream, m_SceneDefinition);
	}
}