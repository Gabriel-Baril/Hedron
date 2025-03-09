#include "hobj_zone.h"

namespace hdn
{
	void HZone::Serialize(hostream& stream, HObjectSerializationFlags flags)
	{
		HObject::Serialize(stream);
        hobj_zone_serialize(stream, m_Zone);
	}

    void HZone::Deserialize(histream& stream, HObjectDeserializationFlags flags)
    {
		HObject::Deserialize(stream);
		hobj_zone_deserialize(stream, m_Zone);
    }

    HZone::~HZone()
    {
    }
}