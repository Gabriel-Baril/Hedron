#pragma once

#include "r_vk_device.h"
#include "r_vk_buffer.h"

#include "core/core.h"
#include "core/stl/vector.h"

namespace hdn
{
	class VulkanModel
	{
	public:
		struct Vertex
		{
			vec3f32 position{};
			vec3f32 color{};
			vec3f32 normal{};
			vec2f32 uv{};
		
			static vector<VkVertexInputBindingDescription> get_binding_descriptions();
			static vector<VkVertexInputAttributeDescription> get_attribute_descriptions();

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

			void load_obj_model(const string& filepath);
			void load_fbx_model(const string& filepath);
		};

		VulkanModel(VulkanDevice* device, const VulkanModel::Builder& builder);
		virtual ~VulkanModel();
		VulkanModel(const VulkanModel&) = delete;
		VulkanModel& operator=(const VulkanModel&) = delete;

		static Scope<VulkanModel> create_model_from_obj_file(VulkanDevice* device, const string& filepath);
		static Scope<VulkanModel> create_model_from_fbx_file(VulkanDevice* device, const string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void create_vertex_buffers(const vector<Vertex>& vertices);
		void create_index_buffers(const vector<u32>& indices);
	private:
		VulkanDevice* m_Device;
		
		// The buffer and it's assigned memory are 2 different objects
		Scope<VulkanBuffer> m_VertexBuffer;
		u32 m_VertexCount;

		bool m_HasIndexBuffer = false;
		Scope<VulkanBuffer> m_IndexBuffer;
		u32 m_IndexCount;
	};
}