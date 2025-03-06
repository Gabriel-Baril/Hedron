#pragma once

#include "hobj.h"

namespace hdn
{
	class HDefinition : public HObject
	{
	public:
		HDefinition() = default;
		
		virtual void Serialize(hostream& stream, HObjectSerializationFlags flags = HObjectSerializationFlags::Default) override
		{
			HObject::Serialize(stream);
		}

		virtual void Deserialize(histream& stream, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default) override
		{
			HObject::Deserialize(stream);
		}

		inline virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HDefinition>(); }
		virtual ~HDefinition() = default;
	};
}