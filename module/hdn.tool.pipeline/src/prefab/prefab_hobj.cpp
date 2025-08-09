#include "prefab_hobj.h"

namespace hdn
{
	void HPrefab::serialize(hostream& stream)
	{
		HObject::serialize(stream);
		prefab_serialize(stream, m_Prefab);
	}

	void HPrefab::deserialize(histream& stream)
	{
		HObject::deserialize(stream);
		prefab_deserialize(stream, m_Prefab);
	}
}