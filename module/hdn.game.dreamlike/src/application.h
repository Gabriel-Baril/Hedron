
#pragma once

#include "r_vk_device.h"
#include "r_vk_window.h"
#include "gameobject.h"
#include "r_vk_renderer.h"
#include "r_vk_descriptors.h"

#include "physics/physics_world.h"
#include "flecs/flecs.h"

#include "core/application/application.h"

namespace hdn
{
	class Application : public IApplication
	{
	public:
		static constexpr u32 WIDTH = 800;
		static constexpr u32 HEIGHT = 600;
	public:
		Application();
		virtual ~Application();
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		void run();
	private:
		void load_game_objects();
	private:
		VulkanWindow m_Window{ WIDTH, HEIGHT, "First App"};
		VulkanDevice m_Device{ m_Window };
		VulkanRenderer m_Renderer{ &m_Window, &m_Device };

		// Order of declarations matters
		Scope<VulkanDescriptorPool> m_GlobalPool{}; // System that require descriptors only relevant to their work should create their own HDNDescriptorPool

		PhysicsWorld m_PhysicsWorld;

		flecs::world m_EcsWorld;
	};
}