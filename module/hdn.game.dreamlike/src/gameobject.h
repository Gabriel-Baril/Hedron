#pragma once

#include "core/core.h"
#include "flecs/flecs.h"

#include "ecs/components/native_script_component.h"

namespace hdn
{
	class HDNGameObject
	{
	public:
		using id_t = flecs::entity_t;

		static HDNGameObject CreateGameObject(flecs::world& ecs, const char* name = nullptr)
		{
			flecs::entity e = ecs.entity(name);
			return HDNGameObject{ e };
		}

		static HDNGameObject MakePointLight(flecs::world& ecs, f32 intensity = 10.0f, f32 radius = 0.1f, vec3f32 color = vec3f32{1.0f});
		
		template<typename T>
		const T* Get()
		{
			return m_Entity.get<T>();
		}

		template<typename T>
		T* GetMut()
		{
			return m_Entity.get_mut<T>();
		}

		template<typename T>
		void Set(const T& component)
		{
			m_Entity.set(component);
		}

		template<typename T>
		void AddNativeScript()
		{
			if (!m_Entity.has<NativeScriptComponent>())
			{
				m_Entity.add<NativeScriptComponent>();
			}
			INativeScript* script = new T;
			script->Bind(m_Entity);
			m_Entity.get_mut<NativeScriptComponent>()->Add(script);
		}

		flecs::entity GetEntity()
		{
			return m_Entity;
		}

		id_t GetID()
		{
			return m_Entity.id();
		}

		const char* Name()
		{
			return m_Entity.name();
		}
	private:
		HDNGameObject(flecs::entity e) : m_Entity{ e } {}
		flecs::entity m_Entity;
	};
}