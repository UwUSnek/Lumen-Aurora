﻿
#include "LuxEngine/Engine/Engine.h"
#include "LuxEngine/Engine/Compute/CShader_t.h"






//Dark magic. Don't worry, it works
static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallbackFn(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64 object, uint64 location, int32 messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData) {
	printf("Debug Report: %s: %s\n", pLayerPrefix, pMessage);
	return VK_FALSE;
}





void Engine::runCompute() {
	__windowOutput = gpuCellCreate(swapchainExtent.width * swapchainExtent.height * 4/*A8-R8-G8-B8*/, false);

	__windowSize = gpuCellCreate(4 * 2, true);
	uint32* pwindowSize = rcast<uint32*>(gpuCellMap(__windowSize));
	pwindowSize[0] = swapchainExtent.width;
	pwindowSize[1] = swapchainExtent.height;


	__vertices = gpuCellCreate(4 * 2, true);
	uint32* pVertices = rcast<uint32*>(gpuCellMap(__vertices));
	pVertices[0] = 100;
	pVertices[1] = 10;

	LuxArray<LuxCell> cells = { __windowOutput, __windowSize, __vertices };
	testShader0 = cshaderNew(&cells, "LuxEngine/Contents/shaders/comp.spv");

	{ //#LLID CCB0000 Create copy command buffers 
		copyShader = CShaders.add(LuxShader_t{});							//Add the shader to the shader array
		CShaders[copyShader].commandBuffers.resize(swapchainImages.size());//Resize the command buffer array in the shader
		__lp_cshaderCreateCopyCommandBuffers();									//Create command buffers and command pool
	}
}






void Engine::cleanupCompute() {
	forEach(CBuffers, i) {
		if (CBuffers.isValid(i)) {
			vkDestroyBuffer(compute.LD, CBuffers[i].buffer, null);							//Destroy the buffer
			vkFreeMemory(compute.LD, CBuffers[i].memory, null);								//Free the memory of the buffer
		}
	}

	//TODO fix
	//vkDestroyDescriptorPool(compute.LD, computeDescriptorPool, null);				//Destroy the descriptor pool
	//vkDestroyDescriptorSetLayout(compute.LD, computeDescriptorSetLayout, null);		//Destroy the descriptor set layout

	//vkDestroyPipeline(compute.LD, computePipeline, null);							//Destroy the pipeline
	//vkDestroyPipelineLayout(compute.LD, computePipelineLayout, null);				//Destroy the pipeline layout

	//vkDestroyCommandPool(compute.LD, computeCommandPool, null);						//Destroy the compute command pool
}
