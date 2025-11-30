#include "zone_hobj.h"

namespace dm
{
	void HZone::serialize(hostream &stream)
	{
		HObject::serialize(stream);
		zone_serialize(stream, m_Zone);
	}

	void HZone::deserialize(histream &stream)
	{
		HObject::deserialize(stream);
		zone_deserialize(stream, m_Zone);
	}

	HZone::~HZone()
	{
	}
}
