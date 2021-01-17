#pragma once

#include <entt.hpp>

#include "Hedron/Core/Core.h"
#include "Hedron/Scene/Scene.h"

namespace Hedron
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename Component>
		bool has_component()
		{
			return m_scene->m_registry.has<Component>(m_entityHandle);
		}

		template<typename Component>
		Component& get_component()
		{
			HDR_CORE_ASSERT(has_component<Component>(), "Entity does not have component!");
			return m_scene->m_registry.get<Component>(m_entityHandle);
		}

		template<typename Component, typename... Args>
		Component& add_component(Args&&... args)
		{
			HDR_CORE_ASSERT(!has_component<Component>(), "Entity already has component!");
			Component& component = m_scene->m_registry.emplace<Component>(m_entityHandle, std::forward<Args>(args)...);
			m_scene->on_component_added<Component>(*this, component);
			return component;
		}

		template<typename Component, typename... Args>
		void remove_component()
		{
			HDR_CORE_ASSERT(has_component<Component>(), "Entity does not have component!");
			m_scene->m_registry.remove<Component>(m_entityHandle);
		}

		bool operator==(const Entity& other) const
		{
			return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !operator==(other);
		}

		operator bool() const { return m_entityHandle != entt::null; }
		operator uint32_t() const { return static_cast<uint32_t>(m_entityHandle); }
		operator entt::entity() const { return m_entityHandle; }
	private:
		entt::entity m_entityHandle{ entt::null }; // id
		Scene* m_scene = nullptr;
	};
}