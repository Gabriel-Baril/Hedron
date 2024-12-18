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
			Builder(HDNDevice& lveDevice) : lveDevice{ lveDevice } {}

			Builder& addBinding(
				uint32_t binding,
				VkDescriptorType descriptorType,
				VkShaderStageFlags stageFlags,
				uint32_t count = 1);
			std::unique_ptr<HDNDescriptorSetLayout> build() const;

		private:
			HDNDevice& lveDevice;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
		};

		HDNDescriptorSetLayout(
			HDNDevice& lveDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		~HDNDescriptorSetLayout();
		HDNDescriptorSetLayout(const HDNDescriptorSetLayout&) = delete;
		HDNDescriptorSetLayout& operator=(const HDNDescriptorSetLayout&) = delete;

		VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

	private:
		HDNDevice& lveDevice;
		VkDescriptorSetLayout descriptorSetLayout;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

		friend class HDNDescriptorWriter;
	};

	class HDNDescriptorPool {
	public:
		class Builder {
		public:
			Builder(HDNDevice& lveDevice) : lveDevice{ lveDevice } {}

			Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
			Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
			Builder& setMaxSets(uint32_t count);
			std::unique_ptr<HDNDescriptorPool> build() const;

		private:
			HDNDevice& lveDevice;
			std::vector<VkDescriptorPoolSize> poolSizes{};
			uint32_t maxSets = 1000;
			VkDescriptorPoolCreateFlags poolFlags = 0;
		};

		HDNDescriptorPool(
			HDNDevice& lveDevice,
			uint32_t maxSets,
			VkDescriptorPoolCreateFlags poolFlags,
			const std::vector<VkDescriptorPoolSize>& poolSizes);
		~HDNDescriptorPool();
		HDNDescriptorPool(const HDNDescriptorPool&) = delete;
		HDNDescriptorPool& operator=(const HDNDescriptorPool&) = delete;

		bool allocateDescriptor(
			const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

		void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

		void resetPool();

		VkDescriptorPool GetDescriptor() const { return descriptorPool; }

	private:
		HDNDevice& lveDevice;
		VkDescriptorPool descriptorPool;

		friend class HDNDescriptorWriter;
	};

	class HDNDescriptorWriter {
	public:
		HDNDescriptorWriter(HDNDescriptorSetLayout& setLayout, HDNDescriptorPool& pool);

		HDNDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		HDNDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

		bool build(VkDescriptorSet& set);
		void overwrite(VkDescriptorSet& set);

	private:
		HDNDescriptorSetLayout& setLayout;
		HDNDescriptorPool& pool;
		std::vector<VkWriteDescriptorSet> writes;
	};
}