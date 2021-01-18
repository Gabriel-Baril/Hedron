#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Hedron/Scene/ScriptableEntity.h"
#include "Hedron/Scene/SceneCamera.h"

// Components are supposed to contain data only
namespace Hedron
{
	struct TagComponent
	{
		std::string tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };



		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: translation{ translation } {}

		glm::mat4 get_transform() const
		{
			glm::mat4 rotationMatrix = glm::toMat4(glm::quat(rotation));
				//glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 }) *
				//glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 }) *
				//glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), translation) * 
				rotationMatrix *
				glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color{ color } {}
	};

	struct CameraComponent
	{
		Hedron::SceneCamera camera;
		bool primary = true; // TODO: Think about moving to scene 
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// Script in C++
	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*instantiateScript)();
		void (*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind()
		{
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* script) { delete script->instance; script->instance = nullptr; };
		}
	};
}