
//####################################################################################
// This file was generated by Tools/Build/Generators/GenInterfaces
// Changes could be overwritten without notice
//####################################################################################
#include "Generated/Shaders/Border2.ilsl.hpp"

#include "Lynx/Core/Init.hpp"
#include "Lynx/Core/Render/Shaders/Shader.hpp"
#include "Lynx/Core/Render/Render.hpp"



namespace lnx::shd::gsi{


	void Border2::spawn(
		const u32v3 vGroupCount, core::RenderCore& pRenderCore
	) {
		pRenderCore.addObject_m.lock();

			createDescriptorSets();
			createCommandBuffers(vGroupCount, pRenderCore);
			pRenderCore.swp.shadersCBs.add(commandBuffers[0]);
		pRenderCore.addObject_m.unlock();
	}








	void Border2::createDescriptorSets() {
		vk::DescriptorPoolSize sizes[2] = {
			vk::DescriptorPoolSize().setType(vk::DescriptorType::eStorageBuffer).setDescriptorCount(0),{}
		};
		auto poolInfo = vk::DescriptorPoolCreateInfo()
			.setFlags         (vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
			.setMaxSets       (1)
			.setPoolSizeCount (1)
			.setPPoolSizes    (sizes)
		;
		switch(core::dvc::g_graphics().ld.createDescriptorPool(&poolInfo, nullptr, &descriptorPool)) {
			case vk::Result::eErrorFragmentationEXT:  dbg::logError("Fragmentation error");  break;
			vkDefaultCases;
		}



		auto allocateSetInfo = vk::DescriptorSetAllocateInfo()
			.setDescriptorPool     (descriptorPool)
			.setDescriptorSetCount (1)
			.setPSetLayouts        (&g_Border2_layout().descriptorSetLayout)
		;
		switch(core::dvc::g_graphics().ld.allocateDescriptorSets(&allocateSetInfo, &descriptorSet)) {
			case vk::Result::eErrorFragmentedPool:    dbg::logError("Fragmented pool");      break;
			case vk::Result::eErrorOutOfPoolMemory:   dbg::logError("Out of pool memory");   break;
			vkDefaultCases;
		}



		vk::WriteDescriptorSet writeSets[0];
		core::dvc::g_graphics().ld.updateDescriptorSets(0, writeSets, 0, nullptr);
	}








	void Border2::createCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore) {
		auto allocateCbInfo = vk::CommandBufferAllocateInfo()
			.setCommandPool        (pRenderCore.commandPool)
			.setLevel              (vk::CommandBufferLevel::ePrimary)
			.setCommandBufferCount (1)
		;
		commandBuffers.resize(1);
		switch(core::dvc::g_graphics().ld.allocateCommandBuffers(&allocateCbInfo, commandBuffers.begin())) { vkDefaultCases; }

		auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		switch(commandBuffers[0].begin(beginInfo)) { vkDefaultCases; }
		commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pRenderCore.pipelines[g_Border2_pipelineIndex()]);
		commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, g_Border2_layout().pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);
		switch(commandBuffers[0].end()) { vkDefaultCases; }
	}








	void Border2::updateCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore) {
		auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		switch(commandBuffers[0].begin(beginInfo)) { vkDefaultCases; }
		commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pRenderCore.pipelines[g_Border2_pipelineIndex()]);
		commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, g_Border2_layout().pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);
		switch(commandBuffers[0].end()) { vkDefaultCases; }
	}








	void Border2::destroy() {
		//TODO
	}








	_lnx_init_var_value_def((InterfaceLayout), Border2_layout,        lnx::shd::gsi) {}
	_lnx_init_var_value_def((uint32),          Border2_pipelineIndex, lnx::shd::gsi) { *pVar = core::shaders::g_pipelineNum()++; }
	_lnx_init_fun_def(LNX_H_BORDER2, lnx::shd::gsi) {
		core::shaders::g_pipelineLayouts().resize(core::shaders::g_pipelineNum());
		core::shaders::g_pipelineLayouts()[g_Border2_pipelineIndex()] = &g_Border2_layout();
		{ //Create descriptor set layout
			vk::DescriptorSetLayoutBinding bindingLayouts[0];

			auto layoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
				.setBindingCount (0)
				.setPBindings    (bindingLayouts)
			;
			//Create the descriptor set layout
			switch(core::dvc::g_graphics().ld.createDescriptorSetLayout(&layoutCreateInfo, nullptr, &g_Border2_layout().descriptorSetLayout)) { vkDefaultCases; }
		}




		{ //Create pipeline layout
			uint64 fileLength = 0;
			uint32* code = core::shaders::loadSpv(&fileLength, "Lynx/src/Generated/Shaders/Lynx-Border2.ilsl.comp.spv");
			g_Border2_layout().shaderModule = core::shaders::createModule(core::dvc::g_graphics().ld, code, fileLength);

			g_Border2_layout().shaderStageCreateInfo = vk::PipelineShaderStageCreateInfo()
				.setStage  (vk::ShaderStageFlagBits::eCompute)
				.setModule (g_Border2_layout().shaderModule)
				.setPName  ("main")
			;

			auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
				.setSetLayoutCount (1)
				.setPSetLayouts    (&g_Border2_layout().descriptorSetLayout)
			;
			switch(core::dvc::g_graphics().ld.createPipelineLayout(&pipelineLayoutCreateInfo, nullptr, &g_Border2_layout().pipelineLayout)) { vkDefaultCases; }
		}
	}
}