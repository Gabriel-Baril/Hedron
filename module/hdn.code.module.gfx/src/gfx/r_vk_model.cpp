#include "r_vk_model.h"

#include "core/stl/unordered_map.h"

#include "core/hash.h"

#include <tinyobjloader/tiny_obj_loader.h>

#include <ofbx.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std
{
	template<>
	struct hash<hdn::VulkanModel::Vertex>
	{
		size_t operator()(const hdn::VulkanModel::Vertex& vertex) const
		{
			size_t seed = 0;
			hdn::hash_merge(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}

namespace eastl {
	template <>
	struct hash<hdn::VulkanModel::Vertex> {
		size_t operator()(const hdn::VulkanModel::Vertex& vertex) const noexcept {
			return std::hash<hdn::VulkanModel::Vertex>()(vertex);
		}
	};
}

namespace hdn
{
	VulkanModel::VulkanModel(VulkanDevice* device, const VulkanModel::Builder& builder)
		: m_Device{device}
	{
		create_vertex_buffers(builder.vertices);
		create_index_buffers(builder.indices);
	}


	VulkanModel::~VulkanModel()
	{
	}

	Scope<VulkanModel> VulkanModel::create_model_from_obj_file(VulkanDevice* device, const string& filepath)
	{
		Builder builder{};
		builder.load_obj_model(filepath);
		return make_scope<VulkanModel>(device, builder);
	}

	Scope<VulkanModel> VulkanModel::create_model_from_fbx_file(VulkanDevice* device, const string& filepath)
	{
		Builder builder{};
		builder.load_fbx_model(filepath);
		return make_scope<VulkanModel>(device, builder);
	}

	void VulkanModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_VertexBuffer->get_buffer()};
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		if (m_HasIndexBuffer)
		{
			vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->get_buffer(), 0, VK_INDEX_TYPE_UINT32); // The index type could be smaller based on the model
		}
	}

	void VulkanModel::draw(VkCommandBuffer commandBuffer)
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

	void VulkanModel::create_vertex_buffers(const vector<Vertex>& vertices)
	{
		m_VertexCount = static_cast<u32>(vertices.size());
		assert(m_VertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;

		const u32 vertexSize = sizeof(vertices[0]);
		VulkanBuffer stagingBuffer{
			m_Device,
			vertexSize,
			m_VertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.map();
		stagingBuffer.write_to_buffer((void*)vertices.data());

		m_VertexBuffer = make_scope<VulkanBuffer>(
			m_Device,
			vertexSize,
			m_VertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		m_Device->copy_buffer(stagingBuffer.get_buffer(), m_VertexBuffer->get_buffer(), bufferSize);
	}

	void VulkanModel::create_index_buffers(const vector<u32>& indices)
	{
		m_IndexCount = static_cast<u32>(indices.size());
		m_HasIndexBuffer = m_IndexCount > 0;
		if (!m_HasIndexBuffer)
		{
			return;
		}

		VkDeviceSize bufferSize = sizeof(indices[0]) * m_IndexCount;
		u32 indexSize = sizeof(indices[0]);

		VulkanBuffer stagingBuffer{
			m_Device,
			indexSize,
			m_IndexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.map();
		stagingBuffer.write_to_buffer((void*)indices.data());

		m_IndexBuffer = make_scope<VulkanBuffer>(
			m_Device,
			indexSize,
			m_IndexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		m_Device->copy_buffer(stagingBuffer.get_buffer(), m_IndexBuffer->get_buffer(), bufferSize);
	}

	vector<VkVertexInputBindingDescription> VulkanModel::Vertex::get_binding_descriptions()
	{
		vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	vector<VkVertexInputAttributeDescription> VulkanModel::Vertex::get_attribute_descriptions()
	{
		vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		attributeDescriptions.reserve(4);
		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
		attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
		attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
		attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT	  , offsetof(Vertex, uv) });


		return attributeDescriptions;
	}

	void VulkanModel::Builder::load_obj_model(const string& filepath)
	{
		tinyobj::attrib_t attrib; // Store positions, colors, uvs
		std::vector<tinyobj::shape_t> shapes; // Index values for each elements
		std::vector<tinyobj::material_t> materials;
		string warns, errors;
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warns, &errors, filepath.c_str()))
		{
			HTHROW(std::runtime_error, warns + errors);
		}

		vertices.clear();
		indices.clear();

		unordered_map<Vertex, u32> uniqueVertices;

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

	void VulkanModel::Builder::load_fbx_model(const string& filepath)
	{
		std::ifstream file(filepath.c_str(), std::ios::binary | std::ios::ate);
		if (!file)
		{
			HTHROW_FMT(std::runtime_error, "Failed to open file '{0}'", filepath.c_str());
		}

		size_t fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		vector<u8> buffer(fileSize);
		if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize))
		{
			HTHROW_FMT(std::runtime_error, "Failed to read file '{0}'", filepath.c_str());
		}

		vertices.clear();
		indices.clear();

		unordered_map<Vertex, u32> uniqueVertices;
		const ofbx::IScene* scene = ofbx::load(buffer.data(), fileSize, static_cast<ofbx::u16>(ofbx::LoadFlags::NONE));
		HDEBUG("Mesh Count: {0}", scene->getMeshCount());
		for (int i = 0;i < scene->getMeshCount(); i++)
		{
			const ofbx::Mesh* fbxMesh = scene->getMesh(i);
			const ofbx::Geometry* geom = fbxMesh->getGeometry();
			if (!geom)
			{
				continue;
			}

			const ofbx::Vec3Attributes positions = geom->getGeometryData().getPositions();
			const ofbx::Vec3Attributes normals = geom->getGeometryData().getNormals();
			const ofbx::Vec2Attributes uvs = geom->getGeometryData().getUVs();
			const ofbx::Vec4Attributes colors = geom->getGeometryData().getColors();

			HDEBUG("Position Count: {0}, {1}", positions.count, positions.values_count);
			HDEBUG("Normal Count: {0}, {1}", normals.count, normals.values_count);
			HDEBUG("UV Count: {0}", uvs.count);
			HDEBUG("Color Count: {0}", colors.count);

			for (int j = 0; j < positions.count; j++) // positions.count is the amount of indices
			{
				Vertex vertex{};

				ofbx::Vec3 position = positions.get(j);
				vertex.position = {
					position.x,
					position.y,
					position.z
				};

				ofbx::Vec3 normal = normals.get(j);
				vertex.normal = {
					normal.x,
					normal.y,
					normal.z
				};

				ofbx::Vec2 uv = uvs.get(j);
				vertex.uv = {
					uv.x,
					uv.y
				};

				vertex.color = {
					1.0f,
					1.0f,
					1.0f
				};

				if (!uniqueVertices.contains(vertex))
				{
					uniqueVertices[vertex] = static_cast<u32>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}

			size_t meshMemorySavedFromVertexDedupByte = (positions.count - vertices.size()) * sizeof(Vertex) - indices.size() * sizeof(u32);
			HDEBUG("Vertex Count After Deduplication: {0}", vertices.size());
			HDEBUG("Mesh Memory Saved From Deduplication: {0} bytes", meshMemorySavedFromVertexDedupByte);
		}
	}
}
