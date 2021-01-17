#include "hdrpch.h"
#include "Hedron/Scene/SceneSerializer.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

#include "Hedron/Scene/Components.h"
#include "Hedron/Scene/SceneCamera.h"
#include "Hedron/Scene/Entity.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
		}
	};
}

namespace Hedron
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.y << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_scene(scene)
	{
	}
	
	static void serialize_entity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;
		
		if (entity.has_component<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.get_component<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}
		
		if (entity.has_component<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& transformComponent = entity.get_component<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << transformComponent.translation;
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent.rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComponent.scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.has_component<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.get_component<CameraComponent>();
			auto& camera = cameraComponent.camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType"   << YAML::Value << (uint32_t)camera.get_projection_type();
			out << YAML::Key << "PerspectiveFOV"   << YAML::Value << camera.get_perspective_vertical_fov();
			out << YAML::Key << "PerspectiveNear"  << YAML::Value << camera.get_perspective_near_clip();
			out << YAML::Key << "PerspectiveFar"   << YAML::Value << camera.get_perspective_far_clip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.get_orthographic_size();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.get_orthographic_near_clip();
			out << YAML::Key << "OrthographicFar"  << YAML::Value << camera.get_orthographic_far_clip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.has_component<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& spriteRendererComponent = entity.get_component<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;

			out << YAML::EndMap; // CameraComponent
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::serialize_yaml(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Untitled";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		m_scene->m_registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_scene.get() };
			if (!entity)
				return;
			serialize_entity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}
	
	void SceneSerializer::serialize_binary(const std::string& filePath)
	{
		HDR_CORE_ASSERT(false, "Not Implemented");
	}
	
	bool SceneSerializer::deserialize_yaml(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str()); // Root node
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		HDR_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO: Unique idenfier

				std::string name;
				auto tagComponentNode = entity["TagComponent"];
				if (tagComponentNode)
					name = tagComponentNode["Tag"].as<std::string>();

				HDR_CORE_TRACE("Deserialized entity with ID: {0}, Name: {1}", uuid, name);
				
				Entity deserializedEntity = m_scene->create_entity(name);

				auto transformComponentNode = entity["TransformComponent"];
				if (transformComponentNode)
				{
					// TODO: Create add_or_get_component function for Entity
					auto& transformComponent = deserializedEntity.get_component<TransformComponent>();
					
					transformComponent.translation = transformComponentNode["Translation"].as<glm::vec3>();
					transformComponent.rotation = transformComponentNode["Rotation"].as<glm::vec3>();
					transformComponent.scale = transformComponentNode["Scale"].as<glm::vec3>();
				}
				
				auto cameraComponentNode = entity["CameraComponent"];
				if (cameraComponentNode)
				{
					auto& cameraComponent = deserializedEntity.add_component<CameraComponent>();
					auto& cameraProps = cameraComponentNode["Camera"];

					cameraComponent.camera.set_projection_type(cameraProps["ProjectionType"].as<int>());
						
					cameraComponent.camera.set_perspective_vertical_fov(cameraProps["PerspectiveFOV"].as<float>());
					cameraComponent.camera.set_perspective_near_clip(cameraProps["PerspectiveNear"].as<float>());
					cameraComponent.camera.set_perspective_far_clip(cameraProps["PerspectiveFar"].as<float>());

					cameraComponent.camera.set_orthographic_size(cameraProps["OrthographicSize"].as<float>());
					cameraComponent.camera.set_orthographic_near_clip(cameraProps["OrthographicNear"].as<float>());
					cameraComponent.camera.set_orthographic_far_clip(cameraProps["OrthographicFar"].as<float>());
				
					cameraComponent.primary = cameraComponentNode["Primary"].as<bool>();
					cameraComponent.fixedAspectRatio = cameraComponentNode["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponentNode = entity["SpriteRendererComponent"];
				if (spriteRendererComponentNode)
				{
					auto& spriteRendererComponent = deserializedEntity.add_component<SpriteRendererComponent>();
					spriteRendererComponent.color = spriteRendererComponentNode["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}
	
	bool SceneSerializer::deserialize_binary(const std::string& filePath)
	{
		HDR_CORE_ASSERT(false, "Not Implemented");
		return false;
	}
}