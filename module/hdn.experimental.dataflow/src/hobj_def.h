#pragma once
#include "hobj.h"

namespace hdn
{
	class HDefinition : public HObject
	{
	public:
		HDefinition() = default;

		virtual void Load(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default) override
		{
			HObject::Load(archive, flags);
		}

		virtual void Save(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) const override
		{
			HObject::Save(archive, flags);
		}

		inline virtual u64 GetTypeHash() const override { return TYPE_HASH(HDefinition); }

		virtual ~HDefinition() = default;
	};
}