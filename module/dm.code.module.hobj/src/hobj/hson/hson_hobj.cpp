#include "hson_hobj.h"

namespace hdn
{
	void HHson::serialize(hostream& stream)
	{
		HObject::serialize(stream);
		hson_serialize(stream, m_Hson);
	}

	void HHson::deserialize(histream& stream)
	{
		HObject::deserialize(stream);
		hson_deserialize(stream, m_Hson);
	}
}
