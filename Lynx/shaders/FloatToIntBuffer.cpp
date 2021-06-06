
//####################################################################################
// This file was generated automatically. Changes could be overwritten without notice
//####################################################################################

#include "Lynx/shaders/FloatToIntBuffer.hpp"
#include "Lynx/Core/AutoInit.hpp"
#include "Lynx/Core/Render/Window/Window.hpp"
#include "Lynx/Core/Render/Shaders/Shader.hpp"
#define LNX_H_FLOATTOINTBUFFER



namespace lnx::shd{


	void FloatToIntBuffer::create(vram::ptr<f32v4, eVRam, eStorage> pSrc, vram::ptr<u32, eVRam, eStorage> pDst, vram::ptr<u32v2, eVRam, eStorage> pWsize, vram::ptr<u32, eVRam, eStorage> pZbuff, const u32v3 vGroupCount, Window& pWindow){
		pWindow.renderCore.addObject_m.lock();
			_src.vdata = (vram::ptr<char, eVRam, eStorage>)pSrc;
			_dst.vdata = (vram::ptr<char, eVRam, eStorage>)pDst;
			_wsize.vdata = (vram::ptr<char, eVRam, eStorage>)pWsize;
			_zbuff.vdata = (vram::ptr<char, eVRam, eStorage>)pZbuff;

			createDescriptorSets();
			createCommandBuffers(vGroupCount, pWindow);
			pWindow.renderCore.swp.shadersCBs.add(commandBuffers[0]);
		pWindow.renderCore.addObject_m.unlock();
	}


	void FloatToIntBuffer::createDescriptorSets(){
		vk::DescriptorPoolSize sizes[2] = {
			vk::DescriptorPoolSize().setType(vk::DescriptorType::eStorageBuffer).setDescriptorCount(4),
			{}
		};
		auto poolInfo = vk::DescriptorPoolCreateInfo()
			.setFlags         (vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)
			.setMaxSets       (1)
			.setPoolSizeCount (1)
			.setPPoolSizes    (sizes)
		;
		switch(core::dvc::graphics.ld.createDescriptorPool(&poolInfo, nullptr, &descriptorPool)){
			case vk::Result::eErrorFragmentationEXT:  dbg::printError("Fragmentation error");  break;
			vkDefaultCases;
		}



		auto allocateSetInfo = vk::DescriptorSetAllocateInfo()
			.setDescriptorPool     (descriptorPool)
			.setDescriptorSetCount (1)
			.setPSetLayouts        (&FloatToIntBuffer::layout.descriptorSetLayout)
		;
		switch(core::dvc::graphics.ld.allocateDescriptorSets(&allocateSetInfo, &descriptorSet)){
			case vk::Result::eErrorFragmentedPool:    dbg::printError("Fragmented pool");      break;
			case vk::Result::eErrorOutOfPoolMemory:   dbg::printError("Out of pool memory");   break;
			vkDefaultCases;
		}



		vk::WriteDescriptorSet writeSets[4];
		auto bufferInfo0 = vk::DescriptorBufferInfo()
			.setBuffer (_src.vdata.cell->csc.buffer)
			.setOffset (_src.vdata.cell->localOffset)
			.setRange  (_src.vdata.cell->cellSize)
		;
		writeSets[0] = vk::WriteDescriptorSet()
			.setDstSet          (descriptorSet)
			.setDstBinding      (0)
			.setDescriptorCount (1)
			.setDescriptorType  (vk::DescriptorType::eStorageBuffer)
			.setPBufferInfo     (&bufferInfo0)
		;

		auto bufferInfo1 = vk::DescriptorBufferInfo()
			.setBuffer (_dst.vdata.cell->csc.buffer)
			.setOffset (_dst.vdata.cell->localOffset)
			.setRange  (_dst.vdata.cell->cellSize)
		;
		writeSets[1] = vk::WriteDescriptorSet()
			.setDstSet          (descriptorSet)
			.setDstBinding      (1)
			.setDescriptorCount (1)
			.setDescriptorType  (vk::DescriptorType::eStorageBuffer)
			.setPBufferInfo     (&bufferInfo1)
		;

		auto bufferInfo2 = vk::DescriptorBufferInfo()
			.setBuffer (_wsize.vdata.cell->csc.buffer)
			.setOffset (_wsize.vdata.cell->localOffset)
			.setRange  (_wsize.vdata.cell->cellSize)
		;
		writeSets[2] = vk::WriteDescriptorSet()
			.setDstSet          (descriptorSet)
			.setDstBinding      (2)
			.setDescriptorCount (1)
			.setDescriptorType  (vk::DescriptorType::eStorageBuffer)
			.setPBufferInfo     (&bufferInfo2)
		;

		auto bufferInfo3 = vk::DescriptorBufferInfo()
			.setBuffer (_zbuff.vdata.cell->csc.buffer)
			.setOffset (_zbuff.vdata.cell->localOffset)
			.setRange  (_zbuff.vdata.cell->cellSize)
		;
		writeSets[3] = vk::WriteDescriptorSet()
			.setDstSet          (descriptorSet)
			.setDstBinding      (3)
			.setDescriptorCount (1)
			.setDescriptorType  (vk::DescriptorType::eStorageBuffer)
			.setPBufferInfo     (&bufferInfo3)
		;
		core::dvc::graphics.ld.updateDescriptorSets(4, writeSets, 0, nullptr);
	}








	void FloatToIntBuffer::createCommandBuffers(const u32v3 vGroupCount, Window& pWindow){
		auto allocateCbInfo = vk::CommandBufferAllocateInfo()
			.setCommandPool        (pWindow.renderCore.commandPool)
			.setLevel              (vk::CommandBufferLevel::ePrimary)
			.setCommandBufferCount (1)
		;
		commandBuffers.resize(1);
		switch(core::dvc::graphics.ld.allocateCommandBuffers(&allocateCbInfo, commandBuffers.begin())){ vkDefaultCases; }

		auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		switch(commandBuffers[0].begin(beginInfo)){ vkDefaultCases; }
		commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pWindow.renderCore.pipelines[FloatToIntBuffer::pipelineIndex]);
		commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, FloatToIntBuffer::layout.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);
		switch(commandBuffers[0].end()){ vkDefaultCases; }
	}








	void FloatToIntBuffer::updateCommandBuffers(const u32v3 vGroupCount, Window& pWindow){
		auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		switch(commandBuffers[0].begin(beginInfo)){ vkDefaultCases; }
		commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pWindow.renderCore.pipelines[FloatToIntBuffer::pipelineIndex]);
		commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, FloatToIntBuffer::layout.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
		commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);
		switch(commandBuffers[0].end()){ vkDefaultCases; }
	}








	void FloatToIntBuffer::destroy(){
		//TODO
	}








	Shader_b::Layout FloatToIntBuffer::layout;
	uint32 FloatToIntBuffer::pipelineIndex = core::shaders::pipelineNum++;
	LnxAutoInit(LNX_H_FLOATTOINTBUFFER){
		core::shaders::pipelineLayouts.resize(core::shaders::pipelineNum);
		core::shaders::pipelineLayouts[FloatToIntBuffer::pipelineIndex] = &FloatToIntBuffer::layout;
		{ //Create descriptor set layout
			vk::DescriptorSetLayoutBinding bindingLayouts[4];
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
				.setDescriptorType     (vk::DescriptorType::eStorageBuffer)
				.setDescriptorCount    (1)
				.setStageFlags         (vk::ShaderStageFlagBits::eCompute)
				.setPImmutableSamplers (nullptr)
			;

			bindingLayouts[3] = vk::DescriptorSetLayoutBinding()
				.setBinding            (3)
				.setDescriptorType     (vk::DescriptorType::eStorageBuffer)
				.setDescriptorCount    (1)
				.setStageFlags         (vk::ShaderStageFlagBits::eCompute)
				.setPImmutableSamplers (nullptr)
			;

			auto layoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
				.setBindingCount (4)
				.setPBindings    (bindingLayouts)
			;
			//Create the descriptor set layout
			switch(core::dvc::graphics.ld.createDescriptorSetLayout(&layoutCreateInfo, nullptr, &FloatToIntBuffer::layout.descriptorSetLayout)){ vkDefaultCases; }
		}




		{ //Create pipeline layout
			uint64 fileLength = 0;
			uint32* code = core::shaders::loadSpv(&fileLength, (core::shaders::shaderPath + "FloatToIntBuffer.spv").begin());
			FloatToIntBuffer::layout.shaderModule = core::shaders::createModule(core::dvc::graphics.ld, code, fileLength);

			FloatToIntBuffer::layout.shaderStageCreateInfo = vk::PipelineShaderStageCreateInfo()
				.setStage  (vk::ShaderStageFlagBits::eCompute)
				.setModule (FloatToIntBuffer::layout.shaderModule)
				.setPName  ("main")
			;

			auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
				.setSetLayoutCount (1)
				.setPSetLayouts    (&FloatToIntBuffer::layout.descriptorSetLayout)
			;
			switch(core::dvc::graphics.ld.createPipelineLayout(&pipelineLayoutCreateInfo, nullptr, &FloatToIntBuffer::layout.pipelineLayout)){ vkDefaultCases; }
		}
	}
}