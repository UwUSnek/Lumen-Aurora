﻿
#include "LuxEngine/Engine/Engine.h"
#include "LuxEngine/Engine/Compute/CShader.struct.h"






//Dark magic. Don't worry, it works
static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallbackFn(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64 object, uint64 location, int32 messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData) {
	printf("Debug Report: %s: %s\n", pLayerPrefix, pMessage);
	return VK_FALSE;
}





void Engine::runCompute() {
	{ //Create copy command buffers #LLID CCB0000
		copyShader = CShaders.add(LuxCShader{});								//Add the shader to the shader array
		CShaders[copyShader].commandBuffers.resize(swapchainImages.size());	//Resize the command buffer array in the shader
		__lp_cshaderCreateCopyCommandBuffers();										//Create command buffers and command pool
	}


	__windowSize = gpuCellCreate(4 * 2, true);
	uint32* pwindowSize = (uint32*)gpuCellMap(__windowSize);
	pwindowSize[0] = width;
	pwindowSize[1] = height;

	__vertices = gpuCellCreate(4 * 2, true);
	uint32* mappedVertices = (uint32*)gpuCellMap(__vertices);
	mappedVertices[0] = 100;
	mappedVertices[1] = 10;

	LuxArray<LuxCell> cells = { __windowOutput, __windowSize, __vertices };
	testShader0 = cshaderNew(&cells, "LuxEngine/Contents/shaders/comp.spv");
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
