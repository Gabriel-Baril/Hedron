#include "scene_hobj.h"

namespace hdn
{
	void HScene::serialize(hostream& stream)
	{
		HObject::serialize(stream);
		scene_serialize(stream, m_Scene);
	}

	void HScene::deserialize(histream& stream)
	{
		HObject::deserialize(stream);
		scene_deserialize(stream, m_Scene);
	}
}
