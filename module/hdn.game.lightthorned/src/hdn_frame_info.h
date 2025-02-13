#pragma once

#include "hdn_camera.h"
#include "hdn_game_object.h"

#include "vulkan/vulkan.h"

namespace hdn
{
	#define MAX_LIGHTS 10

	struct PointLight
	{
		vec4f32 position{}; // ignore w
		vec4f32 color{}; // w is intensity
	};

	// Global Uniform Buffer Object
	struct GlobalUbo
	{
		mat4f32 projection{ 1.0f };
		mat4f32 view{ 1.0f };
		mat4f32 inverseView{ 1.0f };
		vec4f32 ambientLightColor{ 1.0f, 1.0f, 1.0f, 0.02f };
		PointLight pointLights[MAX_LIGHTS];
		i32 numLights;
	};

	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		HDNCamera* camera;
		VkDescriptorSet globalDescriptorSet;
		flecs::world* ecsWorld;
	};
}