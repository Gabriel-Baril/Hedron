#pragma once

#include "core/core.h"
#include "flecs/flecs.h"

#include "ecs/components/native_script_component.h"

namespace dm
{
	// Abstraction layer over a flecs::entity
	class Entity
	{
	public:
		using id_t = flecs::entity_t;

		template <typename T>
		const T *Get()
		{
			return m_Entity.get<T>();
		}

		template <typename T>
		T *GetMut()
		{
			return m_Entity.get_mut<T>();
		}

		template <typename T>
		void Set(const T &component)
		{
			m_Entity.set(component);
		}

		template <typename T>
		void AddNativeScript()
		{
			if (!m_Entity.has<NativeScriptComponent>())
			{
				m_Entity.add<NativeScriptComponent>();
			}
			INativeScript *script = new T;
			script->bind(m_Entity);
			m_Entity.get_mut<NativeScriptComponent>()->add(script);
		}

		flecs::entity GetEntity()
		{
			return m_Entity;
		}

		id_t GetID()
		{
			return m_Entity.id();
		}

		const char *Name()
		{
			return m_Entity.name();
		}
		Entity() : m_Entity{flecs::entity::null()} {}
		Entity(flecs::entity e) : m_Entity{e} {}

	private:
		flecs::entity m_Entity;
	};
}
