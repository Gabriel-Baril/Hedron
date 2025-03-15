#pragma once

#include "hobj/hobj.h"

namespace hdn
{
	// hgen: Base class for all generated hobj meant for development (for example, cache entry, cache register, etc)
	class HGeneratedObject : public HObject
	{
	public:
		HGeneratedObject() = default;
		virtual void Serialize(hostream& stream, HObjectSerializationFlags flags = HObjectSerializationFlags::Default) override;
		virtual void Deserialize(histream& stream, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default) override;
		inline virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HGeneratedObject>(); }
		virtual ~HGeneratedObject() = default;
	};
}
