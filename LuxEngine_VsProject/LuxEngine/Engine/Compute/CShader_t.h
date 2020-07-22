#pragma once


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
	lux::Array <VkCommandBuffer> commandBuffers;

	//Since vulkan uses pointers everywhere, I need to create the objects with malloc() to prevent them from vanishing randomly
	//This array saves all of them so they can be free()d when the shader is destroyed
	lux::DynArray<void*> __lp_ptrs;
};

