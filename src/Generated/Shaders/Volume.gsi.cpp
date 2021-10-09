
//####################################################################################
// This file was generated by Tools/Build/GenInterfaces
// Changes could be overwritten without notice
//####################################################################################
#include "Generated/Shaders/Volume.gsi.hpp"

#include "Lynx/Core/Init.hpp"
#include "Lynx/Core/Render/Shaders/Shader.hpp"
#include "Lynx/Core/Render/Render.hpp"



namespace lnx::shd::gsi{


	void Volume::spawn(
		const l_outcol& pOutcol,
		const l_wsize& pWsize,
		const u32v3 vGroupCount, core::RenderCore& pRenderCore
	){
		pRenderCore.addObject_m.lock();
			outcol = pOutcol;
			wsize = pWsize;

			createDescriptorSets();
			createCommandBuffers(vGroupCount, pRenderCore);
			pRenderCore.swp.shadersCBs.add(commandBuffers[0]);
		pRenderCore.addObject_m.unlock();
	}


	void Volume::createDescriptorSets(){
		vk::DescriptorPoolSize sizes[2] = {
			vk::DescriptorPoolSize().setType(vk::DescriptorType::eStorageBuffer).setDescriptorCount(2),
			vk::DescriptorPoolSize().setType(vk::DescriptorType::eUniformBuffer).setDescriptorCount(1)
		};
		auto poolInfo = vk::DescriptorPoolCreateInfo()
			.setFlags         (vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
			.setMaxSets       (1)
			.setPoolSizeCount (2)
			.setPPoolSizes    (sizes)
		;
		switch(core::dvc::g_graphics().ld.createDescriptorPool(&poolInfo, nullptr, &descriptorPool)){
			case vk::Result::eErrorFragmentationEXT:  dbg::logError("Fragmentation error");  break;
			vkDefaultCases;
		}



		auto allocateSetInfo = vk::DescriptorSetAllocateInfo()
			.setDescriptorPool     (descriptorPool)
			.setDescriptorSetCount (1)
			.setPSetLayouts        (&g_Volume_layout().descriptorSetLayout)
		;
		switch(core::dvc::g_graphics().ld.allocateDescriptorSets(&allocateSetInfo, &descriptorSet)){
			case vk::Result::eErrorFragmentedPool:    dbg::logError("Fragmented pool");      break;
			case vk::Result::eErrorOutOfPoolMemory:   dbg::logError("Out of pool memory");   break;
			vkDefaultCases;
		}



		vk::WriteDescriptorSet writeSets[3];
		auto bufferInfo0 = vk::DescriptorBufferInfo()
			.setBuffer (outcol.vdata.cell->csc.buffer)
			.setOffset (outcol.vdata.cell->localOffset)
			.setRange  (outcol.vdata.cell->cellSize)
		;
		writeSets[0] = vk::WriteDescriptorSet()
			.setDstSet          (descriptorSet)
			.setDstBinding      (0)
			.setDescriptorCount (1)
			.setDescriptorType  (vk::DescriptorType::eStorageBuffer)
			.setPBufferInfo     (&bufferInfo0)
		;

		auto bufferInfo1 = vk::DescriptorBufferInfo()
			.setBuffer (wsize.vdata.cell->csc.buffer)
			.setOffset (wsize.vdata.cell->localOffset)
			.setRange  (wsize.vdata.cell->cellSize)
		;
		writeSets[1] = vk::WriteDescriptorSet()
			.setDstSet          (descriptorSet)
			.setDstBinding      (1)
			.setDescriptorCount (1)
			.setDescriptorType  (vk::DescriptorType::eStorageBuffer)
			.setPBufferInfo     (&bufferInfo1)
		;

		auto bufferInfo2 = vk::DescriptorBufferInfo()
			.setBuffer (data.vdata.cell->csc.buffer)
			.setOffset (data.vdata.cell->localOffset)
			.setRange  (data.vdata.cell->cellSize)
		;
		writeSets[2] = vk::WriteDescriptorSet()
			.setDstSet          (descriptorSet)
			.setDstBinding      (2)
			.setDescriptorCount (1)
			.setDescriptorType  (vk::DescriptorType::eUniformBuffer)
			.setPBufferInfo     (&bufferInfo2)
		;
		core::dvc::g_graphics().ld.updateDescriptorSets(3, writeSets, 0, nullptr);
	}








	void Volume::createCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore){
		auto allocateCbInfo = vk::CommandBufferAllocateInfo()
			.setCommandPool        (pRenderCore.commandPool)
			.setLevel              (vk::CommandBufferLevel::ePrimary)
			.setCommandBufferCount (1)
		;
		commandBuffers.resize(1);
		switch(core::dvc::g_graphics().ld.allocateCommandBuffers(&allocateCbInfo, commandBuffers.begin())){ vkDefaultCases; }

		auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		switch(commandBuffers[0].begin(beginInfo)){ vkDefaultCases; }
		commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pRenderCore.pipelines[g_Volume_pipelineIndex()]);
		commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, g_Volume_layout().pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);
		switch(commandBuffers[0].end()){ vkDefaultCases; }
	}








	void Volume::updateCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore){
		auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		switch(commandBuffers[0].begin(beginInfo)){ vkDefaultCases; }
		commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pRenderCore.pipelines[g_Volume_pipelineIndex()]);
		commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, g_Volume_layout().pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);
		switch(commandBuffers[0].end()){ vkDefaultCases; }
	}








	void Volume::destroy(){
		//TODO
	}








	_lnx_init_var_value_def((InterfaceLayout), Volume_layout,        lnx::shd::gsi){}
	_lnx_init_var_value_def((uint32),          Volume_pipelineIndex, lnx::shd::gsi){ pVar = core::shaders::g_pipelineNum()++; }
	_lnx_init_fun_def(LNX_H_VOLUME, lnx::shd::gsi){
		core::shaders::g_pipelineLayouts().resize(core::shaders::g_pipelineNum());
		core::shaders::g_pipelineLayouts()[g_Volume_pipelineIndex()] = &g_Volume_layout();
		{ //Create descriptor set layout
			vk::DescriptorSetLayoutBinding bindingLayouts[3];
			bindingLayouts[0] = vk::DescriptorSetLayoutBinding()
				.setBinding            (0)
				.setDescriptorType     (vk::DescriptorType::eStorageBuffer)
				.setDescriptorCount    (1)
				.setStageFlags         (vk::ShaderStageFlagBits::eCompute)
				.setPImmutableSamplers (nullptr)
			;

			bindingLayouts[1] = vk::DescriptorSetLayoutBinding()
				.setBinding            (1)
				.setDescriptorType     (vk::DescriptorType::eStorageBuffer)
				.setDescriptorCount    (1)
				.setStageFlags         (vk::ShaderStageFlagBits::eCompute)
				.setPImmutableSamplers (nullptr)
			;

			bindingLayouts[2] = vk::DescriptorSetLayoutBinding()
				.setBinding            (2)
				.setDescriptorType     (vk::DescriptorType::eUniformBuffer)
				.setDescriptorCount    (1)
				.setStageFlags         (vk::ShaderStageFlagBits::eCompute)
				.setPImmutableSamplers (nullptr)
			;

			auto layoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
				.setBindingCount (3)
				.setPBindings    (bindingLayouts)
			;
			//Create the descriptor set layout
			switch(core::dvc::g_graphics().ld.createDescriptorSetLayout(&layoutCreateInfo, nullptr, &g_Volume_layout().descriptorSetLayout)){ vkDefaultCases; }
		}




		{ //Create pipeline layout
			uint64 fileLength = 0;
			uint32* code = core::shaders::loadSpv(&fileLength, "Lynx/src/Generated/Shaders/Volume.spv");
			g_Volume_layout().shaderModule = core::shaders::createModule(core::dvc::g_graphics().ld, code, fileLength);

			g_Volume_layout().shaderStageCreateInfo = vk::PipelineShaderStageCreateInfo()
				.setStage  (vk::ShaderStageFlagBits::eCompute)
				.setModule (g_Volume_layout().shaderModule)
				.setPName  ("main")
			;

			auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
				.setSetLayoutCount (1)
				.setPSetLayouts    (&g_Volume_layout().descriptorSetLayout)
			;
			switch(core::dvc::g_graphics().ld.createPipelineLayout(&pipelineLayoutCreateInfo, nullptr, &g_Volume_layout().pipelineLayout)){ vkDefaultCases; }
		}
	}
}