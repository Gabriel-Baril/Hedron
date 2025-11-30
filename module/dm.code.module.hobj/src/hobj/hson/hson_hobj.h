#pragma once

#include "hson.h"
#include "hson_build.h"
#include "hobj/hobj.h"

namespace hdn
{
	// Standalone serializable hson
	class HHson : public HObject
	{
	public:
		HHson() = default;
		virtual void serialize(hostream& stream) override;
		virtual void deserialize(histream& stream) override;
		inline virtual h64 get_type_hash() const override { return hash_generate_from_type<HHson>(); }
		virtual ~HHson() = default;

		Hson& get_hson() { return m_Hson; };
		const Hson& get_hson() const { return m_Hson; };
	private:
		Hson m_Hson;
	};
}