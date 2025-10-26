#pragma once

#include "core/core.h"
#include "flecs/flecs.h"

#include "ecs/components/native_script_component.h"

namespace hdn
{
	class Entity
	{
	public:
		using id_t = flecs::entity_t;

		static Entity make_point_light(flecs::world& ecs, f32 intensity = 10.0f, f32 radius = 0.1f, vec3f32 color = vec3f32{1.0f});
		
		template<typename T>
		const T* get()
		{
			return m_Entity.get<T>();
		}

		template<typename T>
		T* get_mut()
		{
			return m_Entity.get_mut<T>();
		}

		template<typename T>
		void set(const T& component)
		{
			m_Entity.set(component);
		}

		template<typename T>
		void add_native_script()
		{
			if (!m_Entity.has<NativeScriptComponent>())
			{
				m_Entity.add<NativeScriptComponent>();
			}
			INativeScript* script = new T;
			script->bind(m_Entity);
			m_Entity.get_mut<NativeScriptComponent>()->add(script);
		}

		flecs::entity get_entity()
		{
			return m_Entity;
		}

		id_t get_id()
		{
			return m_Entity.id();
		}

		const char* name()
		{
			return m_Entity.name();
		}

		Entity() : m_Entity{ flecs::entity::null() } {}
		Entity(flecs::entity e) : m_Entity{ e } {}
	private:
		flecs::entity m_Entity;
	};
}