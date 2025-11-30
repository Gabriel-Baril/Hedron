#pragma once

#include "r_vk_device.h"

namespace dm
{
	class VulkanBuffer
	{
	public:
		VulkanBuffer(
				VulkanDevice *device,
				VkDeviceSize instanceSize,
				uint32_t instanceCount,
				VkBufferUsageFlags usageFlags,
				VkMemoryPropertyFlags memoryPropertyFlags,
				VkDeviceSize minOffsetAlignment = 1);
		~VulkanBuffer();

		VulkanBuffer(const VulkanBuffer &) = delete;
		VulkanBuffer &operator=(const VulkanBuffer &) = delete;

		VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		void unmap();

		void write_to_buffer(void *data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		VkDescriptorBufferInfo build_descriptor_info(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

		void write_to_index(void *data, int index);
		VkResult flush_index(int index);
		VkDescriptorBufferInfo descriptor_info_for_index(int index);
		VkResult invalidate_index(int index);

		VkBuffer get_buffer() const { return m_Buffer; }
		void *get_mapped_memory() const { return m_Mapped; }
		uint32_t get_instance_count() const { return m_InstanceCount; }
		VkDeviceSize get_instance_size() const { return m_InstanceSize; }
		VkDeviceSize get_alignment_size() const { return m_InstanceSize; }
		VkBufferUsageFlags get_usage_flags() const { return m_UsageFlags; }
		VkMemoryPropertyFlags get_memory_property_flags() const { return m_MemoryPropertyFlags; }
		VkDeviceSize get_buffer_size() const { return m_BufferSize; }

	private:
		static VkDeviceSize get_alignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

		VulkanDevice *m_Device;
		void *m_Mapped = nullptr;
		VkBuffer m_Buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_Memory = VK_NULL_HANDLE;

		VkDeviceSize m_BufferSize;
		uint32_t m_InstanceCount;
		VkDeviceSize m_InstanceSize;
		VkDeviceSize m_AlignmentSize;
		VkBufferUsageFlags m_UsageFlags;
		VkMemoryPropertyFlags m_MemoryPropertyFlags;
	};
}
