#include "hobj_zone_api.h"
#include "hobj_zone_serializer.h"
#include "hobj_zone_deserializer.h"

namespace hdn
{
    void HZone::Deserialize(FBufferReader& archive, HObjectDeserializationFlags flags)
    {
        ZoneDeserializer deserializer;
        deserializer.Deserialize(archive, m_Zone);
    }

    void HZone::Serialize(FBufferWriter& archive, HObjectSerializationFlags flags)
	{
		// ZoneSerializer serializer;
		// serializer.Serialize(archive);
    }

    HZone::~HZone()
    {
    }
}