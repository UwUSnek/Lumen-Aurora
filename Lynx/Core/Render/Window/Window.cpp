#include "Lynx/Core/IncludeVulkan.hpp"
#include <climits>
#include "Lynx/Core/Core.hpp"
#include "Lynx/Core/Render/Window/Window.hpp"
#include "Lynx/Core/Render/GCommands.hpp"
#include "Lynx/Core/Render/Shaders/Shader.hpp"
#include "Lynx/Core/Input/Input.hpp"
#include "Lynx/Types/Containers/RaArray.hpp"
#include "Lynx/Types/Object/2D/RenderSpace2.hpp"

#include "Lynx/shaders/Border2.hpp"
#include "Lynx/shaders/Line2.hpp"
#include "Lynx/shaders/FloatToIntBuffer.hpp"







namespace lnx{
	void Window::run(){
		_dbg(thr::self::setName("App | Window"));
		init();
		renderLoop();
		clear();
		pthread_exit(nullptr);
	}




	void Window::init() {


		window = glfwCreateWindow((i32)width, (i32)height, "Lynx Engine", nullptr, nullptr);
		switch(glfwCreateWindowSurface(core::dvc::instance, window, nullptr, rcast<vk::SurfaceKHR::CType*>(&surface))){
			case VkResult::VK_SUCCESS: break;
			case VkResult::VK_ERROR_INITIALIZATION_FAILED: dbg::printError("Initialization failed"); break;
			case VkResult::VK_ERROR_EXTENSION_NOT_PRESENT: dbg::printError("Extension not present"); break;
			default: _dbg(dbg::printError("Unknown result")) _rls(noop);
		}


		{ //Set icon
			#include "__tmp__IconData.hpp"
			unsigned char* h = (unsigned char*)malloc(64*64*4);	//FIXME REMOVE
  			memcpy(h, h2, 64*64*4);								//FIXME REMOVE
  			for(int i = 0; i < 64*64; ++i) {					//FIXME REMOVE
	  			auto tmp = h[i*4 + 0];
	  			h[i*4 + 0] = h[i*4 + 2];
	  			h[i*4 + 2] = tmp;
  			}
			GLFWimage icon{
				.width = 64,
				.height = 64,
				.pixels = h
			};
			glfwSetWindowIcon(window, 1, &icon);
		}


		glfwSetWindowUserPointer      (window, this);
		glfwSetFramebufferSizeCallback(window, resizeCallback);
		glfwSetCursorPosCallback      (window, input::onMove);
		glfwSetMouseButtonCallback    (window, input::onClick);
		glfwSetScrollCallback         (window, input::onAxis);
		glfwSetCursorEnterCallback    (window, input::onEnter);
		// glfwSetKeyCallback            (window, input::onKey); //FIXME


		renderCore.w = this;
		renderCore.init();



		//FIXME ADD RECREATE FUNCTION TO GENERATED INTERFACES
		initialized = true;
	}








	//Creates the default command buffers used for the render
	void Window::createDefaultCommandBuffers__() { //TODO
		{ //Render command pool
			auto commandPoolCreateInfo = vk::CommandPoolCreateInfo() 					//Create command pool create infos
				.setFlags            (vk::CommandPoolCreateFlagBits::eResetCommandBuffer)	//Command buffers and pool can be reset
				.setQueueFamilyIndex (core::dvc::graphics.pd.indices.computeFamilies[0])		//Set the compute family where to bind the command pool
			;
			switch(core::dvc::graphics.ld.createCommandPool(&commandPoolCreateInfo, nullptr, &renderCore.commandPool)){ vkDefaultCases; }
		}




		{ //Copy
			auto commandPoolCreateInfo = vk::CommandPoolCreateInfo() 					//Create command pool
				.setQueueFamilyIndex (core::dvc::graphics.pd.indices.computeFamilies[0])	//Set the compute family where to bind the command pool
			; //FIXME
			switch(core::dvc::graphics.ld.createCommandPool(&commandPoolCreateInfo, nullptr, &renderCore.copyCommandPool)){ vkDefaultCases; }

			auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo() 			//Allocate one command buffer for each swapchain image
				.setCommandPool        (renderCore.copyCommandPool)									//Set command pool where to allocate the command buffer
				.setLevel              (vk::CommandBufferLevel::ePrimary)					//Set the command buffer as primary level command buffer
				.setCommandBufferCount (renderCore.swp.images.count())									//Set command buffer count
			;
			switch(core::dvc::graphics.ld.allocateCommandBuffers(&commandBufferAllocateInfo, renderCore.copyCommandBuffers.begin())){ vkDefaultCases; }




			//Record a present command buffers for each swapchain images
			for(uint32 imgIndex = 0; imgIndex < renderCore.swp.images.count(); imgIndex++) {
				auto beginInfo = vk::CommandBufferBeginInfo() 							//Simultaneous use allows the command buffer to be executed multiple times
					.setFlags (vk::CommandBufferUsageFlagBits::eSimultaneousUse)
				;
				//Start recording commands
				switch(renderCore.copyCommandBuffers[imgIndex].begin(&beginInfo)){ vkDefaultCases; }
					//Create a barrier to use the swapchain image as an optimal transfer destination to copy the buffer in it
					readToWriteBarrier.image = renderCore.swp.images[imgIndex].image;					//Set swapchain image
					vk::PipelineStageFlags 													//Create stage flags
						srcStage = vk::PipelineStageFlagBits::eColorAttachmentOutput,			//The swapchain image is in color output stage
						dstStage = vk::PipelineStageFlagBits::eTransfer;						//Change it to transfer stage to copy the buffer in it
					renderCore.copyCommandBuffers[imgIndex].pipelineBarrier(srcStage, dstStage, vk::DependencyFlagBits::eDeviceGroup, 0, nullptr, 0, nullptr, 1, &readToWriteBarrier);
					//! ^ https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDependencyFlagBits.html //FIXME dependency flags was 0 but C++ doesn't allow that

					copyRegion.imageExtent = vk::Extent3D{ renderCore.swp.createInfo.imageExtent.width, renderCore.swp.createInfo.imageExtent.height, 1 };	//Copy the whole buffer
					renderCore.copyCommandBuffers[imgIndex].copyBufferToImage(renderCore.iOut_g.cell->csc.buffer, renderCore.swp.images[imgIndex].image, vk::ImageLayout::eTransferDstOptimal, 1, &copyRegion);


					//Create a barrier to use the swapchain image as a present source image
					writeToReadBarrier.image = renderCore.swp.images[imgIndex].image;					//Set swapchain image
					vk::PipelineStageFlags 													//Create stage flags
						srcStage1 = vk::PipelineStageFlagBits::eTransfer,						//The image is in transfer stage from the buffer copy
						dstStage1 = vk::PipelineStageFlagBits::eColorAttachmentOutput;			//Change it to color output to present them
					renderCore.copyCommandBuffers[imgIndex].pipelineBarrier(srcStage1, dstStage1, vk::DependencyFlagBits::eDeviceGroup, 0, nullptr, 0, nullptr, 1, &writeToReadBarrier);
					//! ^ https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkDependencyFlagBits.html //FIXME dependency flags was 0 but C++ doesn't allow that
				switch(renderCore.copyCommandBuffers[imgIndex].end()){ vkDefaultCases; }										//End command buffer recording
			}
		}
	}






	void Window::clear(){
		renderCore.clear();
		renderCore.swp.~Swapchain();
		core::dvc::instance.destroySurfaceKHR(surface, nullptr);
		glfwDestroyWindow(window);
		initialized = false;
	}


	void Window::close(){ //TODO add parameter to not wait for window to close
		running = false;
		while(initialized) thr::self::yield();
		// t.join();
	}


	void Window::qSpawn(obj::Obj_bb* pObject){
		// dbg::checkCond(thr::self::thr() == t.thr, "This function cannot be called by the render thread.");

		requests_m.lock();
			pObject->render.updates = pObject->render.updates | obj::UpdateBits::eSpawn;
			requests.add(pObject);
		requests_m.unlock();
	}
}