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
		bool IsComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
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

		VkCommandPool GetCommandPool() { return m_CommandPool; }
		VkDevice GetDevice() { return m_Device; }
		VkSurfaceKHR GetSurface() { return m_Surface; }
		VkInstance GetInstance() { return m_Instance; }
		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() { return m_PresentQueue; }
		VkPhysicalDeviceProperties GetPhysicalDeviceProperties() { return m_Properties; }

		SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(m_PhysicalDevice); }
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_PhysicalDevice); }
		VkFormat FindSupportedFormat(
			const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		// Buffer Helper Functions
		void CreateBuffer(
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory);
		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

		void CreateImageWithInfo(
			const VkImageCreateInfo& imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory);
	private:
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateCommandPool();

		// helper functions
		bool IsDeviceSuitable(VkPhysicalDevice device);
		vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void HasGflwRequiredInstanceExtensions();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

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