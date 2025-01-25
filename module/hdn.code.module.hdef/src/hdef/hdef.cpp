#include "hdef.h"

namespace hdn
{
	void HDefinition::Deserialize(FBufferReader& archive, HObjectLoadFlags flags)
	{
		HObject::Deserialize(archive, flags);
	}

	void HDefinition::Serialize(FBufferWriter& archive, HObjectSaveFlags flags)
	{
		HObject::Serialize(archive, flags);
	}
}
