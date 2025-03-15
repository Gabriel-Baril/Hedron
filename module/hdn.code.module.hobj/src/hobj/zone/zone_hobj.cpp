#include "zone_hobj.h"

namespace hdn
{
	void HZone::Serialize(hostream& stream, HObjectSerializationFlags flags)
	{
		HObject::Serialize(stream);
        Zone_Serialize(stream, m_Zone);
	}

    void HZone::Deserialize(histream& stream, HObjectDeserializationFlags flags)
    {
		HObject::Deserialize(stream);
		Zone_Deserialize(stream, m_Zone);
    }

    HZone::~HZone()
    {
    }
}