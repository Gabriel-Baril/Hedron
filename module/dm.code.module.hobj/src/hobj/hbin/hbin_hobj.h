#pragma once

#include "hobj/hobj.h"

namespace dm
{
	// hbin: Base class for all game artefacts
	class HBinaryArtefact : public HObject
	{
	public:
		HBinaryArtefact() = default;
		virtual void serialize(hostream &stream) override;
		virtual void deserialize(histream &stream) override;
		inline virtual h64 get_type_hash() const override { return hash_generate_from_type<HBinaryArtefact>(); }
		virtual ~HBinaryArtefact() = default;
	};
}
