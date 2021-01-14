#pragma once
#include <Hedron.h>

namespace Hedron
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void on_update(Timestep ts) override;
		virtual void on_imgui_render() override;
		virtual void on_event(Event& event) override;
	private:
		Ref<FrameBuffer> m_frameBuffer;

		Ref<Scene> m_activeScene;
		entt::entity m_square;

		bool m_viewportFocused = false, m_viewportHovered = false;
		
		glm::vec2 m_viewPortSize = { 0.0f, 0.0f };
		glm::vec4 m_squareColor;
		OrthographicCameraController m_cameraController;
	};
}

