#include "HDNModel.h"

#include "HDNUtils.h"

#include <tinyobjloader/tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <cassert>
#include <unordered_map>

namespace std
{
	template<>
	struct hash<hdn::HDNModel::Vertex>
	{
		size_t operator()(const hdn::HDNModel::Vertex& vertex) const
		{
			size_t seed = 0;
			hdn::HashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}

namespace hdn
{
	HDNModel::HDNModel(HDNDevice* device, const HDNModel::Builder& builder)
		: m_Device{device}
	{
		CreateVertexBuffers(builder.vertices);
		CreateIndexBuffers(builder.indices);
	}


	HDNModel::~HDNModel()
	{
	}

	Scope<HDNModel> HDNModel::CreateModelFromFile(HDNDevice* device, const std::string& filepath)
	{
		Builder builder{};
		builder.LoadModel(filepath);
		// HDN_CORE_INFO("Vertex Count: {0}", builder.vertices.size());
		return CreateScope<HDNModel>(device, builder);
	}

	void HDNModel::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_VertexBuffer->GetBuffer()};
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		if (m_HasIndexBuffer)
		{
			vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32); // The index type could be smaller based on the model
		}
	}

	void HDNModel::Draw(VkCommandBuffer commandBuffer)
	{
		if (m_HasIndexBuffer)
		{
			vkCmdDrawIndexed(commandBuffer, m_IndexCount, 1, 0, 0, 0);
		}
		else
		{
			vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
		}
	}

	void HDNModel::CreateVertexBuffers(const std::vector<Vertex>& vertices)
	{
		m_VertexCount = static_cast<u32>(vertices.size());
		assert(m_VertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;

		const u32 vertexSize = sizeof(vertices[0]);
		HDNBuffer stagingBuffer{
			m_Device,
			vertexSize,
			m_VertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)vertices.data());

		m_VertexBuffer = CreateScope<HDNBuffer>(
			m_Device,
			vertexSize,
			m_VertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		m_Device->CopyBuffer(stagingBuffer.GetBuffer(), m_VertexBuffer->GetBuffer(), bufferSize);
	}

	void HDNModel::CreateIndexBuffers(const std::vector<u32>& indices)
	{
		m_IndexCount = static_cast<u32>(indices.size());
		m_HasIndexBuffer = m_IndexCount > 0;
		if (!m_HasIndexBuffer)
		{
			return;
		}

		VkDeviceSize bufferSize = sizeof(indices[0]) * m_IndexCount;
		u32 indexSize = sizeof(indices[0]);

		HDNBuffer stagingBuffer{
			m_Device,
			indexSize,
			m_IndexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)indices.data());

		m_IndexBuffer = CreateScope<HDNBuffer>(
			m_Device,
			indexSize,
			m_IndexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		m_Device->CopyBuffer(stagingBuffer.GetBuffer(), m_IndexBuffer->GetBuffer(), bufferSize);
	}

	std::vector<VkVertexInputBindingDescription> HDNModel::Vertex::GetBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> HDNModel::Vertex::GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		attributeDescriptions.reserve(4);
		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
		attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
		attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
		attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT	  , offsetof(Vertex, uv) });


		return attributeDescriptions;
	}

	void HDNModel::Builder::LoadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib; // Store positions, colors, uvs
		std::vector<tinyobj::shape_t> shapes; // Index values for each elements
		std::vector<tinyobj::material_t> materials;
		std::string warns, errors;
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warns, &errors, filepath.c_str()))
		{
			throw std::runtime_error(warns + errors);
		}

		vertices.clear();
		indices.clear();

		std::unordered_map<Vertex, u32> uniqueVertices;

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices) // Loop in each face element in the model
			{
				Vertex vertex{};
				
				if (index.vertex_index >= 0)
				{
					vertex.position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

					vertex.color = {
						attrib.colors[3 * index.vertex_index + 0],
						attrib.colors[3 * index.vertex_index + 1],
						attrib.colors[3 * index.vertex_index + 2]
					};
				}

				if (index.normal_index >= 0)
				{
					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}

				if (index.texcoord_index >= 0)
				{
					vertex.uv = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				if (!uniqueVertices.contains(vertex))
				{
					uniqueVertices[vertex] = static_cast<u32>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
}
