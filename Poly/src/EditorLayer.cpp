#include "EditorLayer.h"
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

namespace Hedron
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer layer"),
		m_cameraController(1280.f / 720.f, true),
		m_backgroundColor(0.2f, 0.2f, 0.2f, 1.0f)
	{

	}

	void EditorLayer::on_attach()
	{
		HDR_PROFILE_FUNCTION();

		FrameBufferSpec frameBufferSpec;
		frameBufferSpec.width = Application::get().get_window().get_width();
		frameBufferSpec.height = Application::get().get_window().get_height();
		m_frameBuffer = FrameBuffer::create(frameBufferSpec);
	}

	void EditorLayer::on_detach()
	{
		HDR_PROFILE_FUNCTION();
	}

	void EditorLayer::on_update(Timestep ts)
	{
		HDR_PROFILE_FUNCTION();

		Renderer2D::reset_stats();
		m_cameraController.on_update(ts);
		//HDR_INFO("Delta time: [{0} sec] [{1} ms] [{2} fps]", ts.get_seconds(), ts.get_milliseconds(), 1000 / ts.get_milliseconds());

		// Camera position
		//HDR_INFO("[{0}, {1}, {2}]", cam.x, cam.y, cam.z);

		m_frameBuffer->bind();
		RenderCommand::set_clear_color(m_backgroundColor);
		RenderCommand::clear();

		Renderer2D::begin_scene(m_cameraController.get_camera());

		Renderer2D::fill_rect({ -0.5f, -0.5f, 0.0f }, {1.0f, 1.0f});

		Renderer2D::end_scene();
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
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::MenuItem("Quit"))
					Application::get().close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Background color control panel");
		ImGui::SliderFloat("Red background color", &m_backgroundColor.r, 0.0f, 1.0f);
		ImGui::SliderFloat("Greed background color", &m_backgroundColor.g, 0.0f, 1.0f);
		ImGui::SliderFloat("Blue background color", &m_backgroundColor.b, 0.0f, 1.0f);

		ImGui::End();
			
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail(); // Get the size of our panel
		if (m_viewPortSize != *(glm::vec2*)&viewportPanelSize)
		{
			m_frameBuffer->resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_cameraController.on_resize(viewportPanelSize.x, viewportPanelSize.y);
			m_viewPortSize = { viewportPanelSize.x, viewportPanelSize.y };
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
		m_cameraController.on_event(event);
	}

}