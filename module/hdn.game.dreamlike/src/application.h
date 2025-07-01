#pragma once

#include "gfx/r_vk_device.h"
#include "gfx/r_vk_window.h"
#include "gameobject.h"
#include "gfx/r_vk_renderer.h"
#include "gfx/r_vk_descriptors.h"

#include "scene.h"

#include "core/application/application.h"

#include "core/event/events.h"
#include "core/application/system_registry.h"

namespace hdn
{
	struct ApplicationRenderSystem
	{
		ApplicationRenderSystem()
		{

		}

		void init()
		{
			
		}
	};

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

		VulkanWindow& get_window() { return *m_Renderer->get_window(); }
		VulkanDevice& get_device() { return *m_Renderer->get_device(); }
		VulkanRenderer& get_renderer() { return *m_Renderer; }

		void on_event(Event& event) override;
		bool on_window_resized(WindowResizedEvent& event);
		bool on_key_pressed(KeyPressedEvent& event);
		bool on_mouse_button_pressed(MouseButtonPressedEvent& event);

		Ref<RuntimeScene> create_scene();

		void run();
	private:
		void frame();
	private:
		Ref<VulkanRenderer> m_Renderer;

		Scope<VulkanDescriptorSetLayout> m_GlobalSetLayout;
		vector<Scope<VulkanBuffer>> m_UboBuffers{ VulkanSwapChain::MAX_FRAMES_IN_FLIGHT };

		// Order of declarations matters
		Scope<VulkanDescriptorPool> m_GlobalPool{}; // System that require descriptors only relevant to their work should create their own HDNDescriptorPool
		vector<VkDescriptorSet> m_GlobalDescriptorSets{ VulkanSwapChain::MAX_FRAMES_IN_FLIGHT };

#if USING(HDN_DEBUG)
		Scope<VulkanDescriptorPool> m_ImguiDescriptorPool;
		QueueFamilyIndices m_QueueFamilyIndices;
#endif

		Ref<RuntimeScene> m_Scene;

		SystemRegistry m_SystemRegistry;
	};
}