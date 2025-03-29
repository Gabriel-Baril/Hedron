#pragma once

#include "hobj/hobj.h"

namespace hdn
{
	// hbin: Base class for all game artefacts
	class HBinaryArtefact : public HObject
	{
	public:
		HBinaryArtefact() = default;
		virtual void Serialize(hostream& stream) override;
		virtual void Deserialize(histream& stream) override;
		inline virtual h64 GetTypeHash() const override { return hash_generate_from_type<HBinaryArtefact>(); }
		virtual ~HBinaryArtefact() = default;
	};
}
