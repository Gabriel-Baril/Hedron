#pragma once

#include "hobj/hobj.h"

namespace hdn
{
	// hdef are component of information meant to be serialized within an hobj which is designed to be exposed to developer and content creator. This is essentially the uasset equivalent for hedron.
	class HDefinition : public HObject
	{
	public:
		HDefinition() = default;
		virtual void Serialize(hostream& stream) override;
		virtual void Deserialize(histream& stream) override;
		inline virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HDefinition>(); }
		virtual ~HDefinition() = default;
	};
}