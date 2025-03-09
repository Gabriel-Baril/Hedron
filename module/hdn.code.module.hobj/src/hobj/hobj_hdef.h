#pragma once

#include "hobj.h"

namespace hdn
{
	class HDefinition : public HObject
	{
	public:
		HDefinition() = default;
		virtual void Serialize(hostream& stream, HObjectSerializationFlags flags = HObjectSerializationFlags::Default) override;
		virtual void Deserialize(histream& stream, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default) override;
		inline virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HDefinition>(); }
		virtual ~HDefinition() = default;
	};
}