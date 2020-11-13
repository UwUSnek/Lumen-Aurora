﻿#include "LuxEngine/Core/Compute/Compute.hpp"
#include "LuxEngine/Core/Compute/CBuffers.hpp"
#include "LuxEngine/Core/Compute/CShader.hpp"
#include "LuxEngine/Core/Graphics/GSwapchain.hpp"
#include "LuxEngine/Core/Devices.hpp"
#include "LuxEngine/Core/Core.hpp"
#include "LuxEngine/Core/LuxAutoInit.hpp"




namespace lux::core::c{
	VkCommandPool				copyCommandPool = nullptr;
	RTArray<VkCommandBuffer>	copyCommandBuffers;
	VkCommandBuffer				clearCommandBuffer = nullptr;



	//Initializes the compute objects
	//> Engine internal use
	void init(){
		{ //Initialize window buffers and count
			g::wnd::gpuCellWindowOutput_i	= rem::allocBck(g::wnd::width * g::wnd::height * 4, 	CellClass::AUTO, AllocType::DEDICATED_STORAGE); //A8-R8-G8-B8 UI
			g::wnd::gpuCellWindowOutput		= rem::allocBck(g::wnd::width * g::wnd::height * 4 * 4,	CellClass::AUTO, AllocType::DEDICATED_STORAGE); //A32-R32-G32-B32 UF
			g::wnd::gpuCellWindowZBuffer 	= rem::allocBck(g::wnd::width * g::wnd::height * 4, 	CellClass::AUTO, AllocType::DEDICATED_STORAGE);

			g::wnd::gpuCellWindowSize = rem::allocBck(4 * 2,  CellClass::AUTO, AllocType::SHARED_STORAGE);	//Create cell for window size //TODO use dedicated storage and update every time
			uint32* pwindowSize = (uint32*)(g::wnd::gpuCellWindowSize->map());								//Map window size cell //TODO use gpu pointer instead of raw cell
			pwindowSize[0] = g::swapchain::swapchainExtent.width;											//Set width
			pwindowSize[1] = g::swapchain::swapchainExtent.height;											//Set height
			g::wnd::gpuCellWindowSize->unmap();																//Unmap
		}

		{ //#LLID CCB0000 Create copy command buffers
			copyCommandBuffers.resize(g::swapchain::swapchainImages.count( ));			//Resize the command buffer array in the shader
			shaders::createDefaultCommandBuffers( );									//Create command buffers and command pool
		}

		{ //Create default shaders //TODO fix
			shaders::CShadersLayouts.resize(ShaderLayout::LUX_DEF_SHADER_NUM);
			shaders::createDefLayout(LUX_DEF_SHADER_2D_LINE, 4, { 0, 0, 0, 1 });
			shaders::createDefLayout(LUX_DEF_SHADER_2D_BORDER, 4, { 0, 0, 0, 1 });
			shaders::createDefLayout(LUX_DEF_SHADER_CLEAR, 4, { 0, 0, 0, 0 });

			shaders::clearShader = shaders::newShader(
				{ g::wnd::gpuCellWindowOutput, g::wnd::gpuCellWindowOutput_i, core::g::wnd::gpuCellWindowZBuffer, g::wnd::gpuCellWindowSize },
				LUX_DEF_SHADER_CLEAR, (g::wnd::width * g::wnd::height) / (32 * 32) + 1, 1, 1
			);
		}
	}




	//Frees and destroys the compute objects //TODO fix
	//> Engine internal use
	void computeCleanup( ) {
		for(uint32 i = 0; i < /*rem::buffers.count( )*/(uint32)CellClassIndex::NUM * (uint32)AllocType::NUM; ++i) {
			for(uint32 j = 0; j < rem::buffers[i].buffers.size( ); ++j){
				if(rem::buffers[i].buffers.isValid(j)){
					vkDestroyBuffer(dvc::compute.LD, rem::buffers[i].buffers[j].buffer, nullptr);
					vkFreeMemory(dvc::compute.LD, rem::buffers[i].buffers[j].memory, nullptr);
				}
			}
		}
	}
}