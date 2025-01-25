#pragma once

#include "core/hobj/hobj_util.h"

#include "hdef/hdef.h"
#include "hdef/hdef_lightcfg.h"

namespace hdn
{
	class HScene : public HDefinition
	{
	public:
		HScene() = default;
		virtual void Deserialize(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default) override;
		virtual void Serialize(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) override;
		virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HScene>(); }
		void SetLightConfig(HObjPtr<HLightConfig> lightConfig) { m_LightConfig = lightConfig; }
		HObjPtr<HLightConfig> GetLightConfig() const { return m_LightConfig; }
		virtual ~HScene() = default;
	private:
		HObjPtr<HLightConfig> m_LightConfig = nullptr;
	};
}
