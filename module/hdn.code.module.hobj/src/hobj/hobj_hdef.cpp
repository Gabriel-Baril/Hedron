#include "hdef.h"

namespace hdn
{
	void HDefinition::Deserialize(FBufferReader& archive, HObjectDeserializationFlags flags)
	{
		HObject::Deserialize(archive, flags);
	}

	void HDefinition::Serialize(FBufferWriter& archive, HObjectSerializationFlags flags)
	{
		HObject::Serialize(archive, flags);
	}
}
