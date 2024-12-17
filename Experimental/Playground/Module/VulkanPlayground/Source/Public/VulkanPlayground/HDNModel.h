#pragma once

#include "HDNDevice.h"

#include "Core/Core.h"

#include <vector>

namespace hdn
{
	class HDNModel
	{
	public:
		struct Vertex
		{
			vec3f32 position;
			vec3f32 color;
		
			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32> indices{};
		};

		HDNModel(HDNDevice* device, const HDNModel::Builder& builder);
		virtual ~HDNModel();
		HDNModel(const HDNModel&) = delete;
		HDNModel& operator=(const HDNModel&) = delete;
		HDNModel(HDNModel&&) = delete;
		HDNModel& operator=(HDNModel&&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);
	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);
		void CreateIndexBuffers(const std::vector<uint32>& indices);
	private:
		HDNDevice* m_Device;
		
		// The buffer and it's assigned memory are 2 different objects
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		uint32 m_VertexCount;

		bool m_HasIndexBuffer = false;
		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_IndexBufferMemory;
		uint32 m_IndexCount;
	};
}