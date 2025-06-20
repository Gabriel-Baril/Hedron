#pragma once

#include "r_vk_device.h"

#include "core/stl/vector.h"
#include "core/stl/unordered_map.h"

namespace hdn {

	class VulkanDescriptorSetLayout {
	public:
		class Builder {
		public:
			Builder(VulkanDevice& device) : m_Device{ device } {}

			Builder& add_binding(
				uint32_t binding,
				VkDescriptorType descriptorType,
				VkShaderStageFlags stageFlags,
				uint32_t count = 1);
			Scope<VulkanDescriptorSetLayout> build() const;

		private:
			VulkanDevice& m_Device;
			unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
		};

		VulkanDescriptorSetLayout(
			VulkanDevice& device, unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		~VulkanDescriptorSetLayout();
		VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;

		VkDescriptorSetLayout get_descriptor_set_layout() const { return m_DescriptorSetLayout; }

	private:
		VulkanDevice& m_Device;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;

		friend class VulkanDescriptorWriter;
	};

	class VulkanDescriptorPool {
	public:
		class Builder {
		public:
			Builder(VulkanDevice& device) : m_Device{ device } {}

			Builder& add_pool_size(VkDescriptorType descriptorType, uint32_t count);
			Builder& set_pool_flags(VkDescriptorPoolCreateFlags flags);
			Builder& set_max_sets(uint32_t count);
			Scope<VulkanDescriptorPool> build() const;

		private:
			VulkanDevice& m_Device;
			vector<VkDescriptorPoolSize> m_PoolSizes{};
			uint32_t m_MaxSets = 1000;
			VkDescriptorPoolCreateFlags m_PoolFlags = 0;
		};

		VulkanDescriptorPool(
			VulkanDevice& device,
			uint32_t maxSets,
			VkDescriptorPoolCreateFlags poolFlags,
			const vector<VkDescriptorPoolSize>& poolSizes);
		~VulkanDescriptorPool();
		VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
		VulkanDescriptorPool& operator=(const VulkanDescriptorPool&) = delete;

		bool allocate_descriptor(
			const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

		void free_descriptors(vector<VkDescriptorSet>& descriptors) const;

		void reset_pool();

		VkDescriptorPool get_descriptor() const { return m_DescriptorPool; }

	private:
		VulkanDevice& m_Device;
		VkDescriptorPool m_DescriptorPool;

		friend class VulkanDescriptorWriter;
	};

	class VulkanDescriptorWriter {
	public:
		VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool);

		VulkanDescriptorWriter& write_buffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		VulkanDescriptorWriter& write_image(uint32_t binding, VkDescriptorImageInfo* imageInfo);

		bool build(VkDescriptorSet& set);
		void overwrite(VkDescriptorSet& set);

	private:
		VulkanDescriptorSetLayout& m_SetLayout;
		VulkanDescriptorPool& m_Pool;
		vector<VkWriteDescriptorSet> m_Writes;
	};
}