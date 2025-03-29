#pragma once

#include "hobj/hobj.h"

namespace hdn
{
	// hgen: Base class for all generated hobj meant for development (for example, cache entry, cache register, etc)
	class HGeneratedObject : public HObject
	{
	public:
		HGeneratedObject() = default;
		virtual void Serialize(hostream& stream) override;
		virtual void Deserialize(histream& stream) override;
		inline virtual h64 GetTypeHash() const override { return hash_generate_from_type<HGeneratedObject>(); }
		virtual ~HGeneratedObject() = default;
	};
}
