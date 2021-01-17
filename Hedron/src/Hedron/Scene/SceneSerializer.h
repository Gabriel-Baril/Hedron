#pragma once

#include "Scene.h"

namespace Hedron
{
	// Light stack-constructible class
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void serialize_yaml(const std::string& filePath);
		void serialize_binary(const std::string& filePath);
		
		bool deserialize_yaml(const std::string& filePath);
		bool deserialize_binary(const std::string& filePath);
	private:
		Ref<Scene> m_scene;
	};
}