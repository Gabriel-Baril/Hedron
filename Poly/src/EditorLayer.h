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

		glm::vec2 m_viewPortSize = { 0.0f, 0.0f };
		glm::vec4 m_backgroundColor;
		OrthographicCameraController m_cameraController;
	};
}
