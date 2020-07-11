#pragma once
#include <vulkan/vulkan.h>
#include "LuxEngine/Types/Containers/LuxArray.h"
#include "LuxEngine/Types/Containers/LuxDynArray.h"

//A container struct for the components of a shader
struct LuxShader_t {
	//Descriptors
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;
	VkDescriptorSetLayout descriptorSetLayout;

	//Pipeline
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;

	//Commands
	VkCommandPool commandPool;
	LuxArray <VkCommandBuffer> commandBuffers;

	//Since vulkan uses pointers everywhere, I need to create the objects with malloc() to prevent them from vanishing randomly
	//This array saves all of them so they can be free()d when the shader is destroyed
	LuxDynArray<void*> __lp_ptrs;
};

