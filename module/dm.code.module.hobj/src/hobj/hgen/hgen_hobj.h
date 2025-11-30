#pragma once

#include "hobj/hobj.h"

namespace dm
{
	// hgen: Base class for all generated hobj meant for development (for example, cache entry, cache register, etc)
	class HGeneratedObject : public HObject
	{
	public:
		HGeneratedObject() = default;
		virtual void serialize(hostream &stream) override;
		virtual void deserialize(histream &stream) override;
		inline virtual h64 get_type_hash() const override { return hash_generate_from_type<HGeneratedObject>(); }
		virtual ~HGeneratedObject() = default;
	};
}
