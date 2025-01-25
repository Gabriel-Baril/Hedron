#include "hdef_scene.h"

namespace hdn
{
	void HScene::Deserialize(FBufferReader& archive, HObjectLoadFlags flags)
	{
		HDefinition::Deserialize(archive, flags);
		hkey lightObjectKey;
		bin::Read(archive, lightObjectKey);
		m_LightConfig = HObjectUtil::GetObjectFromKey<HLightConfig>(lightObjectKey, flags);
	}

	void HScene::Serialize(FBufferWriter& archive, HObjectSaveFlags flags)
	{
		HDefinition::Serialize(archive, flags);
		bin::Write(archive, m_LightConfig->GetKey());
	}
}
