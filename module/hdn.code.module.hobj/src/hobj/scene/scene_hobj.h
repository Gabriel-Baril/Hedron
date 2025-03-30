#pragma once

#include "hobj/hdef/hdef_hobj.h"
#include "scene.h"

namespace hdn
{
	class HSceneDef : public HDefinition
	{
	public:
		virtual ~HSceneDef() = default;

		virtual void serialize(hostream& stream) override;
		virtual void deserialize(histream& stream) override;

		SceneDef& get_scene_definition() { return m_MapDefinition; };
		const SceneDef& get_scene_definition() const { return m_MapDefinition; };
		inline virtual h64 get_type_hash() const override { return hash_generate_from_type<HSceneDef>(); }
	private:
		SceneDef m_MapDefinition;
	};
}