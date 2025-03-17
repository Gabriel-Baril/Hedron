#include "hson_hobj.h"

namespace hdn
{
	void HHson::Serialize(hostream& stream)
	{
		HObject::Serialize(stream);
		hson_serialize(stream, m_Hson);
	}

	void HHson::Deserialize(histream& stream)
	{
		HObject::Deserialize(stream);
		hson_deserialize(stream, m_Hson);
	}
}
