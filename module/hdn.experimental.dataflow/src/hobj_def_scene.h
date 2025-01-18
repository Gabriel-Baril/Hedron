#pragma once
#include "hobj_def.h"
#include "hobj_def_lightcfg.h"
#include "hobj_util.h"

namespace hdn
{
	class HScene : public HDefinition
	{
	public:
		HScene() = default;

		virtual void Load(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default) override
		{
			HDefinition::Load(archive, flags);

			HObjectKey lightObjectKey;
			bin::Read(archive, lightObjectKey);
			m_LightConfig = HObjectUtil::GetObjectFromKey<HLightConfig>(lightObjectKey, flags);
		}

		virtual void Save(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) const override
		{
			HDefinition::Save(archive, flags);
			bin::Write(archive, m_LightConfig->GetKey());
		}

		virtual u64 GetTypeHash() const override { return TYPE_HASH(HScene); }

		void SetLightConfig(HObjPtr<HLightConfig> lightConfig) { m_LightConfig = lightConfig; }
		HObjPtr<HLightConfig> GetLightConfig() const { return m_LightConfig; }

		virtual ~HScene() = default;
	private:
		HObjPtr<HLightConfig> m_LightConfig = nullptr;
	};
}
