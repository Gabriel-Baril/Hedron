#pragma once

#include "hobj/hdef/hdef_hobj.h"
#include "scene.h"

namespace hdn
{
	class HSceneDef : public HDefinition
	{
	public:
		virtual ~HSceneDef() = default;

		virtual void Serialize(hostream& stream) override
		{
			HDefinition::Serialize(stream);
			SceneDef_Serialize(stream, m_MapDefinition);
		}

		virtual void Deserialize(histream& stream) override
		{
			HDefinition::Deserialize(stream);
			SceneDef_Deserialize(stream, m_MapDefinition);
		}

		SceneDef& GetSceneDefinition() { return m_MapDefinition; };
		const SceneDef& GetSceneDefinition() const { return m_MapDefinition; };
		inline virtual hash64_t GetTypeHash() const { return GenerateTypeHash<HSceneDef>(); }
	private:
		SceneDef m_MapDefinition;
	};
}