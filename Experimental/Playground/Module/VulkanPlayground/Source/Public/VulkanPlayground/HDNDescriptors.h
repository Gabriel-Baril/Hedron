#pragma once

#include "HDNDevice.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace hdn {

	class HDNDescriptorSetLayout {
	public:
		class Builder {
		public:
			Builder(HDNDevice& device) : m_Device{ device } {}

			Builder& AddBinding(
				uint32_t binding,
				VkDescriptorType descriptorType,
				VkShaderStageFlags stageFlags,
				uint32_t count = 1);
			Scope<HDNDescriptorSetLayout> Build() const;

		private:
			HDNDevice& m_Device;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
		};

		HDNDescriptorSetLayout(
			HDNDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		~HDNDescriptorSetLayout();
		HDNDescriptorSetLayout(const HDNDescriptorSetLayout&) = delete;
		HDNDescriptorSetLayout& operator=(const HDNDescriptorSetLayout&) = delete;

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_DescriptorSetLayout; }

	private:
		HDNDevice& m_Device;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;

		friend class HDNDescriptorWriter;
	};

	class HDNDescriptorPool {
	public:
		class Builder {
		public:
			Builder(HDNDevice& device) : m_Device{ device } {}

			Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
			Builder& SetPoolFlags(VkDescriptorPoolCreateFlags flags);
			Builder& SetMaxSets(uint32_t count);
			Scope<HDNDescriptorPool> Build() const;

		private:
			HDNDevice& m_Device;
			std::vector<VkDescriptorPoolSize> m_PoolSizes{};
			uint32_t m_MaxSets = 1000;
			VkDescriptorPoolCreateFlags m_PoolFlags = 0;
		};

		HDNDescriptorPool(
			HDNDevice& device,
			uint32_t maxSets,
			VkDescriptorPoolCreateFlags poolFlags,
			const std::vector<VkDescriptorPoolSize>& poolSizes);
		~HDNDescriptorPool();
		HDNDescriptorPool(const HDNDescriptorPool&) = delete;
		HDNDescriptorPool& operator=(const HDNDescriptorPool&) = delete;

		bool AllocateDescriptor(
			const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

		void FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

		void ResetPool();

		VkDescriptorPool GetDescriptor() const { return m_DescriptorPool; }

	private:
		HDNDevice& m_Device;
		VkDescriptorPool m_DescriptorPool;

		friend class HDNDescriptorWriter;
	};

	class HDNDescriptorWriter {
	public:
		HDNDescriptorWriter(HDNDescriptorSetLayout& setLayout, HDNDescriptorPool& pool);

		HDNDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		HDNDescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

		bool Build(VkDescriptorSet& set);
		void Overwrite(VkDescriptorSet& set);

	private:
		HDNDescriptorSetLayout& m_SetLayout;
		HDNDescriptorPool& m_Pool;
		std::vector<VkWriteDescriptorSet> m_Writes;
	};
}