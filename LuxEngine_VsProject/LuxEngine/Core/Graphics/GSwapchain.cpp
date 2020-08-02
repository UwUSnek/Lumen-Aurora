﻿
#include "LuxEngine/Core/LuxCore.h"
#include "LuxEngine/Core/Graphics/GSwapchain.h"








namespace lux::core::g{
	VkSwapchainKHR			swapchain;
	Array<VkImage>			swapchainImages;
	Array<VkImageView>		swapchainImageViews;
	VkFormat				swapchainImageFormat;
	VkExtent2D				swapchainExtent;
	Array<VkFramebuffer>	swapchainFramebuffers;








	VkSurfaceFormatKHR swapchainChooseSurfaceFormat(const lux::Array<VkSurfaceFormatKHR>* pAvailableFormats) {
		for(const auto& availableFormat : *pAvailableFormats) {
			//if(availableFormat.format == VK_FORMAT_R32G32B32A32_SFLOAT && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			if(availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return (*pAvailableFormats)[0];
	}




	//Returns the presentation mode that will be used. Use immediate or mailbox (causes tearing), FIFO if using VSync
	VkPresentModeKHR swapchainChoosePresentMode(const lux::Array<VkPresentModeKHR>* pAvailablePresentModes) {
		if(lux::getEngine().useVSync) return VK_PRESENT_MODE_FIFO_KHR;
		for(const auto& availablePresentMode : *pAvailablePresentModes) {
			if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) return availablePresentMode;
		}
		return VK_PRESENT_MODE_IMMEDIATE_KHR;
	}




	VkExtent2D swapchainChooseExtent(const VkSurfaceCapabilitiesKHR* pCapabilities) {
		int32 width, height;
		glfwGetFramebufferSize(lux::core::g::window, &width, &height);
		return VkExtent2D{
			max(pCapabilities->minImageExtent.width, min(pCapabilities->maxImageExtent.width, (uint32)width)),
			max(pCapabilities->minImageExtent.height, min(pCapabilities->maxImageExtent.height, (uint32)height))
		};
	}





	SwapChainSupportDetails swapchainQuerySupport(const VkPhysicalDevice vDevice) {
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vDevice, lux::getEngine().surface, &details.capabilities);

		uint32 formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(vDevice, lux::getEngine().surface, &formatCount, nullptr);
		if(formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(vDevice, lux::getEngine().surface, &formatCount, details.formats.begin( ));
		}

		uint32 presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(vDevice, lux::getEngine().surface, &presentModeCount, nullptr);
		if(presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(vDevice, lux::getEngine().surface, &presentModeCount, details.presentModes.begin( ));
		}

		return details;
	}








	//-------------------------------------------------------------------------------------------------------------------------------------------------//








	void swapchainCreate( ) {
		//Get swapchain details
		SwapChainSupportDetails swapChainSupport = swapchainQuerySupport(lux::core::g::graphics.PD.device);

		//Choose max image count. Minimum or minimum +1 if supported
		uint32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}


		//swapchain creation infos
		VkSurfaceFormatKHR surfaceFormat{ swapchainChooseSurfaceFormat(&swapChainSupport.formats) };
		VkSwapchainCreateInfoKHR createInfo{
			.sType{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR },
			.surface{ lux::getEngine().surface },
			.minImageCount{ imageCount },
			.imageFormat{ surfaceFormat.format },
			.imageColorSpace{ surfaceFormat.colorSpace },
			.imageExtent{ swapchainChooseExtent(&swapChainSupport.capabilities) },
			.imageArrayLayers{ 1 },
			.imageUsage{ VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT },
		};

		uint32 queueFamilyIndices[] = { lux::core::g::graphics.PD.indices.graphicsFamily, lux::core::g::graphics.PD.indices.presentFamily };
		if(lux::core::g::graphics.PD.indices.graphicsFamily != lux::core::g::graphics.PD.indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = swapchainChoosePresentMode(&swapChainSupport.presentModes);
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;


		//Create swapchain
		TryVk(vkCreateSwapchainKHR(lux::core::g::graphics.LD, &createInfo, nullptr, &lux::core::g::swapchain)) Exit("Failed to create swapchain");


		//Save data
		uint32 swapchainImageCount;
		vkGetSwapchainImagesKHR(lux::core::g::graphics.LD, lux::core::g::swapchain, &swapchainImageCount, nullptr);					//Get image count
		lux::core::g::swapchainImages.resize(swapchainImageCount);
		vkGetSwapchainImagesKHR(lux::core::g::graphics.LD, lux::core::g::swapchain, &swapchainImageCount, lux::core::g::swapchainImages.begin( ));	//Save images
		lux::core::g::swapchainImageFormat = surfaceFormat.format;													//Save format
		lux::core::g::swapchainExtent = createInfo.imageExtent;														//Save extent


		//Create image views
		lux::core::g::swapchainImageViews.resize(lux::core::g::swapchainImages.size( ));
		for(uint32 i = 0; i < lux::core::g::swapchainImages.size( ); ++i) lux::core::g::swapchainImageViews[i] = lux::core::g::swapchainCreateImageView(lux::core::g::swapchainImages[i], lux::core::g::swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);


		lux::core::g::createRenderPass( );
		lux::core::g::createFramebuffers( );
	}








	void swapchainCleanup( ) {
		vkDestroyRenderPass(lux::core::g::graphics.LD, lux::core::g::renderPass, nullptr);													//Destroy render pass
		for(auto framebuffer : lux::core::g::swapchainFramebuffers) vkDestroyFramebuffer(lux::core::g::graphics.LD, framebuffer, nullptr);	//Destroy framebuffers
		for(auto imageView : lux::core::g::swapchainImageViews) vkDestroyImageView(lux::core::g::graphics.LD, imageView, nullptr);			//Destroy image viewslux::core::g::compute
		vkDestroySwapchainKHR(lux::core::g::graphics.LD, lux::core::g::swapchain, nullptr);													//destroy swapchain
	}






	void swapchainRecreate(const bool vWindowResized) {
		if(vWindowResized) lux::core::g::windowResizeFence.startFirst( );
		int32 width, height;
		glfwGetFramebufferSize(lux::core::g::window, &width, &height);

		if(width != 0 && height != 0) {
			vkDeviceWaitIdle(lux::core::g::graphics.LD);
			swapchainCleanup( );
			swapchainCreate( );


			{ //destroy copy command buffers
				vkFreeCommandBuffers(lux::core::g::compute.LD, lux::core::c::copyCommandPool, lux::core::c::copyCommandBuffers.size( ), lux::core::c::copyCommandBuffers.begin( ));
				vkDestroyCommandPool(lux::core::g::compute.LD, lux::core::c::copyCommandPool, nullptr);
			}

			uint32* pwindowSize = scast<uint32*>(lux::core::c::gpuCellMap(lux::core::g::gpuCellWindowSize));
			pwindowSize[0] = lux::core::g::swapchainExtent.width;
			pwindowSize[1] = lux::core::g::swapchainExtent.height;

			{ //#LLID CCB0000 Create copy command buffers
				lux::core::c::copyCommandBuffers.resize(lux::core::g::swapchainImages.size( ));	//Resize the command buffer array in the shader
				lux::core::c::cshaderCreateDefaultCommandBuffers( );				//Create command buffers and command pool
			}
		}
		if(vWindowResized) lux::core::g::windowResizeFence.endFirst( );
	}
}