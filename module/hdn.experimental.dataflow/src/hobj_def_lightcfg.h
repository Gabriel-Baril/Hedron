#pragma once
#include "hobj_def.h"

namespace hdn
{
	class HLightConfig : public HDefinition
	{
	public:
		HLightConfig() = default;

		virtual void Load(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default) override
		{
			HDefinition::Load(archive, flags);
			bin::Read(archive, m_MaxPrimaryLightCount);
			bin::Read(archive, m_MaxSecondaryLightCount);
		}

		virtual void Save(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) const override
		{
			HDefinition::Save(archive, flags);
			bin::Write(archive, m_MaxPrimaryLightCount);
			bin::Write(archive, m_MaxSecondaryLightCount);
		}

		virtual u64 GetTypeHash() const override { return TYPE_HASH(HLightConfig); }

		inline u32 GetMaxPrimaryLightCount() const { return m_MaxPrimaryLightCount; }
		inline void SetMaxPrimaryLightCount(u32 count) { m_MaxPrimaryLightCount = count; }
		inline u32 GetMaxSecondaryLightCount() const { return m_MaxSecondaryLightCount; }
		inline void SetMaxSecondaryLightCount(u32 count) { m_MaxSecondaryLightCount = count; }

		virtual ~HLightConfig() = default;
	private:
		u32 m_MaxPrimaryLightCount = 0;
		u32 m_MaxSecondaryLightCount = 0;
	};
}