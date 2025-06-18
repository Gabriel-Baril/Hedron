#include "r_vk_descriptors.h"

namespace hdn {

	// *************** Descriptor Set Layout Builder *********************

	VulkanDescriptorSetLayout::Builder& VulkanDescriptorSetLayout::Builder::add_binding(
		uint32_t binding,
		VkDescriptorType descriptorType,
		VkShaderStageFlags stageFlags,
		uint32_t count) {
		assert(m_Bindings.count(binding) == 0 && "Binding already in use");
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descriptorType;
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags = stageFlags;
		m_Bindings[binding] = layoutBinding;
		return *this;
	}

	Scope<VulkanDescriptorSetLayout> VulkanDescriptorSetLayout::Builder::build() const {
		return make_scope<VulkanDescriptorSetLayout>(m_Device, m_Bindings);
	}

	// *************** Descriptor Set Layout *********************

	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(
		VulkanDevice& device, unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
		: m_Device{ device }, m_Bindings{ bindings } {
		vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
		for (auto kv : bindings) {
			setLayoutBindings.push_back(kv.second);
		}

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
		descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

		if (vkCreateDescriptorSetLayout(
			m_Device.get_device(),
			&descriptorSetLayoutInfo,
			nullptr,
			&m_DescriptorSetLayout) != VK_SUCCESS) {
			HTHROW(std::runtime_error, "failed to create descriptor set layout!");
		}
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() {
		vkDestroyDescriptorSetLayout(m_Device.get_device(), m_DescriptorSetLayout, nullptr);
	}

	// *************** Descriptor Pool Builder *********************

	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::add_pool_size(
		VkDescriptorType descriptorType, uint32_t count) {
		m_PoolSizes.push_back({ descriptorType, count });
		return *this;
	}

	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::set_pool_flags(
		VkDescriptorPoolCreateFlags flags) {
		m_PoolFlags = flags;
		return *this;
	}
	VulkanDescriptorPool::Builder& VulkanDescriptorPool::Builder::set_max_sets(uint32_t count) {
		m_MaxSets = count;
		return *this;
	}

	Scope<VulkanDescriptorPool> VulkanDescriptorPool::Builder::build() const {
		return make_scope<VulkanDescriptorPool>(m_Device, m_MaxSets, m_PoolFlags, m_PoolSizes);
	}

	// *************** Descriptor Pool *********************

	VulkanDescriptorPool::VulkanDescriptorPool(
		VulkanDevice& device,
		uint32_t maxSets,
		VkDescriptorPoolCreateFlags poolFlags,
		const vector<VkDescriptorPoolSize>& poolSizes)
		: m_Device{ device } {
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.flags = poolFlags;

		if (vkCreateDescriptorPool(m_Device.get_device(), &descriptorPoolInfo, nullptr, &m_DescriptorPool) !=
			VK_SUCCESS) {
			HTHROW(std::runtime_error, "failed to create descriptor pool!");
		}
	}

	VulkanDescriptorPool::~VulkanDescriptorPool() {
		vkDestroyDescriptorPool(m_Device.get_device(), m_DescriptorPool, nullptr);
	}

	// TODO: Rename to allocateDescriptorSet
	bool VulkanDescriptorPool::allocate_descriptor(
		const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const {
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.pSetLayouts = &descriptorSetLayout;
		allocInfo.descriptorSetCount = 1;

		// Might want to create a "DescriptorPoolManager" class that handles this case, and builds
		// a new pool whenever an old pool fills up. But this is beyond our current scope
		// A more production ready descriptor abstraction: https://vkguide.dev/docs/extra-chapter/abstracting_descriptors/
		if (vkAllocateDescriptorSets(m_Device.get_device(), &allocInfo, &descriptor) != VK_SUCCESS) {
			return false;
		}
		return true;
	}

	void VulkanDescriptorPool::free_descriptors(vector<VkDescriptorSet>& descriptors) const {
		vkFreeDescriptorSets(
			m_Device.get_device(),
			m_DescriptorPool,
			static_cast<uint32_t>(descriptors.size()),
			descriptors.data());
	}

	void VulkanDescriptorPool::reset_pool() {
		vkResetDescriptorPool(m_Device.get_device(), m_DescriptorPool, 0);
	}

	// *************** Descriptor Writer *********************

	VulkanDescriptorWriter::VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool)
		: m_SetLayout{ setLayout }, m_Pool{ pool } {
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::write_buffer(
		uint32_t binding, VkDescriptorBufferInfo* bufferInfo) {
		assert(m_SetLayout.m_Bindings.count(binding) == 1 && "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout.m_Bindings[binding];

		assert(
			bindingDescription.descriptorCount == 1 &&
			"Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = bufferInfo;
		write.descriptorCount = 1;

		m_Writes.push_back(write);
		return *this;
	}

	VulkanDescriptorWriter& VulkanDescriptorWriter::write_image(
		uint32_t binding, VkDescriptorImageInfo* imageInfo) {
		assert(m_SetLayout.m_Bindings.count(binding) == 1 && "Layout does not contain specified binding");

		auto& bindingDescription = m_SetLayout.m_Bindings[binding];

		assert(
			bindingDescription.descriptorCount == 1 &&
			"Binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = imageInfo;
		write.descriptorCount = 1;

		m_Writes.push_back(write);
		return *this;
	}

	bool VulkanDescriptorWriter::build(VkDescriptorSet& set) {
		bool success = m_Pool.allocate_descriptor(m_SetLayout.get_descriptor_set_layout(), set);
		if (!success) {
			return false;
		}
		overwrite(set);
		return true;
	}

	void VulkanDescriptorWriter::overwrite(VkDescriptorSet& set) {
		for (auto& write : m_Writes) {
			write.dstSet = set;
		}
		vkUpdateDescriptorSets(m_Pool.m_Device.get_device(), static_cast<u32>(m_Writes.size()), m_Writes.data(), 0, nullptr);
	}
}
