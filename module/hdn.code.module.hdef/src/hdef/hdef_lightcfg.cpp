#include "hdef_lightcfg.h"

namespace hdn
{
	void HLightConfig::Load(FBufferReader& archive, HObjectLoadFlags flags)
	{
		HDefinition::Load(archive, flags);
		bin::Read(archive, m_MaxPrimaryLightCount);
		bin::Read(archive, m_MaxSecondaryLightCount);
	}

	void HLightConfig::Save(FBufferWriter& archive, HObjectSaveFlags flags)
	{
		HDefinition::Save(archive, flags);
		bin::Write(archive, m_MaxPrimaryLightCount);
		bin::Write(archive, m_MaxSecondaryLightCount);
	}
}
