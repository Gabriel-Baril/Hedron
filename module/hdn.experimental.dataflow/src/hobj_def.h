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

		virtual void Save(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) override
		{
			HObject::Save(archive, flags);
		}

		inline virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HDefinition>(); }

		virtual ~HDefinition() = default;
	};
}