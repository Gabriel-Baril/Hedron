#include "hdef.h"

namespace hdn
{
	void HDefinition::Load(FBufferReader& archive, HObjectLoadFlags flags)
	{
		HObject::Load(archive, flags);
	}

	void HDefinition::Save(FBufferWriter& archive, HObjectSaveFlags flags)
	{
		HObject::Save(archive, flags);
	}
}
