#pragma once
#include "core/hobj/hobj.h"

namespace hdn
{
	class HDefinition : public HObject
	{
	public:
		HDefinition() = default;
		virtual void Deserialize(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default) override;
		virtual void Serialize(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) override;
		inline virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HDefinition>(); }
		virtual ~HDefinition() = default;
	};
}