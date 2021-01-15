#pragma once

#include <glm/glm.hpp>
#include <functional>

#include "Hedron/Scene/ScriptableEntity.h"
#include "Hedron/Scene/SceneCamera.h"

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
		glm::mat4 transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: transform{ transform } {}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
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
		std::function<void()> instantiateFunction;
		std::function<void()> destroyInstanceFunction;

		std::function<void(ScriptableEntity*)> onCreateFunction;
		std::function<void(ScriptableEntity*)> onDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> onUpdateFunction;

		template<typename T>
		void bind()
		{
			instantiateFunction = [&]() { instance = new T(); };
			destroyInstanceFunction = [&] { delete (T*)instance; instance = nullptr; };

			onCreateFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->on_create(); };
			onDestroyFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->on_destroy(); };
			onUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { static_cast<T*>(instance)->on_update(ts); };
		}
	};
}