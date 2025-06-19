
#pragma once

#include "r_vk_device.h"
#include "r_vk_window.h"
#include "gameobject.h"
#include "r_vk_renderer.h"
#include "r_vk_descriptors.h"

#include "scene.h"

#include "core/application/application.h"

#include "core/event/events.h"
#include "core/application/system_registry.h"

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

		static Application& get();

		template<typename T>
		static Ref<T> get_system(const char* name)
		{
			return get().m_SystemRegistry.get<T>(name);
		}

		static Ref<RuntimeScene> get_scene()
		{
			return get().m_Scene;
		}

		VulkanWindow& get_window() { return m_Window; }
		VulkanRenderer& get_renderer() { return m_Renderer; }
		VulkanDevice& get_device() { return m_Device; }

		void on_event(Event& event) override;
		bool on_window_resized(WindowResizedEvent& event);
		bool on_key_pressed(KeyPressedEvent& event);
		bool on_mouse_button_pressed(MouseButtonPressedEvent& event);

		void run();
	private:
		VulkanWindow m_Window{ WIDTH, HEIGHT, "First App"};
		VulkanDevice m_Device{ m_Window };
		VulkanRenderer m_Renderer{ &m_Window, &m_Device };
		Scope<VulkanDescriptorSetLayout> m_GlobalSetLayout;

		// Order of declarations matters
		Scope<VulkanDescriptorPool> m_GlobalPool{}; // System that require descriptors only relevant to their work should create their own HDNDescriptorPool
		vector<VkDescriptorSet> m_GlobalDescriptorSets{ VulkanSwapChain::MAX_FRAMES_IN_FLIGHT };

		Ref<RuntimeScene> m_Scene;

		SystemRegistry m_SystemRegistry;
	};
}