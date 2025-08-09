#pragma once

#include "hobj/hdef/hdef_hobj.h"
#include "scene.h"

namespace hdn
{
	class HScene : public HObject
	{
	public:
		virtual ~HScene() = default;

		virtual void serialize(hostream& stream) override;
		virtual void deserialize(histream& stream) override;

		inline virtual h64 get_type_hash() const override { return hash_generate_from_type<HScene>(); }

		Scene& get_scene() { return m_Scene; };
		const Scene& get_scene() const { return m_Scene; };
	private:
		Scene m_Scene;
	};
}