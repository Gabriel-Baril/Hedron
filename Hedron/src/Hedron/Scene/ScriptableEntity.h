#pragma once
#include "Hedron/Scene/Entity.h"

namespace Hedron
{

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename Component>
		Component& get_component()
		{
			return m_entity.get_component<Component>();
		}
	protected:
		virtual void on_create() {}
		virtual void on_destroy() {}
		virtual void on_update(Timestep ts) {}
	private:
		Entity m_entity;
		friend class Scene;
	};
}