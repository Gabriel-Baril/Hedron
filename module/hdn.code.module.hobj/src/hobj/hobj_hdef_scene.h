#pragma once

#include "hobj_hdef.h"
#include "hobj_hdef_scene_api.h"

namespace hdn
{
	class HSceneDefinition : public HDefinition
	{
	public:
		virtual ~HSceneDefinition() = default;

		virtual void Serialize(hostream& stream, HObjectSerializationFlags flags = HObjectSerializationFlags::Default) override
		{
			HDefinition::Serialize(stream);
			hobj_hdef_scene_serialize(stream, m_MapDefinition);
		}

		virtual void Deserialize(histream& stream, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default) override
		{
			HDefinition::Deserialize(stream);
			hobj_hdef_scene_deserialize(stream, m_MapDefinition);
		}

		hobj_hdef_scene& GetSceneDefinition() { return m_MapDefinition; };
		const hobj_hdef_scene& GetSceneDefinition() const { return m_MapDefinition; };
		inline virtual hash64_t GetTypeHash() const { return GenerateTypeHash<HSceneDefinition>(); }
	private:
		hobj_hdef_scene m_MapDefinition;
	};
}