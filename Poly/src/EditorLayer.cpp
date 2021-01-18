#include "EditorLayer.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hedron/Scene/SceneSerializer.h"

#include "Hedron/Utils/PlatformUtils.h"

namespace Hedron
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer layer"),
		m_cameraController(1280.f / 720.f, true),
		m_squareColor(0.0f, 1.0f, 0.0f, 1.0f)
	{

	}

	void EditorLayer::on_attach()
	{
		HDR_PROFILE_FUNCTION();

		FrameBufferSpec frameBufferSpec;
		frameBufferSpec.width = Application::get().get_window().get_width();
		frameBufferSpec.height = Application::get().get_window().get_height();
		m_frameBuffer = FrameBuffer::create(frameBufferSpec);

		m_activeScene = create_ref<Scene>();
#if 0
		m_squareEntity = m_activeScene->create_entity("Square Entity");
		m_squareEntity.add_component<Hedron::SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 0.0f, 0.3f });

		for (uint32_t i = 0; i < 10; i++)
		{
			auto entity = m_activeScene->create_entity("Red Square Entity");
			auto& transformComponent = entity.get_component<TransformComponent>();
			
			transformComponent.scale.x = 0.8f;
			transformComponent.scale.y = 0.8f;
			transformComponent.translation.x = (float)i;
			entity.add_component<Hedron::SpriteRendererComponent>(glm::vec4{ i / 10.0f, i / 2.0f, i / 5.0f, 1.0f });
		}

		m_mainCameraEntity = m_activeScene->create_entity("Main camera");
		m_mainCameraEntity.add_component<Hedron::CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			void on_create()
			{
			}

			void on_destroy()
			{
			}

			void on_update(Timestep ts)
			{
				auto& transform = get_component<TransformComponent>();
				float cameraSpeed = 10.0f;

				if (Input::is_key_pressed(KeyCode::W))
					transform.translation.y += cameraSpeed * ts;
				else if (Input::is_key_pressed(KeyCode::S))
					transform.translation.y -= cameraSpeed * ts;

				if (Input::is_key_pressed(KeyCode::A))
					transform.translation.x -= cameraSpeed * ts;
				else if (Input::is_key_pressed(KeyCode::D))
					transform.translation.x += cameraSpeed * ts;
			}
		};

		m_mainCameraEntity.add_component<Hedron::NativeScriptComponent>().bind<CameraController>();

		m_originCameraEntity = m_activeScene->create_entity("Origin Camera");
		m_originCameraEntity.add_component<Hedron::CameraComponent>().primary = false;
#endif
		m_hierarchyPanel.set_context(m_activeScene);
	}

	void EditorLayer::on_detach()
	{
		HDR_PROFILE_FUNCTION();
	}

	void EditorLayer::on_update(Timestep ts)
	{
		HDR_PROFILE_FUNCTION();
		Renderer2D::reset_stats();
		m_frameBuffer->bind();
		RenderCommand::set_clear_color({0.2f, 0.2f, 0.2f, 1.0f});
		RenderCommand::clear();

		m_activeScene->on_update(ts); // Update scene

		m_frameBuffer->unbind();
	}

	void EditorLayer::on_imgui_render()
	{
		HDR_PROFILE_FUNCTION();

		auto stat = Renderer2D::get_stats();
		const auto& cam = m_cameraController.get_camera();
		auto cam_pos = cam.get_position();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					this->new_scene();
				}
				
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					this->open_scene();
				}
				
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					this->save_as_scene();
				}
				
				if (ImGui::MenuItem("Quit"))
					Application::get().close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_hierarchyPanel.on_imgui_render();

		ImGui::Begin("Stats");
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("	Draw Call(s) : %i", stat.drawCalls);
		ImGui::Text("	Quad Count : %i", stat.quadCount);
		ImGui::Text("	Vertex Count : %i", stat.get_total_vertex_count());
		ImGui::Text("	Index Count : %i", stat.get_total_index_count());
		ImGui::Text("	Triangle Count : %i", stat.get_total_triangle_count());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::get().get_imgui_layer()->block_events(!m_viewportFocused || !m_viewportHovered);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail(); // Get the size of our panel
		if (m_viewPortSize != *(glm::vec2*)&viewportPanelSize && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
		{
			m_frameBuffer->resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_cameraController.on_resize(viewportPanelSize.x, viewportPanelSize.y);
			m_viewPortSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_activeScene->on_viewport_resize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);
		}

		uint32_t textureID = m_frameBuffer->get_color_attachment_rendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_viewPortSize.x, m_viewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}

	void EditorLayer::on_event(Event& event)
	{
		HDR_PROFILE_FUNCTION();
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(HDR_BIND_EVENT_FN(EditorLayer::on_key_pressed));
	}

	bool EditorLayer::on_key_pressed(KeyPressedEvent& event)
	{
		// Shortcut System
		if (event.get_repeat_count() > 0)
			return false;

		bool controlPressed = Input::is_key_pressed(KeyCode::LEFT_CONTROL) || Input::is_key_pressed(KeyCode::RIGHT_CONTROL);
		bool shiftPressed = Input::is_key_pressed(KeyCode::LEFT_SHIFT) || Input::is_key_pressed(KeyCode::RIGHT_SHIFT);

		switch (event.get_key_code()) // TODO: Change get_key_code() to return a KeyCode
		{
			case (uint32_t)KeyCode::N: // TODO: Fix the cast
			{
				if (controlPressed) new_scene();
				break;
			}
			case (uint32_t)KeyCode::O: // TODO: Fix the cast
			{
				if (controlPressed) open_scene();
				break;
			}
			case (uint32_t)KeyCode::S: // TODO: Fix the cast
			{
				if (controlPressed && shiftPressed) save_as_scene();
				break;
			}
		}

		return false;
	}

	void EditorLayer::new_scene()
	{
		m_activeScene = create_ref<Scene>();
		m_activeScene->on_viewport_resize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);
		m_hierarchyPanel.set_context(m_activeScene);
	}

	void EditorLayer::open_scene()
	{
		std::string filePath = FileDialogs::open_file("Hazel Scene (*.hdr)\0*.hdr\0"); // "Hazel Scene (*.hdr)" is the string that appears inside the selection box
		if (!filePath.empty())
		{
			m_activeScene = create_ref<Scene>();
			m_activeScene->on_viewport_resize((uint32_t)m_viewPortSize.x, (uint32_t)m_viewPortSize.y);
			m_hierarchyPanel.set_context(m_activeScene);

			SceneSerializer serializer(m_activeScene);
			serializer.deserialize_yaml(filePath);
		}
	}

	void EditorLayer::save_as_scene()
	{
		std::string filePath = FileDialogs::save_file("Hazel Scene (*.hdr)\0*.hdr\0"); // "Hazel Scene (*.hdr)" is the string that appears inside the selection box
		if (!filePath.empty())
		{
			SceneSerializer serializer(m_activeScene);
			serializer.serialize_yaml(filePath);
		}
	}
}