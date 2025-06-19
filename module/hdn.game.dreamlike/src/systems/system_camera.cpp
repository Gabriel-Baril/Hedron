#include "system_camera.h"
#include "hobj/scene/transform_component.h"

#include "scene.h"
#include "application.h"

namespace hdn
{
	void CameraSystem::init()
	{
		Ref<RuntimeScene> scene = Application::get_scene();

		m_ViewerObject = scene->create_entity();
		TransformComponent transformC;
		transformC.position.z = -2.5f;
		m_ViewerObject.set(transformC);
	}

	void CameraSystem::update(f32 frameTime)
	{
		VulkanWindow& window = Application::get().get_window();
		VulkanRenderer& renderer = Application::get().get_renderer();

		cameraController.move_in_plane_xyz(window.get_glfw_window(), frameTime, m_ViewerObject);
		TransformComponent* viewerObjectTransformC = m_ViewerObject.get_mut<TransformComponent>();
		camera.set_view_yxz(viewerObjectTransformC->position, viewerObjectTransformC->rotation);
		f32 aspect = renderer.get_aspect_ratio();
		camera.set_perspective_projection(glm::radians(50.0f), aspect, 0.01f, 1.0f);
	}
}
