
#pragma once

#include "r_vk_device.h"
#include "r_vk_window.h"
#include "gameobject.h"
#include "r_vk_renderer.h"
#include "r_vk_descriptors.h"

#include "physics/physics_world.h"
#include "flecs/flecs.h"

#include "core/application/application.h"

#include "panel/editor_panel.h"

// ---
#include "keyboard_movement_controller.h"
#include "ecs/systems/simple_render_system.h"
#include "ecs/systems/point_light_system.h"
#include "ecs/systems/physics_gameobject_system.h"
#include "ecs/systems/update_transform_system.h"
#include "ecs/systems/update_script_system.h"
#include "r_vk_imgui.h"
// ---

#include "editor_camera.h"
#include "core/event/events.h"

namespace hdn
{
	class EditorApplication : public IApplication
	{
	public:
		static constexpr u32 DEFAULT_WINDOW_WIDTH = 1280;
		static constexpr u32 DEFAULT_WINDOW_HEIGHT = 720;
	public:
		static EditorApplication& Get();
		VulkanWindow& GetWindow() { return m_Window; }

		void OnEvent(Event& event) override;
		bool OnWindowResized(WindowResizedEvent& event);
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

		EditorApplication();
		virtual ~EditorApplication();
		EditorApplication(const EditorApplication&) = delete;
		EditorApplication& operator=(const EditorApplication&) = delete;

		template<typename T, typename... Args>
		T* Register(Args&&... args)
		{
			m_Panels.push_back(CreateRef<T>(std::forward<Args>(args)...));
			return static_cast<T*>(m_Panels.back().get());
		}

		template<typename T>
		void Unregister()
		{
			// TODO: Unregister layer of the same type (The layer type is defined by the class type)
		}

		void Unregister(int id)
		{
			// TODO: Unregister layer by it's id
		}
		void Run();
	private:
		void LoadGameObjects();
	private:
		VulkanWindow m_Window{ DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "First App" };
		VulkanDevice m_Device{ m_Window };
		VulkanRenderer m_Renderer{ &m_Window, &m_Device };

		// Order of declarations matters
		Scope<VulkanDescriptorPool> m_GlobalPool{}; // System that require descriptors only relevant to their work should create their own HDNDescriptorPool

		PhysicsWorld m_PhysicsWorld;

		flecs::world m_EcsWorld;

		vector<Ref<IEditorPanel>> m_Panels;

		// ------------
		vector<Scope<VulkanBuffer>> m_UboBuffers{ VulkanSwapChain::MAX_FRAMES_IN_FLIGHT };
		Scope<VulkanDescriptorSetLayout> m_GlobalSetLayout;
		vector<VkDescriptorSet> m_GlobalDescriptorSets{ VulkanSwapChain::MAX_FRAMES_IN_FLIGHT }; // One descriptor set per frame

		UpdateTransformSystem m_UpdateTransformSystem{};
		UpdateScriptSystem m_UpdateScriptSystem{};
		SimpleRenderSystem m_SimpleRenderSystem{};
		PointLightSystem m_PointLightSystem{};
		PhysicsGameObjectSystem m_PhysicsGameObjectSystem{};

		EditorCamera m_EditorCamera{};

		ImguiSystem m_ImguiSystem;
		Scope<VulkanDescriptorPool> m_ImguiDescriptorPool;
		QueueFamilyIndices m_QueueFamilyIndices;
	};
}