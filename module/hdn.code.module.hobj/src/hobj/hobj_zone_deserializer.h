#pragma once

#include "core/io/buffer_reader.h"
#include "hobj_zone.h"

namespace hdn
{
	class ZoneDeserializer
	{
	public:
		void Deserialize(FBufferReader& archive, Zone& zone);
	};
}