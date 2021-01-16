#pragma once
#include <Hedron.h>
#include "Panels/SceneHierarchyPanel.h"

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
		SceneHierarchyPanel m_hierarchyPanel;
		
		Entity m_squareEntity;
		
		Entity m_mainCameraEntity;
		Entity m_originCameraEntity;
		bool m_mainCamera;
		bool m_viewportFocused = false, m_viewportHovered = false;

		glm::vec2 m_viewPortSize = { 0.0f, 0.0f };
		glm::vec4 m_squareColor;
		OrthographicCameraController m_cameraController;
	};
}

