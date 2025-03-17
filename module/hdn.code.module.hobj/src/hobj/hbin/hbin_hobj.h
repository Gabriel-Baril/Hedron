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
		inline virtual hash64_t GetTypeHash() const override { return GenerateTypeHash<HBinaryArtefact>(); }
		virtual ~HBinaryArtefact() = default;
	};
}
