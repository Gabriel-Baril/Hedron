#pragma once

#include "hdn_device.h"
#include "hdn_buffer.h"

#include "core/core.h"
#include "core/stl/vector.h"

#include <memory>
#include <vector>

namespace hdn
{
	class HDNModel
	{
	public:
		struct Vertex
		{
			vec3f32 position{};
			vec3f32 color{};
			vec3f32 normal{};
			vec2f32 uv{};
		
			static vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

			bool operator==(const Vertex& other) const
			{
				return
					position == other.position &&
					color == other.color &&
					normal == other.normal &&
					uv == other.uv;
			}
		};

		struct Builder
		{
			vector<Vertex> vertices{};
			vector<u32> indices{};

			void LoadObjModel(const std::string& filepath);
			void LoadFbxModel(const std::string& filepath);
		};

		HDNModel(HDNDevice* device, const HDNModel::Builder& builder);
		virtual ~HDNModel();
		HDNModel(const HDNModel&) = delete;
		HDNModel& operator=(const HDNModel&) = delete;

		static Scope<HDNModel> CreateModelFromObjFile(HDNDevice* device, const std::string& filepath);
		static Scope<HDNModel> CreateModelFromFbxFile(HDNDevice* device, const std::string& filepath);

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);
	private:
		void CreateVertexBuffers(const vector<Vertex>& vertices);
		void CreateIndexBuffers(const vector<u32>& indices);
	private:
		HDNDevice* m_Device;
		
		// The buffer and it's assigned memory are 2 different objects
		Scope<HDNBuffer> m_VertexBuffer;
		u32 m_VertexCount;

		bool m_HasIndexBuffer = false;
		Scope<HDNBuffer> m_IndexBuffer;
		u32 m_IndexCount;
	};
}