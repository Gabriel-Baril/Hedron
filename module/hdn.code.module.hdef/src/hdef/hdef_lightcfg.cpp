#include "hdef_lightcfg.h"

namespace hdn
{
	void HLightConfig::Deserialize(FBufferReader& archive, HObjectLoadFlags flags)
	{
		HDefinition::Deserialize(archive, flags);
		bin::Read(archive, m_MaxPrimaryLightCount);
		bin::Read(archive, m_MaxSecondaryLightCount);
	}

	void HLightConfig::Serialize(FBufferWriter& archive, HObjectSaveFlags flags)
	{
		HDefinition::Serialize(archive, flags);
		bin::Write(archive, m_MaxPrimaryLightCount);
		bin::Write(archive, m_MaxSecondaryLightCount);
	}
}
