#pragma once
#define LUX_H_SWAPCHAIN
#include "LuxEngine/Core/Render/Shaders/Shader_t.hpp"





namespace lux{
	struct Window;
}
namespace lux::core::wnd{
	constexpr int32 __renderMaxFramesInFlight = 2;	//Default:2




	struct Swapchain{
	private:
		friend class lux::Window;

		VkSwapchainKHR			 swapchain;
		public: //FIXME
		VkSwapchainCreateInfoKHR createInfo;
		private:
		VkRenderPass             renderPass;
		bool                     useVSync = true;


		struct SwpFrame{
			VkSemaphore s_imageAcquired;
			VkSemaphore s_objectsRendered;
			VkSemaphore s_copy;
			VkSemaphore s_clear;
			VkFence     f_imageRendered;
		};
		RtArray<SwpFrame> frames;
		uint32            curFrame = 0;		//The index of the frame that is currently being rendered


		struct SwpImages{
			VkImage		images;
			VkImageView	imageViews;
		};
		RtArray<SwpImages> imgs;
		RtArray<VkFramebuffer>	framebuffers;
		VkImageView        createImageView(const VkImage vImage, const VkFormat vFormat, const VkImageAspectFlags vAspectFlags);




		VkSurfaceFormatKHR chooseSurfaceFormat(const RtArray<VkSurfaceFormatKHR>& pAvailableFormats);
		VkPresentModeKHR   choosePresentMode(const RtArray<VkPresentModeKHR>& pAvailablePresentModes);
		VkExtent2D         chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR* pCapabilities);

		VkSurfaceCapabilitiesKHR    getCapabilities();
		RtArray<VkSurfaceFormatKHR> getSurfaceFormats();
		RtArray<VkPresentModeKHR>   getPresentModes();




	public:
		Window* bindedWindow;
		bool renderFramebufferResized = false;

		RtArray<LuxShader_t, uint32> shaders;
		RtArray<VkCommandBuffer>     shadersCBs;


		Swapchain();
		void create(bool vUseVSync);
		void createRenderPass();
		void createFramebuffers();

		void recreate();
		void destroy();
		~Swapchain();
	};
}
