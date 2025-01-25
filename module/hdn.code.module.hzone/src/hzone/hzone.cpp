#include "hzone.h"
#include "zone_serializer.h"
#include "zone_deserializer.h"

namespace hdn
{
    void HZone::Deserialize(FBufferReader& archive, HObjectLoadFlags flags)
    {
        ZoneDeserializer deserializer;
        deserializer.Deserialize(archive, m_Zone);
    }

    void HZone::Serialize(FBufferWriter& archive, HObjectSaveFlags flags)
	{
		// ZoneSerializer serializer;
		// serializer.Serialize(archive);
    }

    HZone::~HZone()
    {
    }
}