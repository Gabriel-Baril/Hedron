#pragma once

#include "editor_panel.h"
#include <vulkan/vulkan.h>
#include "imgui.h"
#include "gfx/r_vk_swapchain.h"

#include "core/stl/vector.h"

namespace hdn
{

	class ViewportPanel : public IEditorPanel
	{
	public:
		ViewportPanel()
			: IEditorPanel{ "viewport" }
		{
		}

		void on_update( float dt ) override
		{
			// Render Vulkan Scene Texture into ImGui
			ImGui::Image((ImTextureID)sceneDescriptorSets[m_FrameIndex], ImVec2(1280, 720));
		}

		void transition_image(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout) {
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = sceneImage;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			VkPipelineStageFlags sourceStage;
			VkPipelineStageFlags destinationStage;

			if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
				barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}

			vkCmdPipelineBarrier(
				commandBuffer,
				sourceStage, destinationStage,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);
		}

		uint32_t find_memory_type(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i; // Found a suitable memory type
				}
			}

			throw std::runtime_error("Failed to find suitable memory type!");
		}

		void create_offscreen_rendertarget(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D extent, VkRenderPass renderPass) {
			VkImageCreateInfo imageCreateInfo = {};
			imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			imageCreateInfo.extent.width = extent.width;
			imageCreateInfo.extent.height = extent.height;
			imageCreateInfo.extent.depth = 1;
			imageCreateInfo.mipLevels = 1;
			imageCreateInfo.arrayLayers = 1;
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

			vkCreateImage(device, &imageCreateInfo, nullptr, &sceneImage);

			// Allocate Memory
			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(device, sceneImage, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = find_memory_type(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			vkAllocateMemory(device, &allocInfo, nullptr, &sceneImageMemory);
			vkBindImageMemory(device, sceneImage, sceneImageMemory, 0);

			// Create Image View
			VkImageViewCreateInfo viewInfo = {};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = sceneImage;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			vkCreateImageView(device, &viewInfo, nullptr, &sceneImageView);

			// Create Sampler
			VkSamplerCreateInfo samplerInfo = {};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

			vkCreateSampler(device, &samplerInfo, nullptr, &sceneSampler);

			// Create Framebuffer
			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = &sceneImageView;
			framebufferInfo.width = extent.width;
			framebufferInfo.height = extent.height;
			framebufferInfo.layers = 1;

			vkCreateFramebuffer(device, &framebufferInfo, nullptr, &sceneFramebuffer);
		}

		void create_descriptor_set(VkDevice device) {
			for (int i = 0;i < sceneDescriptorSets.size(); i++)
			{
				VkDescriptorSetLayoutBinding layoutBinding = {};
				layoutBinding.binding = 0;
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				layoutBinding.descriptorCount = 1;
				layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

				VkDescriptorSetLayoutCreateInfo layoutInfo = {};
				layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				layoutInfo.bindingCount = 1;
				layoutInfo.pBindings = &layoutBinding;

				vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &sceneDescriptorSetLayout);

				// Create Descriptor Pool
				VkDescriptorPoolSize poolSize = {};
				poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				poolSize.descriptorCount = 1;

				VkDescriptorPoolCreateInfo poolInfo = {};
				poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				poolInfo.poolSizeCount = 1;
				poolInfo.pPoolSizes = &poolSize;
				poolInfo.maxSets = 1;

				vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);

				// Allocate Descriptor Set
				VkDescriptorSetAllocateInfo allocInfo = {};
				allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
				allocInfo.descriptorPool = descriptorPool;
				allocInfo.descriptorSetCount = 1;
				allocInfo.pSetLayouts = &sceneDescriptorSetLayout;

				vkAllocateDescriptorSets(device, &allocInfo, &sceneDescriptorSets[i]);
			}
		}

		void update_descriptor_set(VkDevice device, int frameIndex) {
			m_FrameIndex = frameIndex;

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageView = sceneImageView;
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.sampler = sceneSampler;

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = sceneDescriptorSets[m_FrameIndex];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
		}
	private:
		int m_FrameIndex;

		VkImage sceneImage;
		VkImageView sceneImageView;
		VkDeviceMemory sceneImageMemory;
		VkFramebuffer sceneFramebuffer;
		VkSampler sceneSampler;

		vector<VkDescriptorSet> sceneDescriptorSets{ VulkanSwapChain::MAX_FRAMES_IN_FLIGHT };
		VkDescriptorSetLayout sceneDescriptorSetLayout;
		VkDescriptorPool descriptorPool;
	};
}