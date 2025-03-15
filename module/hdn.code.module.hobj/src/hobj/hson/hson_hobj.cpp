#include "hson_hobj.h"

namespace hdn
{
	void HHson::Serialize(hostream& stream, HObjectSerializationFlags flags)
	{
		HObject::Serialize(stream);
		hson_serialize(stream, m_Hson);
	}

	void HHson::Deserialize(histream& stream, HObjectDeserializationFlags flags)
	{
		HObject::Deserialize(stream);
		hson_deserialize(stream, m_Hson);
	}
}
