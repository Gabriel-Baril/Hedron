#include "system_point_light.h"

#include "core/core.h"
#include "core/stl/map.h"

#include "hobj/scene/transform_component.h"
#include "ecs/components/color_component.h"
#include "ecs/components/point_light_component.h"

#include <glm/gtc/constants.hpp>
#include "core/utils.h"

#include "application.h"
#include "system_camera.h"
#include "system_point_light_render.h"

namespace hdn
{
	PointLightSystem::PointLightSystem(VulkanDevice* device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	{
		system_point_light_init(renderPass, globalSetLayout, device);
	}

	PointLightSystem::~PointLightSystem()
	{
		system_point_light_shutdown(); // TODO: Fine for now, but should ensure that nothing else is using the system point light system
	}

	void PointLightSystem::update(FrameInfo& frameInfo, flecs::world world)
	{
		auto rotateLight = glm::rotate(mat4f32(1.0f), frameInfo.frameTime, { 0.0f, -1.0f, 0.0f });

		int lightIndex = 0;
		auto query = world.query<TransformComponent, ColorComponent, PointLightComponent>();
		query.each([&](flecs::entity e, TransformComponent& transformC, ColorComponent& colorC, PointLightComponent& pointLightC) {
			assert(lightIndex < MAX_LIGHTS && "Point Light exceed maximum specified");
			transformC.position = vec3f32(rotateLight * vec4f32(transformC.position, 1.0f));

			// copy light to ubo
			frameInfo.ubo->pointLights[lightIndex].position = vec4f32(transformC.position, 1.0f);
			frameInfo.ubo->pointLights[lightIndex].color = vec4f32(colorC.color, pointLightC.lightIntensity);
			lightIndex += 1;
		});
		frameInfo.ubo->numLights = lightIndex;

		Ref<CameraSystem> cameraSystem = Application::get_system<CameraSystem>(NAME_CAMERA_SYSTEM);
		system_point_light_fetch(world, cameraSystem->get_camera_position());
		system_point_light_queue();
	}

	void PointLightSystem::render(FrameInfo& frameInfo, flecs::world world)
	{
		system_point_light_render(frameInfo.commandBuffer, frameInfo.globalDescriptorSet);
	}
}
