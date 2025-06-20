#pragma once

#include "r_vk_window.h"

#include "core/stl/vector.h"

namespace hdn {
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities{};
		vector<VkSurfaceFormatKHR> formats{};
		vector<VkPresentModeKHR> presentModes{};
	};

	struct QueueFamilyIndices
	{
		uint32_t graphicsFamily = 0;
		uint32_t presentFamily = 0;
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool is_complete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class VulkanDevice
	{
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		VulkanDevice(VulkanWindow& window);
		~VulkanDevice();

		// Not copyable or movable
		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice& operator=(const VulkanDevice&) = delete;
		VulkanDevice(VulkanDevice&&) = delete;
		VulkanDevice& operator=(VulkanDevice&&) = delete;

		VkCommandPool get_command_pool() { return m_CommandPool; }
		VkDevice get_device() { return m_Device; }
		VkSurfaceKHR get_surface() { return m_Surface; }
		VkInstance get_instance() { return m_Instance; }
		VkPhysicalDevice get_physical_device() { return m_PhysicalDevice; }
		VkQueue get_graphics_queue() { return m_GraphicsQueue; }
		VkQueue get_present_queue() { return m_PresentQueue; }
		VkPhysicalDeviceProperties get_physical_device_properties() { return m_Properties; }

		SwapChainSupportDetails get_swap_chain_support() { return query_swap_chain_support(m_PhysicalDevice); }
		uint32_t find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices find_physical_queue_families() { return find_queue_families(m_PhysicalDevice); }
		VkFormat find_supported_format(
			const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		// Buffer Helper Functions
		void create_buffer(
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory);
		VkCommandBuffer begin_single_time_commands();
		void end_single_time_commands(VkCommandBuffer commandBuffer);
		void copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void copy_buffer_to_image(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

		void create_image_with_info(
			const VkImageCreateInfo& imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory);
	private:
		void create_instance();
		void setup_debug_messenger();
		void create_surface();
		void pick_physical_device();
		void create_logical_device();
		void create_command_pool();

		// helper functions
		bool is_device_suitable(VkPhysicalDevice device);
		vector<const char*> get_required_extensions();
		bool check_validation_layer_support();
		QueueFamilyIndices find_queue_families(VkPhysicalDevice device);
		void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void has_gflw_required_instance_extensions();
		bool check_device_extension_support(VkPhysicalDevice device);
		SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);

		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VulkanWindow& m_Window;
		VkCommandPool m_CommandPool;

		VkDevice m_Device;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		VkPhysicalDeviceProperties m_Properties;

		const vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		const vector<const char*> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};

}