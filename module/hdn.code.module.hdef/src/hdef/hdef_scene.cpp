#include "hdef_scene.h"

namespace hdn
{
	void HScene::Load(FBufferReader& archive, HObjectLoadFlags flags)
	{
		HDefinition::Load(archive, flags);
		hkey lightObjectKey;
		bin::Read(archive, lightObjectKey);
		m_LightConfig = HObjectUtil::GetObjectFromKey<HLightConfig>(lightObjectKey, flags);
	}

	void HScene::Save(FBufferWriter& archive, HObjectSaveFlags flags)
	{
		HDefinition::Save(archive, flags);
		bin::Write(archive, m_LightConfig->GetKey());
	}
}
