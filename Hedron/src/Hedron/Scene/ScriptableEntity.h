#pragma once
#include "Hedron/Scene/Entity.h"

namespace Hedron
{

	class ScriptableEntity
	{
	public:
	
		template<typename Component>
		Component& get_component()
		{
			return m_entity.get_component<Component>();
		}
	private:
		Entity m_entity;
		friend class Scene;
	};
}