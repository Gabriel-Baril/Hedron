#pragma once

#include "hobj/hdef/hdef_hobj.h"
#include "prefab.h"

namespace hdn
{
	class HPrefab : public HObject
	{
	public:
		virtual ~HPrefab() = default;

		virtual void serialize(hostream& stream) override;
		virtual void deserialize(histream& stream) override;

		Prefab& get_prefab() { return m_Prefab; };
		const Prefab& get_prefab() const { return m_Prefab; };
		inline virtual h64 get_type_hash() const override { return hash_generate_from_type<HPrefab>(); }
	private:
		Prefab m_Prefab;
	};
}