
//####################################################################################
// This file was generated by Lynx/shaders/GlslToCpp.py
// Changes could be overwritten without notice
//####################################################################################

#include "Lynx/shaders/Line2.gsi.hpp"
#include "Lynx/Core/AutoInit.hpp"
#include "Lynx/Core/Render/Shaders/Shader.hpp"
#include "Lynx/Core/Render/Render.hpp"
#define LNX_H_LINE2



namespace lnx::shd::gsi{


	void Line2::create(
		const l_outcol& pOutcol,
		const l_wsize& pWsize,
		const l_zbuff& pZbuff,
		const u32v3 vGroupCount, core::RenderCore& pRenderCore
	){
		pRenderCore.addObject_m.lock();
			outcol = pOutcol;
			wsize = pWsize;
			zbuff = pZbuff;

			createDescriptorSets();
			createCommandBuffers(vGroupCount, pRenderCore);
			pRenderCore.swp.shadersCBs.add(commandBuffers[0]);
		pRenderCore.addObject_m.unlock();
	}


	void Line2::createDescriptorSets(){
		vk::DescriptorPoolSize sizes[2] = {
			vk::DescriptorPoolSize().setType(vk::DescriptorType::eStorageBuffer).setDescriptorCount(3),
			vk::DescriptorPoolSize().setType(vk::DescriptorType::eUniformBuffer).setDescriptorCount(1)
		};
		auto poolInfo = vk::DescriptorPoolCreateInfo()
			.setFlags         (vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
			.setMaxSets       (1)
			.setPoolSizeCount (2)
			.setPPoolSizes    (sizes)
		;
		switch(core::dvc::graphics.ld.createDescriptorPool(&poolInfo, nullptr, &descriptorPool)){
			case vk::Result::eErrorFragmentationEXT:  dbg::logError("Fragmentation error");  break;
			vkDefaultCases;
		}



		auto allocateSetInfo = vk::DescriptorSetAllocateInfo()
			.setDescriptorPool     (descriptorPool)
			.setDescriptorSetCount (1)
			.setPSetLayouts        (&Line2::layout.descriptorSetLayout)
		;
		switch(core::dvc::graphics.ld.allocateDescriptorSets(&allocateSetInfo, &descriptorSet)){
			case vk::Result::eErrorFragmentedPool:    dbg::logError("Fragmented pool");      break;
			case vk::Result::eErrorOutOfPoolMemory:   dbg::logError("Out of pool memory");   break;
			vkDefaultCases;
		}



		vk::WriteDescriptorSet writeSets[1];
		auto bufferInfo0 = vk::DescriptorBufferInfo()
			.setBuffer (_data.vdata.cell->csc.buffer)
			.setOffset (_data.vdata.cell->localOffset)
			.setRange  (_data.vdata.cell->cellSize)
		;
		writeSets[0] = vk::WriteDescriptorSet()
			.setDstSet          (descriptorSet)
			.setDstBinding      (3)
			.setDescriptorCount (1)
			.setDescriptorType  (vk::DescriptorType::eUniformBuffer)
			.setPBufferInfo     (&bufferInfo0)
		;
		core::dvc::graphics.ld.updateDescriptorSets(1, writeSets, 0, nullptr);
	}








void Line2::createCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore){
	auto allocateCbInfo = vk::CommandBufferAllocateInfo()
		.setCommandPool        (pRenderCore.commandPool)
		.setLevel              (vk::CommandBufferLevel::ePrimary)
		.setCommandBufferCount (1)
	;
	commandBuffers.resize(1);
	switch(core::dvc::graphics.ld.allocateCommandBuffers(&allocateCbInfo, commandBuffers.begin())){ vkDefaultCases; }

	auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
	switch(commandBuffers[0].begin(beginInfo)){ vkDefaultCases; }
	commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pRenderCore.pipelines[Line2::pipelineIndex]);
	commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, Line2::layout.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);
	switch(commandBuffers[0].end()){ vkDefaultCases; }
}








void Line2::updateCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore){
	auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
	switch(commandBuffers[0].begin(beginInfo)){ vkDefaultCases; }
	commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pRenderCore.pipelines[Line2::pipelineIndex]);
	commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, Line2::layout.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);
	switch(commandBuffers[0].end()){ vkDefaultCases; }
}








void Line2::destroy(){
	//TODO
}








ShaderInterface_b::Layout Line2::layout;
uint32 Line2::pipelineIndex = core::shaders::pipelineNum++;
LnxAutoInit(LNX_H_LINE2){
	core::shaders::pipelineLayouts.resize(core::shaders::pipelineNum);
	core::shaders::pipelineLayouts[Line2::pipelineIndex] = &Line2::layout;
	{ //Create descriptor set layout
		vk::DescriptorSetLayoutBinding bindingLayouts[1];
		bindingLayouts[0] = vk::DescriptorSetLayoutBinding()
			.setBinding            (3)
			.setDescriptorType     (vk::DescriptorType::eUniformBuffer)
			.setDescriptorCount    (1)
			.setStageFlags         (vk::ShaderStageFlagBits::eCompute)
			.setPImmutableSamplers (nullptr)
		;

		auto layoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
			.setBindingCount (1)
			.setPBindings    (bindingLayouts)
		;
		//Create the descriptor set layout
		switch(core::dvc::graphics.ld.createDescriptorSetLayout(&layoutCreateInfo, nullptr, &Line2::layout.descriptorSetLayout)){ vkDefaultCases; }
	}




	{ //Create pipeline layout
		uint64 fileLength = 0;
		uint32* code = core::shaders::loadSpv(&fileLength, (core::shaders::shaderPath + "Line2.spv").begin());
		Line2::layout.shaderModule = core::shaders::createModule(core::dvc::graphics.ld, code, fileLength);

		Line2::layout.shaderStageCreateInfo = vk::PipelineShaderStageCreateInfo()
			.setStage  (vk::ShaderStageFlagBits::eCompute)
			.setModule (Line2::layout.shaderModule)
			.setPName  ("main")
		;

		auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
			.setSetLayoutCount (1)
			.setPSetLayouts    (&Line2::layout.descriptorSetLayout)
		;
		switch(core::dvc::graphics.ld.createPipelineLayout(&pipelineLayoutCreateInfo, nullptr, &Line2::layout.pipelineLayout)){ vkDefaultCases; }
	}
}
}